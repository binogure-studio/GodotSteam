#ifndef GODOTSTEAMFRIENDS_H
#define GODOTSTEAMFRIENDS_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/variant/dictionary.h"
#include "scene/resources/texture.h"

// Local dependencies
#include "godotsteam_utils.h"

class GodotSteamFriends : public Object
{
public:
  enum
  {
    AVATAR_SMALL = 0,
    AVATAR_MEDIUM = 1,
    AVATAR_LARGE = 2,

    NO_AVATAR = 0,
    LOADING_AVATAR = 1,

    PERSONA_CHANGE_NAME = (uint64_t) k_EPersonaChangeName,
    PERSONA_CHANGE_STATUS = (uint64_t) k_EPersonaChangeStatus,
    PERSONA_CHANGE_COME_ONLINE = (uint64_t) k_EPersonaChangeComeOnline,
    PERSONA_CHANGE_GONE_OFFLINE = (uint64_t) k_EPersonaChangeGoneOffline,
    PERSONA_CHANGE_GAME_PLAYED = (uint64_t) k_EPersonaChangeGamePlayed,
    PERSONA_CHANGE_GAME_SERVER = (uint64_t) k_EPersonaChangeGameServer,
    PERSONA_CHANGE_AVATAR = (uint64_t) k_EPersonaChangeAvatar,
    PERSONA_CHANGE_JOINED_SOURCE = (uint64_t) k_EPersonaChangeJoinedSource,
    PERSONA_CHANGE_LEFT_SOURCE = (uint64_t) k_EPersonaChangeLeftSource,
    PERSONA_CHANGE_RELATIONSHIP_CHANGED = (uint64_t) k_EPersonaChangeRelationshipChanged,
    PERSONA_CHANGE_NAME_FIRST_SET = (uint64_t) k_EPersonaChangeNameFirstSet,
    PERSONA_CHANGE_BROADCAST = (uint64_t) k_EPersonaChangeBroadcast,
    PERSONA_CHANGE_NICKNAME = (uint64_t) k_EPersonaChangeNickname,
    PERSONA_CHANGE_STEAM_LEVEL = (uint64_t) k_EPersonaChangeSteamLevel,
    PERSONA_CHANGE_RICH_PRESENCE = (uint64_t) k_EPersonaChangeRichPresence
  };

  static GodotSteamFriends *get_singleton();
  static void reset_singleton();

  GodotSteamFriends();
  ~GodotSteamFriends();

  bool isSteamFriendsReady();
  Image drawAvatar(int size, uint8 *buffer);
  uint64_t getFriendCount();
  String getPersonaName();
  String getFriendPersonaName(uint64_t steam_id);
  void setGameInfo(const String &s_key, const String &s_value);
  void clearGameInfo();
  void inviteFriend(uint64_t id, const String &conString);
  void setPlayedWith(uint64_t steam_id);
  Array getRecentPlayers();
  int getFriendAvatar(uint64_t size = AVATAR_MEDIUM, uint64_t steam_id = 0);
  Array getUserSteamGroups();
  Array getUserSteamFriends();
  void activateGameOverlay(const String &type);
  void activateGameOverlayToUser(const String &type, uint64_t steam_id);
  void activateGameOverlayToWebPage(const String &url);
  void activateGameOverlayToStore(uint64_t appid = 0, uint64_t overlayStoreFlag = 2);
  void activateGameOverlayInviteDialog(uint64_t steam_id);

protected:
  static void _bind_methods();
  static GodotSteamFriends *singleton;

private:
  STEAM_CALLBACK(GodotSteamFriends, _avatar_loaded, AvatarImageLoaded_t, callbackAvatarLoaded);
  STEAM_CALLBACK(GodotSteamFriends, _game_lobby_join_requested, GameLobbyJoinRequested_t, callbackGameLobbyJoinRequested);
  STEAM_CALLBACK(GodotSteamFriends, _persona_state_changed, PersonaStateChange_t, callbackPersonaStateChanged);

  GDCLASS(GodotSteamFriends, Object);
};

#endif // GODOTSTEAMFRIENDS_H
