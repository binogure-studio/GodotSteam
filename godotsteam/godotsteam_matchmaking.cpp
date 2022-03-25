#include "godotsteam_matchmaking.h"

GodotSteamMatchmaking *GodotSteamMatchmaking::singleton = NULL;

GodotSteamMatchmaking::GodotSteamMatchmaking():
  callbackLobbyJoined(this, &GodotSteamMatchmaking::_lobby_joined),
  callbackLobbyInvite(this, &GodotSteamMatchmaking::_lobby_invite),
  callbackJoinRequested(this, &GodotSteamMatchmaking::_join_requested),
  callbackMessageReceived(this, &GodotSteamMatchmaking::_message_received),
  callbackLobbyChatUpdate(this, &GodotSteamMatchmaking::_lobby_chat_update)
{
  singleton = this;
}

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

uint64_t GodotSteamMatchmaking::getLobbyOwner(uint64_t steamIDLobby) {
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), -1);

  CSteamID lobbyID = GodotSteamUtils::get_singleton()->createSteamID(steamIDLobby);

  return (uint64_t) SteamMatchmaking()->GetLobbyOwner(lobbyID).ConvertToUint64();
}

void GodotSteamMatchmaking::setLobbyOwner(uint64_t steamIDLobby, uint64_t steamIDNewOwner) {
  STEAM_FAIL_COND(!isSteamMatchmakingReady());

  CSteamID lobbyID = GodotSteamUtils::get_singleton()->createSteamID(steamIDLobby);
  CSteamID newOwnerID = GodotSteamUtils::get_singleton()->createSteamID(steamIDNewOwner);

  SteamMatchmaking()->SetLobbyOwner(lobbyID, newOwnerID);
}

void GodotSteamMatchmaking::findLobbies(int distanceFilter) {
  STEAM_FAIL_COND(!isSteamMatchmakingReady());

  SteamMatchmaking()->AddRequestLobbyListDistanceFilter((ELobbyDistanceFilter)distanceFilter);
  SteamAPICall_t apiCall = SteamMatchmaking()->RequestLobbyList();
  callResultFindLobby.Set(apiCall, this, &GodotSteamMatchmaking::_lobbies_found);
}


Dictionary GodotSteamMatchmaking::getLobbyData(uint64_t steamIDLobby) {
  Dictionary result;
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), result);

  CSteamID lobbyID = GodotSteamUtils::get_singleton()->createSteamID(steamIDLobby);
  int dataCount = SteamMatchmaking()->GetLobbyDataCount(lobbyID);

  for (int index = 0; index < dataCount; ++index) {
    char key[k_nMaxLobbyKeyLength];
    char value[k_cubChatMetadataMax];

    if (SteamMatchmaking()->GetLobbyDataByIndex(lobbyID, index, key, k_nMaxLobbyKeyLength, value, k_cubChatMetadataMax)) {
      result[key] = value;
    }
  }

  return result;
}

void GodotSteamMatchmaking::createLobby(uint64_t lobbyType, uint64_t cMaxMembers) {
  STEAM_FAIL_COND(!isSteamMatchmakingReady());

  SteamAPICall_t apiCall = SteamMatchmaking()->CreateLobby((ELobbyType)lobbyType, cMaxMembers);
  callResultCreateLobby.Set(apiCall, this, &GodotSteamMatchmaking::_lobby_created);
}

// Join an existing lobby
void GodotSteamMatchmaking::joinLobby(uint64_t steamIDLobby) {
  STEAM_FAIL_COND(!isSteamMatchmakingReady());

  CSteamID lobbyID = GodotSteamUtils::get_singleton()->createSteamID(steamIDLobby);
  SteamMatchmaking()->JoinLobby(lobbyID);
}
// Leave a lobby, this will take effect immediately on the client side, other
// users will be notified by LobbyChatUpdate_t callback
void GodotSteamMatchmaking::leaveLobby(uint64_t steamIDLobby) {
  STEAM_FAIL_COND(!isSteamMatchmakingReady());

  CSteamID lobbyID = GodotSteamUtils::get_singleton()->createSteamID(steamIDLobby);
  return SteamMatchmaking()->LeaveLobby(lobbyID);
}
// Invite another user to the lobby, the target user will receive a
// LobbyInvite_t callback, will return true if the invite is successfully sent,
// whether or not the target responds
bool GodotSteamMatchmaking::inviteUserToLobby(uint64_t steamIDLobby, uint64_t steamIDInvitee) {
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), false);

  CSteamID lobbyID = GodotSteamUtils::get_singleton()->createSteamID(steamIDLobby);
  CSteamID inviteeID = GodotSteamUtils::get_singleton()->createSteamID(steamIDInvitee);
  return SteamMatchmaking()->InviteUserToLobby(lobbyID, inviteeID);
}

bool GodotSteamMatchmaking::sendLobbyChatMessage(uint64_t steamIDLobby, const String &message) {
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), false);

  CSteamID lobbyID = GodotSteamUtils::get_singleton()->createSteamID(steamIDLobby);

  // if steamIDLobby is text, size should be strlen( text ) + 1, to include the null terminator
  return SteamMatchmaking()->SendLobbyChatMsg(lobbyID, message.utf8().get_data(), message.length() + 1);
}

bool GodotSteamMatchmaking::setLobbyType(uint64_t steamIDLobby, uint64_t lobby_type) {
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), false);

  return SteamMatchmaking()->SetLobbyType((uint64)steamIDLobby, (ELobbyType)lobby_type);
}

// Sets whether or not a lobby is joinable - defaults to true for a new lobby.
bool GodotSteamMatchmaking::setLobbyJoinable(uint64_t steamIDLobby, bool joinable) {
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), false);

  return SteamMatchmaking()->SetLobbyJoinable((uint64)steamIDLobby, joinable);
}

// Signal the lobby has been created
void GodotSteamMatchmaking::_lobby_created(LobbyCreated_t *lobbyData, bool ioFailure) {
  uint64_t connect = (uint64_t)lobbyData->m_eResult;
  uint64_t lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;

  emit_signal("lobby_created", connect, lobbyID);
}
// Signal that lobby has been joined
void GodotSteamMatchmaking::_lobby_joined(LobbyEnter_t *lobbyData) {
  uint64_t lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;
  uint32 permissions = lobbyData->m_rgfChatPermissions;
  bool locked = lobbyData->m_bLocked;
  uint32 response = lobbyData->m_EChatRoomEnterResponse;

  emit_signal("lobby_joined", lobbyID, permissions, locked, response);
}
// Signal that a lobby invite was sent
void GodotSteamMatchmaking::_lobby_invite(LobbyInvite_t *lobbyData) {
  uint64_t inviterID = (uint64)lobbyData->m_ulSteamIDUser;
  uint64_t lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;
  uint64_t gameID = (uint64)lobbyData->m_ulGameID;

  emit_signal("lobby_invite", inviterID, lobbyID, gameID);
}
// Signal a game/lobby join has been requested
void GodotSteamMatchmaking::_join_requested(GameRichPresenceJoinRequested_t *callData) {
  uint64_t steamID = callData->m_steamIDFriend.GetAccountID();
  String con_string = callData->m_rgchConnect;

  emit_signal("join_requested", steamID, con_string);
}

void GodotSteamMatchmaking::_message_received(LobbyChatMsg_t* callData) {
  char buffer[4096];
  CSteamID lobbyID = callData->m_ulSteamIDLobby;
  CSteamID userID = callData->m_ulSteamIDUser;
  uint8 chatType = callData->m_eChatEntryType;

  int messageSize = SteamMatchmaking()->GetLobbyChatEntry(lobbyID, callData->m_iChatID, &userID, &buffer, 4096, NULL);

  if (messageSize > 0) {
    emit_signal("message_received", (uint64_t)lobbyID.ConvertToUint64(), (uint64_t)userID.ConvertToUint64(), chatType, String::utf8(buffer));
  } else {
    emit_signal("message_received", (uint64_t)lobbyID.ConvertToUint64(), (uint64_t)userID.ConvertToUint64(), chatType, String("Invalid message"));
  }
}

void GodotSteamMatchmaking::_lobby_chat_update(LobbyChatUpdate_t* callData){
  CSteamID lobbyID = callData->m_ulSteamIDLobby;
  CSteamID changedUserID = callData->m_ulSteamIDUserChanged;
  CSteamID makingChangeID = callData->m_ulSteamIDMakingChange;
  uint32 chatState = callData->m_rgfChatMemberStateChange;

  emit_signal("lobby_chat_update", (uint64_t)lobbyID.ConvertToUint64(), (uint64_t)changedUserID.ConvertToUint64(), (uint64_t)makingChangeID.ConvertToUint64(), chatState);
}

void GodotSteamMatchmaking::_lobbies_found(LobbyMatchList_t *callData, bool ioFailure) {
  Array lobbyList;

  for (uint32 index = 0; index < callData->m_nLobbiesMatching; index++) {
    CSteamID lobby_id = SteamMatchmaking()->GetLobbyByIndex(index);

    lobbyList.append((uint64_t) lobby_id.ConvertToUint64());
  } 

  emit_signal("lobby_match_list", lobbyList);
}

void GodotSteamMatchmaking::_bind_methods() {
  ClassDB::bind_method(D_METHOD("createLobby", "type"), &GodotSteamMatchmaking::createLobby, DEFVAL(2));
  ClassDB::bind_method("joinLobby", &GodotSteamMatchmaking::joinLobby);
  ClassDB::bind_method("leaveLobby", &GodotSteamMatchmaking::leaveLobby);
  ClassDB::bind_method("inviteUserToLobby", &GodotSteamMatchmaking::inviteUserToLobby);

  ClassDB::bind_method("findLobbies", &GodotSteamMatchmaking::findLobbies);
  ClassDB::bind_method("getLobbyData", &GodotSteamMatchmaking::getLobbyData);
  ClassDB::bind_method("getLobbyOwner", &GodotSteamMatchmaking::getLobbyOwner);
  ClassDB::bind_method("sendLobbyChatMessage", &GodotSteamMatchmaking::sendLobbyChatMessage);
  ClassDB::bind_method("setLobbyJoinable", &GodotSteamMatchmaking::setLobbyJoinable);
  ClassDB::bind_method("setLobbyOwner", &GodotSteamMatchmaking::setLobbyOwner);
  ClassDB::bind_method("setLobbyType", &GodotSteamMatchmaking::setLobbyType);

  ADD_SIGNAL(MethodInfo("join_requested",
    PropertyInfo(Variant::INT, "from"),
    PropertyInfo(Variant::STRING, "connect_string")
  ));

  ADD_SIGNAL(MethodInfo("lobby_match_list",
    PropertyInfo(Variant::ARRAY, "lobbyList")
  ));

  ADD_SIGNAL(MethodInfo("lobby_created",
    PropertyInfo(Variant::INT, "status"),
    PropertyInfo(Variant::INT, "lobby")
  ));

  ADD_SIGNAL(MethodInfo("lobby_joined",
    PropertyInfo(Variant::INT, "lobby"),
    PropertyInfo(Variant::INT, "permissions"),
    PropertyInfo(Variant::BOOL, "locked"),
    PropertyInfo(Variant::INT, "response")
  ));

  ADD_SIGNAL(MethodInfo("lobby_invite",
    PropertyInfo(Variant::INT, "inviter"),
    PropertyInfo(Variant::INT, "lobby"),
    PropertyInfo(Variant::INT, "game")
  ));

  ADD_SIGNAL(MethodInfo("message_received",
    PropertyInfo(Variant::INT, "lobby"),
    PropertyInfo(Variant::INT, "user"),
    PropertyInfo(Variant::INT, "type"),
    PropertyInfo(Variant::STRING, "message")
  ));

  ADD_SIGNAL(MethodInfo("lobby_chat_update",
    PropertyInfo(Variant::INT, "lobby"),
    PropertyInfo(Variant::INT, "changedUser"),
    PropertyInfo(Variant::INT, "makingChangeUser"),
    PropertyInfo(Variant::INT, "state")
  ));

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

  BIND_CONSTANT(LOBBY_FILTER_DISTANCE_CLOSE);
  BIND_CONSTANT(LOBBY_FILTER_DISTANCE_DEFAULT);
  BIND_CONSTANT(LOBBY_FILTER_DISTANCE_FAR);
  BIND_CONSTANT(LOBBY_FILTER_DISTANCE_WORLDWIDE);

  BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_ENTERED);
  BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_LEFT);
  BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_DISCONNECTED);
  BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_KICKED);
  BIND_CONSTANT(CHAT_MEMBER_STATE_CHANGE_BANNED);
}
