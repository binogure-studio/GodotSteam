#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/io/ip.h"
#include "core/io/ip_address.h"
#include "core/dictionary.h"
#include "core/object.h"
#include "core/reference.h"
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

  CSteamID createSteamID(uint32 steamID, int accountType = -1);

  bool isSteamAppReady();
  bool restartAppIfNecessary(int value);
  int steamInit(uint32 appID, bool force);
  bool isSteamRunning();
  bool hasOtherApp(int value);
  int getDLCCount();
  bool isDLCInstalled(int value);
  void requestAppProofOfPurchaseKey(int value);
  bool isAppInstalled(int value);
  String getCurrentBetaName();
  String getCurrentGameLanguage();
  bool isVACBanned();
  int getEarliestPurchaseUnixTime(int value);
  bool isSubscribedFromFreeWeekend();
  void installDLC(int value);
  void uninstallDLC(int value);

protected:
  static void _bind_methods();
  static GodotSteam *singleton;

private:
  bool isInitSuccess;

  STEAM_CALLBACK(GodotSteam, _overlay_toggled, GameOverlayActivated_t);
  STEAM_CALLBACK(GodotSteam, _low_power, LowBatteryPower_t);
  STEAM_CALLBACK(GodotSteam, _dlc_installed, DlcInstalled_t);

  void run_callbacks()
  {
    SteamAPI_RunCallbacks();
  }

  OBJ_TYPE(GodotSteam, Object);
  OBJ_CATEGORY("GodotSteam");
};
#endif // GODOTSTEAM_H
