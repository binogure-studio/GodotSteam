#ifndef GODOTSTEAMUSER_H
#define GODOTSTEAMUSER_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/io/ip.h"
#include "core/io/ip_address.h"
#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/variant/dictionary.h"
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
  uint64_t getSteamID();
  bool loggedOn();
  uint64_t getPlayerSteamLevel();
  String getUserDataFolder();
  void advertiseGame(const String &server_ip, uint64_t port);
  uint64_t getGameBadgeLevel(uint64_t series, bool foil);

protected:
  static void _bind_methods();
  static GodotSteamUser *singleton;

private:
  GDCLASS(GodotSteamUser, Object);
};
#endif // GODOTSTEAMUSER_H
