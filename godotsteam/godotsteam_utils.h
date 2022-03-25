#ifndef GODOTSTEAMUTILS_H
#define GODOTSTEAMUTILS_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "godotsteam_utils.h"

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

  CSteamID createSteamID(uint32 steamID, uint64_t accountType = -1);
  String getIPCountry();
  bool isOverlayEnabled();
  String getSteamUILanguage();
  uint64_t getAppID();
  uint64_t getSecondsSinceAppActive();
  void setOverlayNotificationPosition(uint64_t pos);
  uint64_t getCurrentBatteryPower();
  bool isSteamRunningInVR();
  uint64_t getServerRealTime();
  bool isSteamInBigPictureMode();
  void startVRDashboard();

protected:
  static void _bind_methods();
  static GodotSteamUtils *singleton;

private:
  GDCLASS(GodotSteamUtils, Object);
};

#endif // GODOTSTEAMUTILS_H
