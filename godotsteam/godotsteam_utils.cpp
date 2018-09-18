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

CSteamID GodotSteamUtils::createSteamID(uint32 steamID, int accountType) {
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

void GodotSteamUtils::setOverlayNotificationPosition(int pos) {
  if ((pos < 0) || (pos > 3) || (SteamUtils() == NULL)) {
    return;
  }
  SteamUtils()->SetOverlayNotificationPosition(ENotificationPosition(pos));
}

String GodotSteamUtils::getSteamUILanguage() {
  return SteamUtils()->GetSteamUILanguage();
}

int GodotSteamUtils::getAppID() {
  if (SteamUtils() == NULL) {
    return 0;
  }
  return SteamUtils()->GetAppID();
}

int GodotSteamUtils::getSecondsSinceAppActive() {
  if (SteamUtils() == NULL) {
    return 0;
  }
  return SteamUtils()->GetSecondsSinceAppActive();
}

int GodotSteamUtils::getCurrentBatteryPower() {
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

int GodotSteamUtils::getServerRealTime() {
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
  ObjectTypeDB::bind_method("getIPCountry", &GodotSteamUtils::getIPCountry);
  ObjectTypeDB::bind_method("isOverlayEnabled",
                            &GodotSteamUtils::isOverlayEnabled);
  ObjectTypeDB::bind_method("getSteamUILanguage",
                            &GodotSteamUtils::getSteamUILanguage);
  ObjectTypeDB::bind_method("getAppID", &GodotSteamUtils::getAppID);
  ObjectTypeDB::bind_method("getSecondsSinceAppActive",
                            &GodotSteamUtils::getSecondsSinceAppActive);
  ObjectTypeDB::bind_method(_MD("setOverlayNotificationPosition", "0-3"),
                            &GodotSteamUtils::setOverlayNotificationPosition);
  ObjectTypeDB::bind_method("getCurrentBatteryPower",
                            &GodotSteamUtils::getCurrentBatteryPower);
  ObjectTypeDB::bind_method("getServerRealTime",
                            &GodotSteamUtils::getServerRealTime);
  ObjectTypeDB::bind_method("isSteamRunningInVR",
                            &GodotSteamUtils::isSteamRunningInVR);
  ObjectTypeDB::bind_method("isSteamInBigPictureMode",
                            &GodotSteamUtils::isSteamInBigPictureMode);
  ObjectTypeDB::bind_method("startVRDashboard",
                            &GodotSteamUtils::startVRDashboard);
}