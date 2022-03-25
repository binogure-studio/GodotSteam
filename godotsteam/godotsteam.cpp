#include "godotsteam.h"

GodotSteam *GodotSteam::singleton = NULL;

GodotSteam::GodotSteam():
  callbackOverlayToggled(this, &GodotSteam::_overlay_toggled),
  callbackLowPower(this, &GodotSteam::_low_power),
  callbackDLCInstalled(this, &GodotSteam::_dlc_installed)
{
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

bool GodotSteam::restartAppIfNecessary(uint64_t value) {
  return SteamAPI_RestartAppIfNecessary((AppId_t)value);
}

// Initialize Steamworks
uint64_t GodotSteam::steamInit(uint32 appID, bool force) {
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

bool GodotSteam::hasOtherApp(uint64_t value) {
  STEAM_FAIL_COND_V(!isSteamAppReady(), false);

  return SteamApps()->BIsSubscribedApp((AppId_t)value);
}

uint64_t GodotSteam::getDLCCount() {
  STEAM_FAIL_COND_V(!isSteamAppReady(), false);

  return SteamApps()->GetDLCCount();
}

bool GodotSteam::isDLCInstalled(uint64_t value) {
  STEAM_FAIL_COND_V(!isSteamAppReady(), false);

  return SteamApps()->BIsDlcInstalled(value);
}

void GodotSteam::requestAppProofOfPurchaseKey(uint64_t value) {
  STEAM_FAIL_COND(!isSteamAppReady());

  SteamApps()->RequestAppProofOfPurchaseKey(value);
}

bool GodotSteam::isAppInstalled(uint64_t value) {
  STEAM_FAIL_COND_V(!isSteamAppReady(), false);

  return SteamApps()->BIsAppInstalled((AppId_t)value);
}

String GodotSteam::getCurrentBetaName() {
  STEAM_FAIL_COND_V(!isSteamAppReady(), "default");
  String result = "default";
  char branchName[k_cchPublishedFileURLMax];

  if (SteamApps()->GetCurrentBetaName(branchName, k_cchPublishedFileURLMax)) {
    result = String(branchName);
  }

  return result;
}

String GodotSteam::getCurrentGameLanguage() {
  STEAM_FAIL_COND_V(!isSteamAppReady(), "None");

  return SteamApps()->GetCurrentGameLanguage();
}

bool GodotSteam::isVACBanned() {
  STEAM_FAIL_COND_V(!isSteamAppReady(), false);

  return SteamApps()->BIsVACBanned();
}

uint64_t GodotSteam::getEarliestPurchaseUnixTime(uint64_t value) {
  STEAM_FAIL_COND_V(!isSteamAppReady(), 0);

  return SteamApps()->GetEarliestPurchaseUnixTime((AppId_t)value);
}

bool GodotSteam::isSubscribedFromFreeWeekend() {
  STEAM_FAIL_COND_V(!isSteamAppReady(), false);

  return SteamApps()->BIsSubscribedFromFreeWeekend();
}

void GodotSteam::installDLC(uint64_t value) {
  STEAM_FAIL_COND(!isSteamAppReady());

  SteamApps()->InstallDLC((AppId_t)value);
}
void GodotSteam::uninstallDLC(uint64_t value) {
  STEAM_FAIL_COND(!isSteamAppReady());

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
  uint64_t appID = (AppId_t)callData->m_nAppID;
  emit_signal("dlc_installed", appID);
}

void GodotSteam::_bind_methods() {
  ClassDB::bind_method("restartAppIfNecessary",
                            &GodotSteam::restartAppIfNecessary);
  ClassDB::bind_method("steamInit", &GodotSteam::steamInit);
  ClassDB::bind_method("isSteamRunning", &GodotSteam::isSteamRunning);
  ClassDB::bind_method("run_callbacks", &GodotSteam::run_callbacks);

  ClassDB::bind_method("hasOtherApp", &GodotSteam::hasOtherApp);
  ClassDB::bind_method("getDLCCount", &GodotSteam::getDLCCount);
  ClassDB::bind_method("isDLCInstalled", &GodotSteam::isDLCInstalled);
  ClassDB::bind_method("requestAppProofOfPurchaseKey",
                            &GodotSteam::requestAppProofOfPurchaseKey);
  ClassDB::bind_method("isAppInstalled", &GodotSteam::isAppInstalled);
  ClassDB::bind_method("getCurrentGameLanguage",
                            &GodotSteam::getCurrentGameLanguage);
  ClassDB::bind_method("getCurrentBetaName",
                            &GodotSteam::getCurrentBetaName);
  ClassDB::bind_method("isVACBanned", &GodotSteam::isVACBanned);
  ClassDB::bind_method("getEarliestPurchaseUnixTime",
                            &GodotSteam::getEarliestPurchaseUnixTime);
  ClassDB::bind_method("isSubscribedFromFreeWeekend",
                            &GodotSteam::isSubscribedFromFreeWeekend);
  ClassDB::bind_method("installDLC", &GodotSteam::installDLC);
  ClassDB::bind_method("uninstallDLC", &GodotSteam::uninstallDLC);

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
