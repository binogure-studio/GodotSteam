#include "godotsteam_user.h"

GodotSteamUser *GodotSteamUser::singleton = NULL;

GodotSteamUser::GodotSteamUser():
  callbackGetAuthSessionTicketResponse(this, &GodotSteamUser::_get_auth_session_ticket_response),
  callbackValidateAuthTicketResponse(this, &GodotSteamUser::_validate_auth_ticket_response),
  callbackSteamServerConnectFailure(this, &GodotSteamUser::_steam_server_connect_failure),
  callbackSteamServersConnected(this, &GodotSteamUser::_steam_servers_connected),
  callbackSteamServersDisconnected(this, &GodotSteamUser::_steam_servers_disconnected)
{
  singleton = this;
}

GodotSteamUser::~GodotSteamUser() { singleton = NULL; }

GodotSteamUser *GodotSteamUser::get_singleton() {
  if (GodotSteamUser::singleton == NULL) {
    GodotSteamUser::singleton = new GodotSteamUser();
  }

  return GodotSteamUser::singleton;
}

void GodotSteamUser::reset_singleton() {
  delete GodotSteamUser::singleton;

  GodotSteamUser::singleton = NULL;
}

bool GodotSteamUser::isSteamUserReady() { return SteamUser() != NULL; }

uint64_t GodotSteamUser::getSteamID() {
  STEAM_FAIL_COND_V(!isSteamUserReady(), 0);

  CSteamID cSteamID = SteamUser()->GetSteamID();
  return cSteamID.ConvertToUint64();
}

bool GodotSteamUser::loggedOn() {
  if (!isSteamUserReady()) {
    return false;
  }
  return SteamUser()->BLoggedOn();
}

uint64_t GodotSteamUser::getPlayerSteamLevel() {
  STEAM_FAIL_COND_V(!isSteamUserReady(), 0);

  return SteamUser()->GetPlayerSteamLevel();
}

String GodotSteamUser::getUserDataFolder() {
  STEAM_FAIL_COND_V(!isSteamUserReady(), "");

  const uint64_t bufferSize = 256;
  char *buffer = new char[bufferSize];
  SteamUser()->GetUserDataFolder((char *)buffer, bufferSize);
  String data_path = buffer;
  delete[] buffer;
  return data_path;
}

void GodotSteamUser::advertiseGame(const String &serverIp, uint64_t port) {
  STEAM_FAIL_COND(!isSteamUserReady());

  // Resolve address and convert it from IPAddress struct to uint32_t
  IPAddress addr;
  if (serverIp.is_valid_ip_address()) {
    addr = serverIp;
  } else {
    addr = IP::get_singleton()->resolve_hostname(serverIp, IP::TYPE_IPV4);
  }
  // Resolution failed - Godot 3.0 has is_invalid() to check this
  if (addr == IPAddress()) {
    return;
  }
  uint32_t ip4 = *((uint32_t *)addr.get_ipv4());
  // Swap the bytes
  uint8_t *ip4_p = (uint8_t *)&ip4;
  for (uint64_t index = 0; index < 2; index++) {
    uint8_t temp = ip4_p[index];
    ip4_p[index] = ip4_p[3 - index];
    ip4_p[3 - index] = temp;
  }
  CSteamID gameserverID = SteamUser()->GetSteamID();
  SteamUser()->AdvertiseGame(gameserverID, *((uint32_t *)ip4_p), port);
}

// Trading Card badges data access, if you only have one set of cards, the
// series will be 1 The user has can have two different badges for a series; the
// regular (max level 5) and the foil (max level 1)
uint64_t GodotSteamUser::getGameBadgeLevel(uint64_t series, bool foil) {
  STEAM_FAIL_COND_V(!isSteamUserReady(), 0);

  return SteamUser()->GetGameBadgeLevel(series, foil);
}

// Authenticate the ticket from the entity Steam ID to be sure it is valid and isn't reused.
uint64_t GodotSteamUser::beginAuthSession(PackedByteArray ticket, uint64_t ticketSize, uint64_t steamId) {
  STEAM_FAIL_COND_V(!isSteamUserReady(), 0);

  CSteamID authSteamId = (steamId == 0) ? SteamUser()->GetSteamID() : (uint64)steamId;

  return EBeginAuthSessionResult(SteamUser()->BeginAuthSession(ticket.ptr(), ticketSize, authSteamId));
}

void GodotSteamUser::cancelAuthTicket(uint64_t authTicket) {
  STEAM_FAIL_COND(!isSteamUserReady());

  SteamUser()->CancelAuthTicket(authTicket);
}

void GodotSteamUser::endAuthSession(uint64_t steamId) {
  STEAM_FAIL_COND(!isSteamUserReady());

  CSteamID authSteamId = (steamId == 0) ? SteamUser()->GetSteamID() : (uint64)steamId;

  SteamUser()->EndAuthSession(authSteamId);
}

Dictionary GodotSteamUser::getAuthSessionTicket() {
  Dictionary authTicket;

  STEAM_FAIL_COND_V(!isSteamUserReady(), authTicket);

  uint32_t ticketSize = AUTH_TICKET_SIZE;
  uint64_t steamId = SteamUser()->GetSteamID().ConvertToUint64();
  unsigned char md5_hash[16];

  PackedByteArray buffer;
  buffer.resize(ticketSize);

  // If no reference is passed, just use NULL
  // Not pretty but will work for now
  const SteamNetworkingIdentity identity = GodotSteamNetworkingSockets::get_singleton()->getOrCreateNetworkingIdentity(steamId);

  // Add this data to the dictionary
  authTicket["id"] = SteamUser()->GetAuthSessionTicket(buffer.ptrw(), ticketSize, &ticketSize, &identity);

  // Generate md5
  CryptoCore::md5(buffer.ptr(), buffer.size(), md5_hash);

  authTicket["buffer"] = buffer;
  authTicket["size"] = ticketSize;
  authTicket["md5"] = String::hex_encode_buffer(md5_hash, 16);

  return authTicket;
}

void GodotSteamUser::_get_auth_session_ticket_response(GetAuthSessionTicketResponse_t *data) {
  uint32 authTicket = data->m_hAuthTicket;
  int result = data->m_eResult;

  emit_signal("get_auth_session_ticket_response", authTicket, result);
}

void GodotSteamUser::_validate_auth_ticket_response(ValidateAuthTicketResponse_t *data) {
  uint64_t authId = data->m_SteamID.ConvertToUint64();
  uint32_t response = data->m_eAuthSessionResponse;
  uint64_t ownerId = data->m_OwnerSteamID.ConvertToUint64();

  emit_signal("validate_auth_ticket_response", authId, response, ownerId);
}

void GodotSteamUser::_steam_server_connect_failure(SteamServerConnectFailure_t *callData) {

  emit_signal("steam_server_connection_failed", (int)callData->m_eResult, callData->m_bStillRetrying);
}

void GodotSteamUser::_steam_servers_connected(SteamServersConnected_t *callData) {

  emit_signal("steam_server_connected");
}

void GodotSteamUser::_steam_servers_disconnected(SteamServersDisconnected_t *callData) {

  emit_signal("steam_server_disconnected", (int)callData->m_eResult);
}


void GodotSteamUser::_bind_methods() {
  ClassDB::bind_method("getSteamID", &GodotSteamUser::getSteamID);
  ClassDB::bind_method("loggedOn", &GodotSteamUser::loggedOn);
  ClassDB::bind_method("getPlayerSteamLevel",
                            &GodotSteamUser::getPlayerSteamLevel);
  ClassDB::bind_method("getUserDataFolder",
                            &GodotSteamUser::getUserDataFolder);
  ClassDB::bind_method(D_METHOD("advertiseGame", "serverOp", "port"),
                            &GodotSteamUser::advertiseGame);
  ClassDB::bind_method("getGameBadgeLevel",
                            &GodotSteamUser::getGameBadgeLevel);

  ClassDB::bind_method(D_METHOD("beginAuthSession", "ticket", "ticketSize", "steamId"),
                            &GodotSteamUser::beginAuthSession);

  ClassDB::bind_method(D_METHOD("cancelAuthTicket", "authTicket"),
                            &GodotSteamUser::cancelAuthTicket);

  ClassDB::bind_method(D_METHOD("endAuthSession", "steamId"),
                            &GodotSteamUser::endAuthSession);

  ClassDB::bind_method(D_METHOD("getAuthSessionTicket"),
                            &GodotSteamUser::getAuthSessionTicket);

  ADD_SIGNAL(MethodInfo("get_auth_session_ticket_response",
    PropertyInfo(Variant::INT, "auth_ticket"),
    PropertyInfo(Variant::INT, "result")
  ));

  ADD_SIGNAL(MethodInfo("validate_auth_ticket_response",
    PropertyInfo(Variant::INT, "auth_id"),
    PropertyInfo(Variant::INT, "response"),
    PropertyInfo(Variant::INT, "owner_id")
  ));

  ADD_SIGNAL(MethodInfo("steam_server_connection_failed",
    PropertyInfo(Variant::INT, "reason"),
    PropertyInfo(Variant::BOOL, "still_retrying")
  ));

  ADD_SIGNAL(MethodInfo("steam_server_connected"));
  ADD_SIGNAL(MethodInfo("steam_server_disconnected",
    PropertyInfo(Variant::INT, "reason")
  ));

  BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_OK);
  BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_INVALID_TICKET);
  BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_DUPLICATE_REQUEST);
  BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_INVALID_VERSION);
  BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_GAME_MISMATCH);
  BIND_CONSTANT(BEGIN_AUTH_SESSION_RESULT_EXPIRED_TICKET);

  BIND_CONSTANT(AUTH_SESSION_RESPONSE_OK);
  BIND_CONSTANT(AUTH_SESSION_RESPONSE_USER_NOT_CONNECTED_TO_STEAM);
  BIND_CONSTANT(AUTH_SESSION_RESPONSE_NO_LICENSE_OR_EXPIRED);
  BIND_CONSTANT(AUTH_SESSION_RESPONSE_VAC_BANNED);
  BIND_CONSTANT(AUTH_SESSION_RESPONSE_LOGGED_IN_ELSEWHERE);
  BIND_CONSTANT(AUTH_SESSION_RESPONSE_VAC_CHECK_TIMED_OUT);
  BIND_CONSTANT(AUTH_SESSION_RESPONSE_AUTH_TICKET_CANCELED);
  BIND_CONSTANT(AUTH_SESSION_RESPONSE_AUTH_TICKET_INVALID_ALREADY_USED);
  BIND_CONSTANT(AUTH_SESSION_RESPONSE_AUTH_TICKET_INVALID);
  BIND_CONSTANT(AUTH_SESSION_RESPONSE_PUBLISHER_ISSUED_BAN);
  BIND_CONSTANT(AUTH_SESSION_RESPONSE_AUTH_TICKET_NETWORK_IDENTITY_FAILURE);
}
