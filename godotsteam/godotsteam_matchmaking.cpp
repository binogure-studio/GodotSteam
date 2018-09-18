#include "godotsteam_matchmaking.h"

GodotSteamMatchmaking *GodotSteamMatchmaking::singleton = NULL;

GodotSteamMatchmaking::GodotSteamMatchmaking() { singleton = this; }
GodotSteamMatchmaking::~GodotSteamMatchmaking() { singleton = NULL; }

GodotSteamMatchmaking *GodotSteamMatchmaking::get_singleton() {
  if (GodotSteamMatchmaking::singleton == NULL) {
    GodotSteamMatchmaking::singleton = new GodotSteamMatchmaking();
  }

  return GodotSteamMatchmaking::singleton;
}

void GodotSteamMatchmaking::reset_singleton() {
  delete GodotSteamMatchmaking::singleton;

  GodotSteamMatchmaking::singleton = NULL;
}
bool GodotSteamMatchmaking::isSteamMatchmakingReady() {
  return SteamMatchmaking() != NULL;
}

void GodotSteamMatchmaking::createLobby(int lobbyType, int cMaxMembers) {
  if (!isSteamMatchmakingReady()) {
    return;
  }

  SteamMatchmaking()->CreateLobby((ELobbyType) lobbyType, cMaxMembers);
}
// Join an existing lobby
void GodotSteamMatchmaking::joinLobby(int steamIDLobby) {
  if (!isSteamMatchmakingReady()) {
    return;
  }
  CSteamID lobbyID = GodotSteamUtils::get_singleton()->createSteamID(steamIDLobby);
  SteamMatchmaking()->JoinLobby(lobbyID);
}
// Leave a lobby, this will take effect immediately on the client side, other
// users will be notified by LobbyChatUpdate_t callback
void GodotSteamMatchmaking::leaveLobby(int steamIDLobby) {
  CSteamID lobbyID = GodotSteamUtils::get_singleton()->createSteamID(steamIDLobby);
  return SteamMatchmaking()->LeaveLobby(lobbyID);
}
// Invite another user to the lobby, the target user will receive a
// LobbyInvite_t callback, will return true if the invite is successfully sent,
// whether or not the target responds
bool GodotSteamMatchmaking::inviteUserToLobby(int steamIDLobby, int steamIDInvitee) {
  if (!isSteamMatchmakingReady()) {
    return 0;
  }
  CSteamID lobbyID = GodotSteamUtils::get_singleton()->createSteamID(steamIDLobby);
  CSteamID inviteeID = GodotSteamUtils::get_singleton()->createSteamID(steamIDInvitee);
  return SteamMatchmaking()->InviteUserToLobby(lobbyID, inviteeID);
}

// Signal the lobby has been created
void GodotSteamMatchmaking::_lobby_created(LobbyCreated_t *lobbyData) {
  int connect = (int)lobbyData->m_eResult;

  int lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;
  emit_signal("lobby_created", connect, lobbyID);
}
// Signal that lobby has been joined
void GodotSteamMatchmaking::_lobby_joined(LobbyEnter_t *lobbyData) {
  int lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;
  uint32 permissions = lobbyData->m_rgfChatPermissions;
  bool locked = lobbyData->m_bLocked;
  uint32 response = lobbyData->m_EChatRoomEnterResponse;
  emit_signal("lobby_joined", lobbyID, permissions, locked, response);
}
// Signal that a lobby invite was sent
void GodotSteamMatchmaking::_lobby_invite(LobbyInvite_t *lobbyData) {
  int inviterID = (uint64)lobbyData->m_ulSteamIDUser;
  int lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;
  int gameID = (uint64)lobbyData->m_ulGameID;
  emit_signal("lobby_invite", inviterID, lobbyID, gameID);
}
// Signal a game/lobby join has been requested
void GodotSteamMatchmaking::_join_requested(GameRichPresenceJoinRequested_t *callData) {
  int steamID = callData->m_steamIDFriend.GetAccountID();
  String con_string = callData->m_rgchConnect;
  emit_signal("join_requested", steamID, con_string);
}

// When connected to a server
void GodotSteamMatchmaking::_server_connected(SteamServersConnected_t *conData) {
  emit_signal("connection_changed", true);
}
// When disconnected from a server
void GodotSteamMatchmaking::_server_disconnected(SteamServersDisconnected_t *conData) {
  emit_signal("connection_changed", false);
}

// Posted after the user gains ownership of DLC & that DLC is installed
void GodotSteamMatchmaking::_dlc_installed(DlcInstalled_t *callData) {
  int appID = (AppId_t)callData->m_nAppID;
  emit_signal("dlc_installed", appID);
}

void GodotSteamMatchmaking::_bind_methods() {
  ObjectTypeDB::bind_method(_MD("createLobby", "type"),
                            &GodotSteamMatchmaking::createLobby, DEFVAL(2));
  ObjectTypeDB::bind_method("joinLobby", &GodotSteamMatchmaking::joinLobby);
  ObjectTypeDB::bind_method("leaveLobby", &GodotSteamMatchmaking::leaveLobby);
  ObjectTypeDB::bind_method("inviteUserToLobby",
                            &GodotSteamMatchmaking::inviteUserToLobby);

  ADD_SIGNAL(MethodInfo("join_requested", PropertyInfo(Variant::INT, "from"),
                        PropertyInfo(Variant::STRING, "connect_string")));
  ADD_SIGNAL(MethodInfo("lobby_created", PropertyInfo(Variant::INT, "lobby")));
  ADD_SIGNAL(MethodInfo("lobby_joined", PropertyInfo(Variant::INT, "lobby"),
                        PropertyInfo(Variant::INT, "permissions"),
                        PropertyInfo(Variant::BOOL, "locked"),
                        PropertyInfo(Variant::INT, "response")));
  ADD_SIGNAL(MethodInfo("lobby_invite", PropertyInfo(Variant::INT, "inviter"),
                        PropertyInfo(Variant::INT, "lobby"),
                        PropertyInfo(Variant::INT, "game")));
  ADD_SIGNAL(MethodInfo("connection_changed",
                        PropertyInfo(Variant::BOOL, "connected")));

  BIND_CONSTANT(LOBBY_TYPE_PRIVATE);
  BIND_CONSTANT(LOBBY_TYPE_FRIENDS);
  BIND_CONSTANT(LOBBY_TYPE_PUBLIC);
  BIND_CONSTANT(LOBBY_TYPE_INVISIBLE);

  BIND_CONSTANT(LOBBY_CONNECTION_OK);
  BIND_CONSTANT(LOBBY_CONNECTION_NO_CONNECTION);
  BIND_CONSTANT(LOBBY_CONNECTION_TIMEOUT);
  BIND_CONSTANT(LOBBY_CONNECTION_FAIL);
  BIND_CONSTANT(LOBBY_CONNECTION_ACCESS_DENIED);
  BIND_CONSTANT(LOBBY_CONNECTION_LIMIT_EXCEEDED);
}
