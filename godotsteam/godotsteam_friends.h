#ifndef GODOTSTEAMFRIENDS_H
#define GODOTSTEAMFRIENDS_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "object.h"
#include "scene/resources/texture.h"
#include "reference.h"
#include "dictionary.h"

// Local dependencies
#include "godotsteam_utils.h"

class GodotSteamFriends : public Object
{
public:
  enum
  {
    AVATAR_SMALL = 0,
    AVATAR_MEDIUM,
    AVATAR_LARGE
  };

  static GodotSteamFriends *get_singleton();
  static void reset_singleton();

  GodotSteamFriends();
  ~GodotSteamFriends();

  bool isSteamFriendsReady();
  Image drawAvatar(int size, uint8 *buffer);
  int getFriendCount();
  String getPersonaName();
  String getFriendPersonaName(int steam_id);
  void setGameInfo(const String &s_key, const String &s_value);
  void clearGameInfo();
  void inviteFriend(int id, const String &conString);
  void setPlayedWith(int steam_id);
  Array getRecentPlayers();
  void getFriendAvatar(int size = AVATAR_MEDIUM);
  Array getUserSteamGroups();
  Array getUserSteamFriends();
  void activateGameOverlay(const String &type);
  void activateGameOverlayToUser(const String &type, int steam_id);
  void activateGameOverlayToWebPage(const String &url);
  void activateGameOverlayToStore(int appid = 0);
  void activateGameOverlayInviteDialog(int id);

protected:
  static void _bind_methods();
  static GodotSteamFriends *singleton;

private:
  STEAM_CALLBACK(GodotSteamFriends, _avatar_loaded, AvatarImageLoaded_t);

  OBJ_TYPE(GodotSteamFriends, Object);
  OBJ_CATEGORY("GodotSteamFriends");
};

#endif // GODOTSTEAMFRIENDS_H
