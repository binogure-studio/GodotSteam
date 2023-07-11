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

  enum {
    GAMEPADTEST_SINGLELINE = (int)k_EFloatingGamepadTextInputModeModeSingleLine,
    GAMEPADTEST_MULTILINES = (int)k_EFloatingGamepadTextInputModeModeMultipleLines,
    GAMEPADTEST_EMAIL = (int)k_EFloatingGamepadTextInputModeModeEmail,
    GAMEPADTEST_NUMERIC = (int)k_EFloatingGamepadTextInputModeModeNumeric
  };
  GodotSteamUtils();
  ~GodotSteamUtils();

  String getIPCountry();
  bool isOverlayEnabled();
  String getSteamUILanguage();
  uint64_t getAppID();
  uint64_t getSecondsSinceAppActive();
  void setOverlayNotificationPosition(uint64_t pos);
  uint64_t getCurrentBatteryPower();
  bool isSteamRunningInVR();
  bool isRunningOnSteamDeck();
  bool showFloatingGamepadTextInput(uint64_t keyboardMode, uint64_t inputPositionX, uint64_t inputPositionY, uint64_t inputWidth, uint64_t inputHeight);
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
