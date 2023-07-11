#include "godotsteam_friends.h"

GodotSteamFriends *GodotSteamFriends::singleton = NULL;

GodotSteamFriends::GodotSteamFriends():
  callbackAvatarLoaded(this, &GodotSteamFriends::_avatar_loaded),
  callbackGameLobbyJoinRequested(this, &GodotSteamFriends::_game_lobby_join_requested)
{
    singleton = this;
}

GodotSteamFriends::~GodotSteamFriends() { singleton = NULL; }

GodotSteamFriends *GodotSteamFriends::get_singleton() {
  if (GodotSteamFriends::singleton == NULL) {
    GodotSteamFriends::singleton = new GodotSteamFriends();
  }

  return GodotSteamFriends::singleton;
}

void GodotSteamFriends::reset_singleton() {
  delete GodotSteamFriends::singleton;

  GodotSteamFriends::singleton = NULL;
}

bool GodotSteamFriends::isSteamFriendsReady() { return SteamFriends() != NULL; }

uint64_t GodotSteamFriends::getFriendCount() {
  STEAM_FAIL_COND_V(!isSteamFriendsReady(), 0);

  return SteamFriends()->GetFriendCount(0x04);
}

String GodotSteamFriends::getPersonaName() {
  STEAM_FAIL_COND_V(!isSteamFriendsReady(), "");

  return SteamFriends()->GetPersonaName();
}

String GodotSteamFriends::getFriendPersonaName(uint64_t steamID) {
  String personaName = "";

  if (isSteamFriendsReady() && steamID > 0) {
    CSteamID friendID = (uint64)steamID;
    bool isDataLoading = SteamFriends()->RequestUserInformation(friendID, true);

    if (!isDataLoading) {
      personaName = SteamFriends()->GetFriendPersonaName(friendID);
    }
  }

  return personaName;
}

void GodotSteamFriends::setGameInfo(const String &s_key,
                                    const String &s_value) {
  // Rich presence data is automatically shared between friends in the same game
  // Each user has a set of key/value pairs, up to 20 can be set
  // Two magic keys (status, connect)
  // setGameInfo() to an empty string deletes the key
  STEAM_FAIL_COND(!isSteamFriendsReady());

  SteamFriends()->SetRichPresence(s_key.utf8().get_data(),
                                  s_value.utf8().get_data());
}

void GodotSteamFriends::clearGameInfo() {
  STEAM_FAIL_COND(!isSteamFriendsReady());

  SteamFriends()->ClearRichPresence();
}

void GodotSteamFriends::inviteFriend(uint64_t steamID, const String &conString) {
  STEAM_FAIL_COND(!isSteamFriendsReady());

  CSteamID friendID = (uint64)steamID;

  SteamFriends()->InviteUserToGame(friendID, conString.utf8().get_data());
}

void GodotSteamFriends::setPlayedWith(uint64_t steamID) {
  STEAM_FAIL_COND(!isSteamFriendsReady());

  CSteamID friendID = (uint64)steamID;
  SteamFriends()->SetPlayedWith(friendID);
}

Array GodotSteamFriends::getRecentPlayers() {
  STEAM_FAIL_COND_V(!isSteamFriendsReady(), Array());

  uint64_t rCount = SteamFriends()->GetCoplayFriendCount();
  Array recents;

  for (uint64_t index = 0; index < rCount; index++) {
    CSteamID rPlayerID = SteamFriends()->GetCoplayFriend(index);

    if (SteamFriends()->GetFriendCoplayGame(rPlayerID) ==
        SteamUtils()->GetAppID()) {
      Dictionary rPlayer;
      String rName = SteamFriends()->GetFriendPersonaName(rPlayerID);
      uint64_t rStatus = SteamFriends()->GetFriendPersonaState(rPlayerID);

      rPlayer["id"] = rPlayerID.GetAccountID();
      rPlayer["name"] = rName;
      rPlayer["status"] = rStatus;

      recents.append(rPlayer);
    }
  }

  return recents;
}

void GodotSteamFriends::getFriendAvatar(uint64_t size, uint64_t steam_id) {
  STEAM_FAIL_COND(size < AVATAR_SMALL || size > AVATAR_LARGE || !isSteamFriendsReady());

  CSteamID avatar_id = (steam_id == 0) ? SteamUser()->GetSteamID() : (uint64)steam_id;
  uint64_t iHandle = 0;

  switch (size) {
    case AVATAR_SMALL:
      iHandle = SteamFriends()->GetSmallFriendAvatar(avatar_id);
      size = 32;
      break;

    case AVATAR_MEDIUM:
      iHandle = SteamFriends()->GetMediumFriendAvatar(avatar_id);
      size = 64;
      break;

    case AVATAR_LARGE:
      iHandle = SteamFriends()->GetLargeFriendAvatar(avatar_id);
      size = 184;
      break;

    default:
      return;
  }

  if (iHandle <= 0) {
    return;
  }

  // Has already loaded, simulate callback
  AvatarImageLoaded_t *avatarData = new AvatarImageLoaded_t;
  avatarData->m_steamID = avatar_id;
  avatarData->m_iImage = iHandle;
  avatarData->m_iWide = size;
  avatarData->m_iTall = size;

  _avatar_loaded(avatarData);

  delete avatarData;

  return;
}

void GodotSteamFriends::_game_lobby_join_requested(GameLobbyJoinRequested_t *callData) {

  CSteamID lobbyID = callData->m_steamIDLobby;

  emit_signal("game_lobby_join_requested", (uint64_t) lobbyID.ConvertToUint64());
}

// Signal that the avatar has been loaded
void GodotSteamFriends::_avatar_loaded(AvatarImageLoaded_t *avatarData) {

  uint32 width, height;
  bool success = SteamUtils()->GetImageSize(avatarData->m_iImage, &width, &height);
  if(!success){
    printf("[Steam] Failed to get image size.\n");
    return;
  }
  PackedByteArray data;
  data.resize(width * height * 4);
  success = SteamUtils()->GetImageRGBA(avatarData->m_iImage, data.ptrw(), data.size());
  if(!success){
    printf("[Steam] Failed to load image buffer from callback\n");
    return;
  }

  call_deferred("emit_signal", "avatar_loaded", (uint64_t) avatarData->m_steamID.ConvertToUint64(), width, data);
}

void GodotSteamFriends::activateGameOverlay(const String &url) {
  STEAM_FAIL_COND(!isSteamFriendsReady());

  SteamFriends()->ActivateGameOverlay(url.utf8().get_data());
}

void GodotSteamFriends::activateGameOverlayToUser(const String &url, uint64_t steamID) {
  STEAM_FAIL_COND(!isSteamFriendsReady());

  CSteamID overlayUserID = (uint64)steamID;
  SteamFriends()->ActivateGameOverlayToUser(url.utf8().get_data(), overlayUserID);
}

void GodotSteamFriends::activateGameOverlayToWebPage(const String &url) {
  STEAM_FAIL_COND(!isSteamFriendsReady());

  SteamFriends()->ActivateGameOverlayToWebPage(url.utf8().get_data());
}

void GodotSteamFriends::activateGameOverlayToStore(AppId_t app_id) {
  STEAM_FAIL_COND(!isSteamFriendsReady());

  SteamFriends()->ActivateGameOverlayToStore(AppId_t(app_id),
                                             EOverlayToStoreFlag(0));
}

Array GodotSteamFriends::getUserSteamGroups() {
  STEAM_FAIL_COND_V(!isSteamFriendsReady(), Array());

  uint64_t groupCount = SteamFriends()->GetClanCount();
  Array steamGroups;

  for (uint64_t index = 0; index < groupCount; index++) {

    Dictionary groups;
    CSteamID groupID = SteamFriends()->GetClanByIndex(index);
    String gName = SteamFriends()->GetClanName(groupID);
    String gTag = SteamFriends()->GetClanTag(groupID);
    groups["id"] = groupID.GetAccountID();
    groups["name"] = gName;
    groups["tag"] = gTag;
    steamGroups.append(groups);
  }

  return steamGroups;
}

Array GodotSteamFriends::getUserSteamFriends() {
  STEAM_FAIL_COND_V(!isSteamFriendsReady(), Array());

  uint64_t fCount = SteamFriends()->GetFriendCount(0x04);
  Array steamFriends;

  for (uint64_t index = 0; index < fCount; index++) {
    Dictionary friends;
    CSteamID friendID = SteamFriends()->GetFriendByIndex(index, 0x04);
    String fName = SteamFriends()->GetFriendPersonaName(friendID);
    uint64_t fStatus = SteamFriends()->GetFriendPersonaState(friendID);
    friends["id"] = friendID.GetAccountID();
    friends["name"] = fName;
    friends["status"] = fStatus;
    steamFriends.append(friends);
  }

  return steamFriends;
}

void GodotSteamFriends::activateGameOverlayInviteDialog(uint64_t steamID) {
  STEAM_FAIL_COND(!isSteamFriendsReady());

  CSteamID lobbyID = (uint64)steamID;
  SteamFriends()->ActivateGameOverlayInviteDialog(lobbyID);
}

void GodotSteamFriends::_bind_methods() {
  ClassDB::bind_method("getFriendCount",
                            &GodotSteamFriends::getFriendCount);
  ClassDB::bind_method("getPersonaName",
                            &GodotSteamFriends::getPersonaName);
  ClassDB::bind_method("getFriendPersonaName",
                            &GodotSteamFriends::getFriendPersonaName);
  ClassDB::bind_method(D_METHOD("setGameInfo", "key", "value"),
                            &GodotSteamFriends::setGameInfo);
  ClassDB::bind_method(D_METHOD("clearGameInfo"),
                            &GodotSteamFriends::clearGameInfo);
  ClassDB::bind_method(D_METHOD("inviteFriend", "steam_id", "connect_string"),
                            &GodotSteamFriends::inviteFriend);
  ClassDB::bind_method(D_METHOD("setPlayedWith", "steam_id"),
                            &GodotSteamFriends::setPlayedWith);
  ClassDB::bind_method("getRecentPlayers",
                            &GodotSteamFriends::getRecentPlayers);
  ClassDB::bind_method(D_METHOD("getFriendAvatar", "size", "steam_id"),
                            &GodotSteamFriends::getFriendAvatar,
                            DEFVAL(AVATAR_MEDIUM),
                            DEFVAL(0));
  ClassDB::bind_method("getUserSteamGroups",
                            &GodotSteamFriends::getUserSteamGroups);
  ClassDB::bind_method("getUserSteamFriends",
                            &GodotSteamFriends::getUserSteamFriends);
  ClassDB::bind_method(D_METHOD("activateGameOverlay", "type"),
                            &GodotSteamFriends::activateGameOverlay,
                            DEFVAL(""));
  ClassDB::bind_method(
      D_METHOD("activateGameOverlayToUser", "type", "steam_id"),
      &GodotSteamFriends::activateGameOverlayToUser, DEFVAL(""));
  ClassDB::bind_method(D_METHOD("activateGameOverlayToWebPage", "url"),
                            &GodotSteamFriends::activateGameOverlayToWebPage);
  ClassDB::bind_method(D_METHOD("activateGameOverlayToStore", "appID"),
                            &GodotSteamFriends::activateGameOverlayToStore,
                            DEFVAL(0));
  ClassDB::bind_method(
      D_METHOD("activateGameOverlayInviteDialog", "steam_id"),
      &GodotSteamFriends::activateGameOverlayInviteDialog);

  ADD_SIGNAL(MethodInfo("avatar_loaded",
    PropertyInfo(Variant::INT, "size"),
    PropertyInfo(Variant::INT, "steamID"),
    PropertyInfo(Variant::ARRAY, "avatar")
  ));

  ADD_SIGNAL(MethodInfo("game_lobby_join_requested",
    PropertyInfo(Variant::INT, "lobby")
  ));

  BIND_CONSTANT(AVATAR_SMALL);
  BIND_CONSTANT(AVATAR_MEDIUM);
  BIND_CONSTANT(AVATAR_LARGE);
}
