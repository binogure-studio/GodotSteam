#include "godotsteam.h"

GodotSteam *GodotSteam::singleton = NULL;

GodotSteam::GodotSteam() {
  isInitSuccess = false;
  singleton = this;
}

GodotSteam::~GodotSteam() { singleton = NULL; }

GodotSteam *GodotSteam::get_singleton() {
  if (GodotSteam::singleton == NULL) {
    GodotSteam::singleton = new GodotSteam();
  }

  return GodotSteam::singleton;
}

void GodotSteam::reset_singleton() {
  delete GodotSteam::singleton;

  GodotSteam::singleton = NULL;
}

bool GodotSteam::isSteamAppReady() { return SteamApps() != NULL; }
bool GodotSteam::isSteamRunning(void) { return SteamAPI_IsSteamRunning(); }

bool GodotSteam::restartAppIfNecessary(int value) {
  return SteamAPI_RestartAppIfNecessary((AppId_t)value);
}

// Initialize Steamworks
int GodotSteam::steamInit(uint32 appID, bool force) {
  if (!force && restartAppIfNecessary(appID)) {
    return STEAM_INIT_NO_CLIENT_ERROR;
  }

  if (!SteamAPI_Init()) {
    return STEAM_INIT_API_NOT_INIT_ERROR;
  }

  if (!SteamUser()->BLoggedOn()) {
    return STEAM_INIT_NO_CONNECTION_ERROR;
  }

  if (SteamUserStats() != NULL) {
    // Load stats and achievements automatically
    SteamUserStats()->RequestCurrentStats();
  }

  return STEAM_INIT_OK;
}

bool GodotSteam::hasOtherApp(int value) {
  if (!isSteamAppReady()) {
    return false;
  }
  return SteamApps()->BIsSubscribedApp((AppId_t)value);
}

int GodotSteam::getDLCCount() {
  if (!isSteamAppReady()) {
    return false;
  }
  return SteamApps()->GetDLCCount();
}

bool GodotSteam::isDLCInstalled(int value) {
  if (!isSteamAppReady()) {
    return false;
  }
  return SteamApps()->BIsDlcInstalled(value);
}

void GodotSteam::requestAppProofOfPurchaseKey(int value) {
  if (!isSteamAppReady()) {
    return;
  }
  SteamApps()->RequestAppProofOfPurchaseKey(value);
}

bool GodotSteam::isAppInstalled(int value) {
  if (!isSteamAppReady()) {
    return false;
  }
  return SteamApps()->BIsAppInstalled((AppId_t)value);
}

String GodotSteam::getCurrentGameLanguage() {
  if (!isSteamAppReady()) {
    return "None";
  }
  return SteamApps()->GetCurrentGameLanguage();
}

bool GodotSteam::isVACBanned() {
  if (!isSteamAppReady()) {
    return false;
  }
  return SteamApps()->BIsVACBanned();
}

int GodotSteam::getEarliestPurchaseUnixTime(int value) {
  if (!isSteamAppReady()) {
    return 0;
  }
  return SteamApps()->GetEarliestPurchaseUnixTime((AppId_t)value);
}

bool GodotSteam::isSubscribedFromFreeWeekend() {
  if (!isSteamAppReady()) {
    return false;
  }
  return SteamApps()->BIsSubscribedFromFreeWeekend();
}

void GodotSteam::installDLC(int value) {
  if (!isSteamAppReady()) {
    return;
  }
  SteamApps()->InstallDLC((AppId_t)value);
}
void GodotSteam::uninstallDLC(int value) {
  if (!isSteamAppReady()) {
    return;
  }
  SteamApps()->UninstallDLC((AppId_t)value);
}

void GodotSteam::_overlay_toggled(GameOverlayActivated_t *callData) {
  if (callData->m_bActive) {
    emit_signal("overlay_toggled", true);
  } else {
    emit_signal("overlay_toggled", false);
  }
}

void GodotSteam::_low_power(LowBatteryPower_t *timeLeft) {
  uint8 power = timeLeft->m_nMinutesBatteryLeft;
  emit_signal("low_power", power);
}

void GodotSteam::_dlc_installed(DlcInstalled_t *callData) {
  int appID = (AppId_t)callData->m_nAppID;
  emit_signal("dlc_installed", appID);
}

void GodotSteam::_bind_methods() {
  ObjectTypeDB::bind_method("restartAppIfNecessary",
                            &GodotSteam::restartAppIfNecessary);
  ObjectTypeDB::bind_method("steamInit", &GodotSteam::steamInit);
  ObjectTypeDB::bind_method("isSteamRunning", &GodotSteam::isSteamRunning);
  ObjectTypeDB::bind_method("run_callbacks", &GodotSteam::run_callbacks);

  ObjectTypeDB::bind_method("hasOtherApp", &GodotSteam::hasOtherApp);
  ObjectTypeDB::bind_method("getDLCCount", &GodotSteam::getDLCCount);
  ObjectTypeDB::bind_method("isDLCInstalled", &GodotSteam::isDLCInstalled);
  ObjectTypeDB::bind_method("requestAppProofOfPurchaseKey",
                            &GodotSteam::requestAppProofOfPurchaseKey);
  ObjectTypeDB::bind_method("isAppInstalled", &GodotSteam::isAppInstalled);
  ObjectTypeDB::bind_method("getCurrentGameLanguage",
                            &GodotSteam::getCurrentGameLanguage);
  ObjectTypeDB::bind_method("isVACBanned", &GodotSteam::isVACBanned);
  ObjectTypeDB::bind_method("getEarliestPurchaseUnixTime",
                            &GodotSteam::getEarliestPurchaseUnixTime);
  ObjectTypeDB::bind_method("isSubscribedFromFreeWeekend",
                            &GodotSteam::isSubscribedFromFreeWeekend);
  ObjectTypeDB::bind_method("installDLC", &GodotSteam::installDLC);
  ObjectTypeDB::bind_method("uninstallDLC", &GodotSteam::uninstallDLC);

  // Signals //////////////////////////////////
  ADD_SIGNAL(
      MethodInfo("overlay_toggled", PropertyInfo(Variant::BOOL, "active")));
  ADD_SIGNAL(MethodInfo("low_power", PropertyInfo(Variant::INT, "power")));

  ADD_SIGNAL(MethodInfo("dlc_installed", PropertyInfo(Variant::INT, "app")));

  BIND_CONSTANT(STEAM_INIT_NO_CLIENT_ERROR);
  BIND_CONSTANT(STEAM_INIT_API_NOT_INIT_ERROR);
  BIND_CONSTANT(STEAM_INIT_NO_CONNECTION_ERROR);
  BIND_CONSTANT(STEAM_INIT_OK);
}
