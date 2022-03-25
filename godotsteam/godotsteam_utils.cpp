#include "godotsteam_utils.h"

GodotSteamUtils *GodotSteamUtils::singleton = NULL;

GodotSteamUtils::GodotSteamUtils() { singleton = this; }
GodotSteamUtils::~GodotSteamUtils() { singleton = NULL; }

GodotSteamUtils *GodotSteamUtils::get_singleton() {
  if (GodotSteamUtils::singleton == NULL) {
    GodotSteamUtils::singleton = new GodotSteamUtils();
  }

  return GodotSteamUtils::singleton;
}

void GodotSteamUtils::reset_singleton() {
  delete GodotSteamUtils::singleton;

  GodotSteamUtils::singleton = NULL;
}

CSteamID GodotSteamUtils::createSteamID(uint32 steamID, uint64_t accountType) {
  CSteamID cSteamID;
  if (accountType < 0 || accountType >= k_EAccountTypeMax) {
    accountType = 1;
  }

  cSteamID.Set(steamID, EUniverse(k_EUniversePublic),
               EAccountType(accountType));

  return cSteamID;
}

String GodotSteamUtils::getIPCountry() { return SteamUtils()->GetIPCountry(); }

bool GodotSteamUtils::isOverlayEnabled() {
  return SteamUtils()->IsOverlayEnabled();
}

void GodotSteamUtils::setOverlayNotificationPosition(uint64_t pos) {
  if ((pos < 0) || (pos > 3) || (SteamUtils() == NULL)) {
    return;
  }
  SteamUtils()->SetOverlayNotificationPosition(ENotificationPosition(pos));
}

String GodotSteamUtils::getSteamUILanguage() {
  return SteamUtils()->GetSteamUILanguage();
}

uint64_t GodotSteamUtils::getAppID() {
  if (SteamUtils() == NULL) {
    return 0;
  }
  return SteamUtils()->GetAppID();
}

uint64_t GodotSteamUtils::getSecondsSinceAppActive() {
  if (SteamUtils() == NULL) {
    return 0;
  }
  return SteamUtils()->GetSecondsSinceAppActive();
}

uint64_t GodotSteamUtils::getCurrentBatteryPower() {
  if (SteamUtils() == NULL) {
    return 0;
  }
  return SteamUtils()->GetCurrentBatteryPower();
}

bool GodotSteamUtils::isSteamRunningInVR() {
  if (SteamUtils() == NULL) {
    return 0;
  }
  return SteamUtils()->IsSteamRunningInVR();
}

uint64_t GodotSteamUtils::getServerRealTime() {
  if (SteamUtils() == NULL) {
    return 0;
  }
  return SteamUtils()->GetServerRealTime();
}

bool GodotSteamUtils::isSteamInBigPictureMode() {
  if (SteamUtils() == NULL) {
    return false;
  }
  return SteamUtils()->IsSteamInBigPictureMode();
}

void GodotSteamUtils::startVRDashboard() {
  if (SteamUtils() == NULL) {
    return;
  }
  SteamUtils()->StartVRDashboard();
}

void GodotSteamUtils::_bind_methods() {
  ClassDB::bind_method("getIPCountry", &GodotSteamUtils::getIPCountry);
  ClassDB::bind_method("isOverlayEnabled",
                            &GodotSteamUtils::isOverlayEnabled);
  ClassDB::bind_method("getSteamUILanguage",
                            &GodotSteamUtils::getSteamUILanguage);
  ClassDB::bind_method("getAppID", &GodotSteamUtils::getAppID);
  ClassDB::bind_method("getSecondsSinceAppActive",
                            &GodotSteamUtils::getSecondsSinceAppActive);
  ClassDB::bind_method(D_METHOD("setOverlayNotificationPosition", "0-3"),
                            &GodotSteamUtils::setOverlayNotificationPosition);
  ClassDB::bind_method("getCurrentBatteryPower",
                            &GodotSteamUtils::getCurrentBatteryPower);
  ClassDB::bind_method("getServerRealTime",
                            &GodotSteamUtils::getServerRealTime);
  ClassDB::bind_method("isSteamRunningInVR",
                            &GodotSteamUtils::isSteamRunningInVR);
  ClassDB::bind_method("isSteamInBigPictureMode",
                            &GodotSteamUtils::isSteamInBigPictureMode);
  ClassDB::bind_method("startVRDashboard",
                            &GodotSteamUtils::startVRDashboard);
}