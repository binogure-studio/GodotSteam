#ifndef GODOTSTEAMUSER_H
#define GODOTSTEAMUSER_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/io/ip.h"
#include "core/io/ip_address.h"
#include "dictionary.h"
#include "object.h"
#include "reference.h"
#include "scene/resources/texture.h"

// Local dependencies
#include "godotsteam_utils.h"

class GodotSteamUser : public Object
{
public:
  static GodotSteamUser *get_singleton();
  static void reset_singleton();

  GodotSteamUser();
  ~GodotSteamUser();

  bool isSteamUserReady();
  int getSteamID();
  bool loggedOn();
  int getPlayerSteamLevel();
  String getUserDataFolder();
  void advertiseGame(const String &server_ip, int port);
  int getGameBadgeLevel(int series, bool foil);

protected:
  static void _bind_methods();
  static GodotSteamUser *singleton;

private:
  OBJ_TYPE(GodotSteamUser, Object);
  OBJ_CATEGORY("GodotSteamUser");
};
#endif // GODOTSTEAMUSER_H
