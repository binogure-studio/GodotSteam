#ifndef GODOTSTEAMUTILS_H
#define GODOTSTEAMUTILS_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "godotsteam_utils.h"
#include "object.h"

#define STEAM_FAIL_COND(m_cond) \
  {                             \
    if (m_cond) {               \
      return;                   \
    }                           \
  }

#define STEAM_FAIL_COND_V(m_cond, m_retval) \
  {                                         \
    if (m_cond) {                           \
      return m_retval;                      \
    }                                       \
  }

class GodotSteamUtils : public Object {
public:
  static GodotSteamUtils *get_singleton();
  static void reset_singleton();

  GodotSteamUtils();
  ~GodotSteamUtils();

  CSteamID createSteamID(uint32 steamID, int accountType = -1);
  String getIPCountry();
  bool isOverlayEnabled();
  String getSteamUILanguage();
  int getAppID();
  int getSecondsSinceAppActive();
  void setOverlayNotificationPosition(int pos);
  int getCurrentBatteryPower();
  bool isSteamRunningInVR();
  int getServerRealTime();
  bool isSteamInBigPictureMode();
  void startVRDashboard();

protected:
  static void _bind_methods();
  static GodotSteamUtils *singleton;

private:
  OBJ_TYPE(GodotSteamUtils, Object);
  OBJ_CATEGORY("GodotSteamUtils");
};

#endif // GODOTSTEAMUTILS_H
