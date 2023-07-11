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
uint64_t GodotSteam::steamInit(uint64_t appID, bool force) {
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

  ADD_SIGNAL(
      MethodInfo("overlay_toggled", PropertyInfo(Variant::BOOL, "active")));
  ADD_SIGNAL(MethodInfo("low_power", PropertyInfo(Variant::INT, "power")));

  ADD_SIGNAL(MethodInfo("dlc_installed", PropertyInfo(Variant::INT, "app")));

  BIND_CONSTANT(STEAM_INIT_NO_CLIENT_ERROR);
  BIND_CONSTANT(STEAM_INIT_API_NOT_INIT_ERROR);
  BIND_CONSTANT(STEAM_INIT_NO_CONNECTION_ERROR);
  BIND_CONSTANT(STEAM_INIT_OK);

  // Steam specific constants
  BIND_CONSTANT(STEAM_RESULT_NONE);
  BIND_CONSTANT(STEAM_RESULT_OK);
  BIND_CONSTANT(STEAM_RESULT_FAIL);
  BIND_CONSTANT(STEAM_RESULT_NO_CONNECTION);
  BIND_CONSTANT(STEAM_RESULT_NO_CONNECTION_RETRY);
  BIND_CONSTANT(STEAM_RESULT_INVALID_PASSWORD);
  BIND_CONSTANT(STEAM_RESULT_LOGGED_IN_ELSE_WHERE);
  BIND_CONSTANT(STEAM_RESULT_INVALID_PROTOCOL_VER);
  BIND_CONSTANT(STEAM_RESULT_INVALID_PARAM);
  BIND_CONSTANT(STEAM_RESULT_FILE_NOT_FOUND);
  BIND_CONSTANT(STEAM_RESULT_BUSY);
  BIND_CONSTANT(STEAM_RESULT_INVALID_STATE);
  BIND_CONSTANT(STEAM_RESULT_INVALID_NAME);
  BIND_CONSTANT(STEAM_RESULT_INVALID_EMAIL);
  BIND_CONSTANT(STEAM_RESULT_DUPLICATEN_AME);
  BIND_CONSTANT(STEAM_RESULT_ACCESS_DENIED);
  BIND_CONSTANT(STEAM_RESULT_TIMEOUT);
  BIND_CONSTANT(STEAM_RESULT_BANNED);
  BIND_CONSTANT(STEAM_RESULT_ACCOUNT_NOT_FOUND);
  BIND_CONSTANT(STEAM_RESULT_INVALID_STEAM_ID);
  BIND_CONSTANT(STEAM_RESULT_SERVICE_UNAVAILABLE);
  BIND_CONSTANT(STEAM_RESULT_NOT_LOGGED_ON);
  BIND_CONSTANT(STEAM_RESULT_PENDING);
  BIND_CONSTANT(STEAM_RESULT_ENCRYPTION_FAILURE);
  BIND_CONSTANT(STEAM_RESULT_INSUFFICIENT_PRIVILEGE);
  BIND_CONSTANT(STEAM_RESULT_LIMIT_EXCEEDED);
  BIND_CONSTANT(STEAM_RESULT_REVOKED);
  BIND_CONSTANT(STEAM_RESULT_EXPIRED);
  BIND_CONSTANT(STEAM_RESULT_ALREADY_REDEEMED);
  BIND_CONSTANT(STEAM_RESULT_DUPLICATE_REQUEST);
  BIND_CONSTANT(STEAM_RESULT_ALREADY_OWNED);
  BIND_CONSTANT(STEAM_RESULT_IP_NOT_FOUND);
  BIND_CONSTANT(STEAM_RESULT_PERSIST_FAILED);
  BIND_CONSTANT(STEAM_RESULT_LOCKING_FAILED);
  BIND_CONSTANT(STEAM_RESULT_LOGON_SESSION_REPLACED);
  BIND_CONSTANT(STEAM_RESULT_CONNECT_FAILED);
  BIND_CONSTANT(STEAM_RESULT_HANDSHAKE_FAILED);
  BIND_CONSTANT(STEAM_RESULT_IO_FAILURE);
  BIND_CONSTANT(STEAM_RESULT_REMOTE_DISCONNECT);
  BIND_CONSTANT(STEAM_RESULT_SHOPPING_CART_NOT_FOUND);
  BIND_CONSTANT(STEAM_RESULT_BLOCKED);
  BIND_CONSTANT(STEAM_RESULT_IGNORED);
  BIND_CONSTANT(STEAM_RESULT_NO_MATCH);
  BIND_CONSTANT(STEAM_RESULT_ACCOUNT_DISABLED);
  BIND_CONSTANT(STEAM_RESULT_SERVICE_READ_ONLY);
  BIND_CONSTANT(STEAM_RESULT_ACCOUNT_NOT_FEATURED);
  BIND_CONSTANT(STEAM_RESULT_ADMINISTRATOR_OK);
  BIND_CONSTANT(STEAM_RESULT_CONTENT_VERSION);
  BIND_CONSTANT(STEAM_RESULT_TRY_ANOTHER_CM);
  BIND_CONSTANT(STEAM_RESULT_PASSWORD_REQUIRED_TO_KICK_SESSION);
  BIND_CONSTANT(STEAM_RESULT_ALREADY_LOGGED_IN_ELSE_WHERE);
  BIND_CONSTANT(STEAM_RESULT_SUSPENDED);
  BIND_CONSTANT(STEAM_RESULT_CANCELLED);
  BIND_CONSTANT(STEAM_RESULT_DATA_CORRUPTION);
  BIND_CONSTANT(STEAM_RESULT_DISK_FULL);
  BIND_CONSTANT(STEAM_RESULT_REMOTE_CALL_FAILED);
  BIND_CONSTANT(STEAM_RESULT_PASSWORD_UNSET);
  BIND_CONSTANT(STEAM_RESULT_EXTERNAL_ACCOUNT_UNLINKED);
  BIND_CONSTANT(STEAM_RESULT_PSN_TICKET_INVALID);
  BIND_CONSTANT(STEAM_RESULT_EXTERNAL_ACCOUNT_ALREADY_LINKED);
  BIND_CONSTANT(STEAM_RESULT_REMOTE_FILE_CONFLICT);
  BIND_CONSTANT(STEAM_RESULT_ILLEGAL_PASSWORD);
  BIND_CONSTANT(STEAM_RESULT_SAME_AS_PREVIOUS_VALUE);
  BIND_CONSTANT(STEAM_RESULT_ACCOUNT_LOG_ON_DENIED);
  BIND_CONSTANT(STEAM_RESULT_CANNOT_USE_OLD_PASSWORD);
  BIND_CONSTANT(STEAM_RESULT_INVALID_LOGIN_AUTH_CODE);
  BIND_CONSTANT(STEAM_RESULT_ACCOUNT_LOG_ON_DENIED_NO_MAIL);
  BIND_CONSTANT(STEAM_RESULT_HARDWARE_NOT_CAPABLE_OF_IPT);
  BIND_CONSTANT(STEAM_RESULT_IPT_INIT_ERROR);
  BIND_CONSTANT(STEAM_RESULT_PARENTAL_CONTROL_RESTRICTED);
  BIND_CONSTANT(STEAM_RESULT_FACEBOOK_QUERY_ERROR);
  BIND_CONSTANT(STEAM_RESULT_EXPIRED_LOGIN_AUTH_CODE);
  BIND_CONSTANT(STEAM_RESULT_IP_LOGIN_RESTRICTION_FAILED);
  BIND_CONSTANT(STEAM_RESULT_ACCOUNT_LOCKED_DOWN);
  BIND_CONSTANT(STEAM_RESULT_ACCOUNT_LOGON_DENIED_VERIFIED_EMAIL_REQUIRED);
  BIND_CONSTANT(STEAM_RESULT_NO_MATCHING_URL);
  BIND_CONSTANT(STEAM_RESULT_BAD_RESPONSE);
  BIND_CONSTANT(STEAM_RESULT_REQUIRE_PASSWORD_REENTRY);
  BIND_CONSTANT(STEAM_RESULT_VALUE_OUT_OF_RANGE);
  BIND_CONSTANT(STEAM_RESULT_UNEXPECTED_ERROR);
  BIND_CONSTANT(STEAM_RESULT_DISABLED);
  BIND_CONSTANT(STEAM_RESULT_INVALID_CEG_SUBMISSION);
  BIND_CONSTANT(STEAM_RESULT_RESTRICTED_DEVICE);
  BIND_CONSTANT(STEAM_RESULT_REGION_LOCKED);
  BIND_CONSTANT(STEAM_RESULT_RATE_LIMIT_EXCEEDED);
  BIND_CONSTANT(STEAM_RESULT_ACCOUNT_LOGIN_DENIED_NEED_TWO_FACTOR);
  BIND_CONSTANT(STEAM_RESULT_ITEM_DELETED);
  BIND_CONSTANT(STEAM_RESULT_ACCOUNT_LOGIN_DENIED_THROTTLE);
  BIND_CONSTANT(STEAM_RESULT_TWO_FACTOR_CODE_MISMATCH);
  BIND_CONSTANT(STEAM_RESULT_TWO_FACTOR_ACTIVATION_CODE_MISMATCH);
  BIND_CONSTANT(STEAM_RESULT_ACCOUNT_ASSOCIATED_TO_MULTIPLE_PARTNERS);
  BIND_CONSTANT(STEAM_RESULT_NOT_MODIFIED);
  BIND_CONSTANT(STEAM_RESULT_NO_MOBILE_DEVICE);
  BIND_CONSTANT(STEAM_RESULT_TIME_NOT_SYNCED);
  BIND_CONSTANT(STEAM_RESULT_SMS_CODE_FAILED);
  BIND_CONSTANT(STEAM_RESULT_ACCOUNT_LIMIT_EXCEEDED);
  BIND_CONSTANT(STEAM_RESULT_ACCOUNT_ACTIVITY_LIMIT_EXCEEDED);
  BIND_CONSTANT(STEAM_RESULT_PHONE_ACTIVITY_LIMIT_EXCEEDED);
  BIND_CONSTANT(STEAM_RESULT_REFUND_TO_WALLET);
  BIND_CONSTANT(STEAM_RESULT_EMAIL_SEND_FAILURE);
  BIND_CONSTANT(STEAM_RESULT_NOT_SETTLED);
  BIND_CONSTANT(STEAM_RESULT_NEED_CAPTCHA);
  BIND_CONSTANT(STEAM_RESULT_GSLT_DENIED);
  BIND_CONSTANT(STEAM_RESULT_GS_OWNER_DENIED);
  BIND_CONSTANT(STEAM_RESULT_INVALID_ITEM_TYPE);
  BIND_CONSTANT(STEAM_RESULT_IP_BANNED);
  BIND_CONSTANT(STEAM_RESULT_GSLT_EXPIRED);
  BIND_CONSTANT(STEAM_RESULT_INSUFFICIENT_FUNDS);
  BIND_CONSTANT(STEAM_RESULT_TOO_MANY_PENDING);
  BIND_CONSTANT(STEAM_RESULT_NO_SITE_LICENSES_FOUND);
  BIND_CONSTANT(STEAM_RESULT_WG_NETWORK_SEND_EXCEEDED);
  BIND_CONSTANT(STEAM_RESULT_ACCOUNT_NOT_FRIENDS);
  BIND_CONSTANT(STEAM_RESULT_LIMITED_USER_ACCOUNT);
  BIND_CONSTANT(STEAM_RESULT_CANT_REMOVE_ITEM);
  BIND_CONSTANT(STEAM_RESULT_ACCOUNT_DELETED);
  BIND_CONSTANT(STEAM_RESULT_EXISTING_USER_CANCELLED_LICENSE);
  BIND_CONSTANT(STEAM_RESULT_COMMUNITY_COOLDOWN);
  BIND_CONSTANT(STEAM_RESULT_NO_LAUNCHER_SPECIFIED);
  BIND_CONSTANT(STEAM_RESULT_MUST_AGREE_TO_SSA);
  BIND_CONSTANT(STEAM_RESULT_LAUNCHER_MIGRATED);
  BIND_CONSTANT(STEAM_RESULT_STEAM_REAL_MMISMATCH);
  BIND_CONSTANT(STEAM_RESULT_INVALID_SIGNATURE);
  BIND_CONSTANT(STEAM_RESULT_PARSE_FAILURE);
  BIND_CONSTANT(STEAM_RESULT_NO_VERIFIED_PHONE);
  BIND_CONSTANT(STEAM_RESULT_IN_SUFFICIENT_BATTERY);
  BIND_CONSTANT(STEAM_RESULT_CHARGER_REQUIRED);
  BIND_CONSTANT(STEAM_RESULT_CACHED_CREDENTIAL_INVALID);
  BIND_CONSTANT(STEAM_RESULT_PHONE_NUMBER_IS_VOIP);
}
