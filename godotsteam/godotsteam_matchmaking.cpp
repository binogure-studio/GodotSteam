#include "godotsteam_matchmaking.h"

GodotSteamMatchmaking *GodotSteamMatchmaking::singleton = NULL;

GodotSteamMatchmaking::GodotSteamMatchmaking():
  callbackLobbyGameCreated(this, &GodotSteamMatchmaking::_lobby_game_created),
  callbackLobbyKicked(this, &GodotSteamMatchmaking::_lobby_kicked),
  callbackLobbyDataUpdated(this, &GodotSteamMatchmaking::_lobby_data_updated),
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

bool GodotSteamMatchmaking::setLobbyData(uint64_t steamIDLobby, const String &key, const String &value) {
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), false);

  CSteamID lobbyID = (uint64)steamIDLobby;

  return SteamMatchmaking()->SetLobbyData(lobbyID, key.utf8().get_data(), value.utf8().get_data());  
}

Array GodotSteamMatchmaking::getLobbyMemberList(uint64_t steamIDLobby) {
  Array result;
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), result);

  CSteamID lobbyID = (uint64)steamIDLobby;
  int numberOfMembers = SteamMatchmaking()->GetNumLobbyMembers(lobbyID);

  for (int index = 0; index < numberOfMembers; index++) {
    CSteamID userSteamID = SteamMatchmaking()->GetLobbyMemberByIndex(lobbyID, index);

    result.push_back((uint64_t)userSteamID.ConvertToUint64());
  }

  return result;
}

uint64_t GodotSteamMatchmaking::getLobbyOwner(uint64_t steamIDLobby) {
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), -1);

  CSteamID lobbyID = (uint64)steamIDLobby;

  return (uint64_t)SteamMatchmaking()->GetLobbyOwner(lobbyID).ConvertToUint64();
}

void GodotSteamMatchmaking::setLobbyOwner(uint64_t steamIDLobby, uint64_t steamIDNewOwner) {
  STEAM_FAIL_COND(!isSteamMatchmakingReady());

  CSteamID lobbyID = (uint64)steamIDLobby;
  CSteamID newOwnerID = (uint64)steamIDNewOwner;

  SteamMatchmaking()->SetLobbyOwner(lobbyID, newOwnerID);
}

bool GodotSteamMatchmaking::kickUserFromLobby(uint64_t steamIDLobby, uint64_t steamIDUser) {
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), false);

  CSteamID lobbyID = (uint64)steamIDLobby;
  ChatMessage chatMessage;

  chatMessage.type = MESSAGE_KICK;
  chatMessage.userID = steamIDUser;
  chatMessage.textLength = 0;

  // Initialize the chat message with null value
  memset(&chatMessage.text, '\0', sizeof(chatMessage.text));

  return SteamMatchmaking()->SendLobbyChatMsg(lobbyID, &chatMessage, sizeof(chatMessage));
}

void GodotSteamMatchmaking::setLobbyMemberData(uint64_t steamIDLobby, const String &key, const String &value) {
  STEAM_FAIL_COND(!isSteamMatchmakingReady());

  CSteamID lobbyID = (uint64)steamIDLobby;

  SteamMatchmaking()->SetLobbyMemberData(lobbyID, key.utf8().get_data(), value.utf8().get_data());
}

String GodotSteamMatchmaking::getLobbyMemberData(uint64_t steamIDLobby, uint64_t steamIDUser, const String &key) {
  String result;
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), result);

  CSteamID lobbyID = (uint64)steamIDLobby;
  CSteamID userID = (uint64)steamIDUser;

  return (String)SteamMatchmaking()->GetLobbyMemberData(lobbyID, userID, key.utf8().get_data());
}

void GodotSteamMatchmaking::addRequestLobbyListResultCountFilter(uint64_t numberOfResults) {
  STEAM_FAIL_COND(!isSteamMatchmakingReady());

  SteamMatchmaking()->AddRequestLobbyListResultCountFilter(numberOfResults);
}

void GodotSteamMatchmaking::addRequestLobbyListStringFilter(const String &key, const String &value, uint64_t comparison_type) {
  STEAM_FAIL_COND(!isSteamMatchmakingReady());

  SteamMatchmaking()->AddRequestLobbyListStringFilter(key.utf8().get_data(), value.utf8().get_data(), (ELobbyComparison)comparison_type);
}

void GodotSteamMatchmaking::addRequestLobbyListFilterSlotsAvailable(uint64_t slotsAvailable) {
  STEAM_FAIL_COND(!isSteamMatchmakingReady());

  SteamMatchmaking()->AddRequestLobbyListFilterSlotsAvailable(slotsAvailable);
}

void GodotSteamMatchmaking::addRequestLobbyListDistanceFilter(uint64_t distanceFilter) {
  STEAM_FAIL_COND(!isSteamMatchmakingReady());

  SteamMatchmaking()->AddRequestLobbyListDistanceFilter((ELobbyDistanceFilter)distanceFilter);
}

void GodotSteamMatchmaking::requestLobbyList() {
  STEAM_FAIL_COND(!isSteamMatchmakingReady());

  SteamAPICall_t apiCall = SteamMatchmaking()->RequestLobbyList();
  callResultFindLobby.Set(apiCall, this, &GodotSteamMatchmaking::_lobbies_found);
}

uint64_t GodotSteamMatchmaking::getLobbyMemberLimit(uint64_t steamIDLobby) {
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), false);

  CSteamID lobbyID = (uint64)steamIDLobby;
  return SteamMatchmaking()->GetLobbyMemberLimit(lobbyID);
}

bool GodotSteamMatchmaking::setLobbyMemberLimit(uint64_t steamIDLobby, uint64_t maxMembers) {
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), false);

  CSteamID lobbyID = (uint64)steamIDLobby;
  return SteamMatchmaking()->SetLobbyMemberLimit(lobbyID, maxMembers);
}

bool GodotSteamMatchmaking::deleteLobbyData(uint64_t steamIDLobby, const String &key) {

  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), false);

  CSteamID lobbyID = (uint64)steamIDLobby;
  return SteamMatchmaking()->DeleteLobbyData(lobbyID, key.utf8().get_data());
}

bool GodotSteamMatchmaking::requestLobbyData(uint64_t steamIDLobby) {
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), false);

  CSteamID lobbyID = (uint64)steamIDLobby;
  return SteamMatchmaking()->RequestLobbyData(lobbyID);
}

Dictionary GodotSteamMatchmaking::getLobbyData(uint64_t steamIDLobby) {
  Dictionary result;
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), result);

  CSteamID lobbyID = (uint64)steamIDLobby;
  int dataCount = SteamMatchmaking()->GetLobbyDataCount(lobbyID);

  for (int index = 0; index < dataCount; ++index) {
    char key[k_nMaxLobbyKeyLength];
    char value[k_cubChatMetadataMax];

    if (SteamMatchmaking()->GetLobbyDataByIndex(lobbyID, index, key, k_nMaxLobbyKeyLength, value, k_cubChatMetadataMax)) {
      result[key] = String::utf8(value);
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

  CSteamID lobbyID = (uint64)steamIDLobby;
  SteamAPICall_t apiCall = SteamMatchmaking()->JoinLobby(lobbyID);
  callResultJoinLobby.Set(apiCall, this, &GodotSteamMatchmaking::_lobby_joined);
}
// Leave a lobby, this will take effect immediately on the client side, other
// users will be notified by LobbyChatUpdate_t callback
void GodotSteamMatchmaking::leaveLobby(uint64_t steamIDLobby) {
  STEAM_FAIL_COND(!isSteamMatchmakingReady());

  CSteamID lobbyID = (uint64)steamIDLobby;
  SteamMatchmaking()->LeaveLobby(lobbyID);
}
// Invite another user to the lobby, the target user will receive a
// LobbyInvite_t callback, will return true if the invite is successfully sent,
// whether or not the target responds
bool GodotSteamMatchmaking::inviteUserToLobby(uint64_t steamIDLobby, uint64_t steamIDInvitee) {
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), false);

  CSteamID lobbyID = (uint64)steamIDLobby;
  CSteamID inviteeID = (uint64)steamIDInvitee;
  return SteamMatchmaking()->InviteUserToLobby(lobbyID, inviteeID);
}

bool GodotSteamMatchmaking::sendLobbyChatMessage(uint64_t steamIDLobby, const String &message) {
  STEAM_FAIL_COND_V(!isSteamMatchmakingReady(), false);

  CSteamID lobbyID = (uint64)steamIDLobby;
  ChatMessage chatMessage;

  chatMessage.type = MESSAGE_TEXT;
  chatMessage.userID = (uint64)GodotSteamUser::get_singleton()->getSteamID();

  uint32_t messageLength = MIN(strlen(message.utf8().get_data()), sizeof(chatMessage.text) - 1);

  memset(&chatMessage.text, '\0', sizeof(chatMessage.text));
  memcpy(&chatMessage.text, message.utf8().get_data(), messageLength);

  chatMessage.textLength = messageLength + 1;

  // if steamIDLobby is text, size should be strlen( text ) + 1, to include the null terminator
  return SteamMatchmaking()->SendLobbyChatMsg(lobbyID, &chatMessage, sizeof(ChatMessage));
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
void GodotSteamMatchmaking::_lobby_joined(LobbyEnter_t *lobbyData, bool ioFailure) {
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
  char buffer[STEAM_MAX_BUFFER_SIZE];
  CSteamID lobbyID = callData->m_ulSteamIDLobby;
  CSteamID userID = callData->m_ulSteamIDUser;
  uint8 chatType = callData->m_eChatEntryType;

  int messageSize = SteamMatchmaking()->GetLobbyChatEntry(lobbyID, callData->m_iChatID, &userID, &buffer, STEAM_MAX_BUFFER_SIZE, NULL);

  if (messageSize > 0) {
    ChatMessage *message = (ChatMessage *)&buffer;

    if (message->type == MESSAGE_TEXT) {
      emit_signal("message_received", (uint64_t)lobbyID.ConvertToUint64(), (uint64_t)userID.ConvertToUint64(), chatType, String::utf8((char *)message->text, message->textLength));
    } else {
      // We're about to kick a user from the lobby
      CSteamID userIDToKick = (uint64)message->userID;
      CSteamID currentUserID = (uint64)GodotSteamUser::get_singleton()->getSteamID();

      if (userIDToKick == currentUserID) {
        SteamMatchmaking()->LeaveLobby(lobbyID);
        emit_signal("lobby_kicked", (uint64_t)lobbyID.ConvertToUint64(), (uint64_t)userID.ConvertToUint64(), 0);
      }
    }
  } else {
    emit_signal("message_received", (uint64_t)lobbyID.ConvertToUint64(), (uint64_t)userID.ConvertToUint64(), chatType, String("Invalid message"));
  }
}

void GodotSteamMatchmaking::_lobby_game_created(LobbyGameCreated_t* callData) {

  CSteamID lobbyID = callData->m_ulSteamIDLobby;
  CSteamID serverID = callData->m_ulSteamIDGameServer;
  uint32_t IPAdress = callData->m_unIP;
  uint16_t port = callData->m_usPort;

  emit_signal("lobby_game_created", (uint64_t)lobbyID.ConvertToUint64(), (uint64_t)serverID.ConvertToUint64(), IPAdress, port);
}

void GodotSteamMatchmaking::_lobby_kicked(LobbyKicked_t* callData) {

  CSteamID lobbyID = callData->m_ulSteamIDLobby;
  CSteamID adminUserID = callData->m_ulSteamIDAdmin;
  uint8_t disconnected = callData->m_bKickedDueToDisconnect;

  emit_signal("lobby_kicked", (uint64_t)lobbyID.ConvertToUint64(), (uint64_t)adminUserID.ConvertToUint64(), disconnected);
}

void GodotSteamMatchmaking::_lobby_data_updated(LobbyDataUpdate_t* callData) {

  CSteamID lobbyID = callData->m_ulSteamIDLobby;
  CSteamID changedUserID = callData->m_ulSteamIDMember;
  uint8_t success = callData->m_bSuccess;

  emit_signal("lobby_data_updated", (uint64_t)lobbyID.ConvertToUint64(), (uint64_t)changedUserID.ConvertToUint64(), success);
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

  ClassDB::bind_method("addRequestLobbyListStringFilter", &GodotSteamMatchmaking::addRequestLobbyListStringFilter);
  ClassDB::bind_method("addRequestLobbyListResultCountFilter", &GodotSteamMatchmaking::addRequestLobbyListResultCountFilter);
  ClassDB::bind_method("addRequestLobbyListFilterSlotsAvailable", &GodotSteamMatchmaking::addRequestLobbyListFilterSlotsAvailable);
  ClassDB::bind_method("addRequestLobbyListDistanceFilter", &GodotSteamMatchmaking::addRequestLobbyListDistanceFilter);
  ClassDB::bind_method("requestLobbyList", &GodotSteamMatchmaking::requestLobbyList);

  ClassDB::bind_method("kickUserFromLobby", &GodotSteamMatchmaking::kickUserFromLobby);

  ClassDB::bind_method("getLobbyMemberLimit", &GodotSteamMatchmaking::getLobbyMemberLimit);
  ClassDB::bind_method("setLobbyMemberLimit", &GodotSteamMatchmaking::setLobbyMemberLimit);

  ClassDB::bind_method("getLobbyData", &GodotSteamMatchmaking::getLobbyData);
  ClassDB::bind_method("getLobbyMemberData", &GodotSteamMatchmaking::getLobbyMemberData);
  ClassDB::bind_method("getLobbyMemberList", &GodotSteamMatchmaking::getLobbyMemberList);
  ClassDB::bind_method("getLobbyOwner", &GodotSteamMatchmaking::getLobbyOwner);

  ClassDB::bind_method("deleteLobbyData", &GodotSteamMatchmaking::deleteLobbyData);
  ClassDB::bind_method("requestLobbyData", &GodotSteamMatchmaking::requestLobbyData);

  ClassDB::bind_method("sendLobbyChatMessage", &GodotSteamMatchmaking::sendLobbyChatMessage);
  ClassDB::bind_method("setLobbyData", &GodotSteamMatchmaking::setLobbyData);
  ClassDB::bind_method("setLobbyJoinable", &GodotSteamMatchmaking::setLobbyJoinable);
  ClassDB::bind_method("setLobbyMemberData", &GodotSteamMatchmaking::setLobbyMemberData);
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

  ADD_SIGNAL(MethodInfo("lobby_game_created",
    PropertyInfo(Variant::INT, "lobby"),
    PropertyInfo(Variant::INT, "serverID"),
    PropertyInfo(Variant::INT, "port")
  ));

  ADD_SIGNAL(MethodInfo("lobby_kicked",
    PropertyInfo(Variant::INT, "lobby"),
    PropertyInfo(Variant::INT, "admin"),
    PropertyInfo(Variant::INT, "disconnected")
  ));

  ADD_SIGNAL(MethodInfo("lobby_data_updated",
    PropertyInfo(Variant::INT, "lobby"),
    PropertyInfo(Variant::INT, "userID"),
    PropertyInfo(Variant::INT, "success")
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

  BIND_CONSTANT(LOBBY_JOIN_SUCCESS);
  BIND_CONSTANT(LOBBY_JOIN_DOESNT_EXIST);
  BIND_CONSTANT(LOBBY_JOIN_NOT_ALLOWED);
  BIND_CONSTANT(LOBBY_JOIN_FULL);
  BIND_CONSTANT(LOBBY_JOIN_ERROR);
  BIND_CONSTANT(LOBBY_JOIN_BANNED);
  BIND_CONSTANT(LOBBY_JOIN_LIMITED);
  BIND_CONSTANT(LOBBY_JOIN_CLAN_DISABLED);
  BIND_CONSTANT(LOBBY_JOIN_COMMUNITY_BAN);
  BIND_CONSTANT(LOBBY_JOIN_MEMBER_BLOCKED_YOU);
  BIND_CONSTANT(LOBBY_JOIN_YOU_BLOCKED_MEMBER);
  BIND_CONSTANT(LOBBY_JOIN_RATE_LIMITE_XCEEDED);

  BIND_CONSTANT(LOBBY_COMPARISON_EQUAL_TO_OR_LESS_THAN);
  BIND_CONSTANT(LOBBY_COMPARISON_LESS_THAN);
  BIND_CONSTANT(LOBBY_COMPARISON_EQUAL);
  BIND_CONSTANT(LOBBY_COMPARISON_GREATER_THAN);
  BIND_CONSTANT(LOBBY_COMPARISON_EQUAL_TO_OR_GREATER_THAN);
  BIND_CONSTANT(LOBBY_COMPARISON_NOT_EQUAL);
}
