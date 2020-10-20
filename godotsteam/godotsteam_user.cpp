#include "godotsteam_user.h"

GodotSteamUser *GodotSteamUser::singleton = NULL;

GodotSteamUser::GodotSteamUser() { singleton = this; }

GodotSteamUser::~GodotSteamUser() { singleton = NULL; }

GodotSteamUser *GodotSteamUser::get_singleton() {
  if (GodotSteamUser::singleton == NULL) {
    GodotSteamUser::singleton = new GodotSteamUser();
  }

  return GodotSteamUser::singleton;
}

void GodotSteamUser::reset_singleton() {
  delete GodotSteamUser::singleton;

  GodotSteamUser::singleton = NULL;
}

bool GodotSteamUser::isSteamUserReady() { return SteamUser() != NULL; }

int GodotSteamUser::getSteamID() {
  STEAM_FAIL_COND_V(!isSteamUserReady(), 0);

  CSteamID cSteamID = SteamUser()->GetSteamID();
  return cSteamID.ConvertToUint64();
}

bool GodotSteamUser::loggedOn() {
  if (!isSteamUserReady()) {
    return false;
  }
  return SteamUser()->BLoggedOn();
}

int GodotSteamUser::getPlayerSteamLevel() {
  STEAM_FAIL_COND_V(!isSteamUserReady(), 0);

  return SteamUser()->GetPlayerSteamLevel();
}

String GodotSteamUser::getUserDataFolder() {
  STEAM_FAIL_COND_V(!isSteamUserReady(), "");

  const int bufferSize = 256;
  char *buffer = new char[bufferSize];
  SteamUser()->GetUserDataFolder((char *)buffer, bufferSize);
  String data_path = buffer;
  delete[] buffer;
  return data_path;
}

void GodotSteamUser::advertiseGame(const String &server_ip, int port) {
  STEAM_FAIL_COND(!isSteamUserReady());

  // Resolve address and convert it from IP_Address struct to uint32_t
  IP_Address addr;
  if (server_ip.is_valid_ip_address()) {
    addr = server_ip;
  } else {
    addr = IP::get_singleton()->resolve_hostname(server_ip, IP::TYPE_IPV4);
  }
  // Resolution failed - Godot 3.0 has is_invalid() to check this
  if (addr == IP_Address()) {
    return;
  }
  uint32_t ip4 = *((uint32_t *)addr.get_ipv4());
  // Swap the bytes
  uint8_t *ip4_p = (uint8_t *)&ip4;
  for (int index = 0; index < 2; index++) {
    uint8_t temp = ip4_p[index];
    ip4_p[index] = ip4_p[3 - index];
    ip4_p[3 - index] = temp;
  }
  CSteamID gameserverID = SteamUser()->GetSteamID();
  SteamUser()->AdvertiseGame(gameserverID, *((uint32_t *)ip4_p), port);
}

// Trading Card badges data access, if you only have one set of cards, the
// series will be 1 The user has can have two different badges for a series; the
// regular (max level 5) and the foil (max level 1)
int GodotSteamUser::getGameBadgeLevel(int series, bool foil) {
  STEAM_FAIL_COND_V(!isSteamUserReady(), 0);

  return SteamUser()->GetGameBadgeLevel(series, foil);
}

void GodotSteamUser::_bind_methods() {
  ObjectTypeDB::bind_method("getSteamID", &GodotSteamUser::getSteamID);
  ObjectTypeDB::bind_method("loggedOn", &GodotSteamUser::loggedOn);
  ObjectTypeDB::bind_method("getPlayerSteamLevel",
                            &GodotSteamUser::getPlayerSteamLevel);
  ObjectTypeDB::bind_method("getUserDataFolder",
                            &GodotSteamUser::getUserDataFolder);
  ObjectTypeDB::bind_method(_MD("advertiseGame", "server_ip", "port"),
                            &GodotSteamUser::advertiseGame);
  ObjectTypeDB::bind_method("getGameBadgeLevel",
                            &GodotSteamUser::getGameBadgeLevel);
}
