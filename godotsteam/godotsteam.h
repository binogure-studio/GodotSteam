#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/io/ip.h"
#include "core/io/ip_address.h"
#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/variant/dictionary.h"
#include "scene/resources/texture.h"

// Local dependencies
#include "godotsteam_friends.h"
#include "godotsteam_utils.h"

class GodotSteam : public Object
{
public:
  enum {
    STEAM_INIT_NO_CLIENT_ERROR,
    STEAM_INIT_API_NOT_INIT_ERROR,
    STEAM_INIT_NO_CONNECTION_ERROR,
    STEAM_INIT_OK
  };

  static GodotSteam *get_singleton();
  static void reset_singleton();
  GodotSteam();
  ~GodotSteam();

  CSteamID createSteamID(uint32 steamID, uint64_t accountType = -1);

  bool isSteamAppReady();
  bool restartAppIfNecessary(uint64_t value);
  uint64_t steamInit(uint32 appID, bool force);
  bool isSteamRunning();
  bool hasOtherApp(uint64_t value);
  uint64_t getDLCCount();
  bool isDLCInstalled(uint64_t value);
  void requestAppProofOfPurchaseKey(uint64_t value);
  bool isAppInstalled(uint64_t value);
  String getCurrentBetaName();
  String getCurrentGameLanguage();
  bool isVACBanned();
  uint64_t getEarliestPurchaseUnixTime(uint64_t value);
  bool isSubscribedFromFreeWeekend();
  void installDLC(uint64_t value);
  void uninstallDLC(uint64_t value);

protected:
  static void _bind_methods();
  static GodotSteam *singleton;

private:
  bool isInitSuccess;

  STEAM_CALLBACK(GodotSteam, _overlay_toggled, GameOverlayActivated_t, callbackOverlayToggled);
  STEAM_CALLBACK(GodotSteam, _low_power, LowBatteryPower_t, callbackLowPower);
  STEAM_CALLBACK(GodotSteam, _dlc_installed, DlcInstalled_t, callbackDLCInstalled);

  void run_callbacks() {
    SteamAPI_RunCallbacks();
  }

  GDCLASS(GodotSteam, Object);
};
#endif // GODOTSTEAM_H
