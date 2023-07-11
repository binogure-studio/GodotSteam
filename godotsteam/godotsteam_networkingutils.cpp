#include "godotsteam_networkingutils.h"

GodotSteamNetworkingUtils *GodotSteamNetworkingUtils::singleton = NULL;

GodotSteamNetworkingUtils::GodotSteamNetworkingUtils():
  callbackRelayNetworkStatusUpdated(this, &GodotSteamNetworkingUtils::_relay_network_status_updated)
{
    singleton = this;
}

GodotSteamNetworkingUtils::~GodotSteamNetworkingUtils() { singleton = NULL; }

GodotSteamNetworkingUtils *GodotSteamNetworkingUtils::get_singleton() {
  if (GodotSteamNetworkingUtils::singleton == NULL) {
    GodotSteamNetworkingUtils::singleton = new GodotSteamNetworkingUtils();
  }

  return GodotSteamNetworkingUtils::singleton;
}

void GodotSteamNetworkingUtils::reset_singleton() {
  delete GodotSteamNetworkingUtils::singleton;

  GodotSteamNetworkingUtils::singleton = NULL;
}

bool GodotSteamNetworkingUtils::isSteamNetworkingUtilsReady() { return SteamNetworkingUtils() != NULL; }

void GodotSteamNetworkingUtils::initRelayNetworkAccess() {
  STEAM_FAIL_COND(!isSteamNetworkingUtilsReady());

  SteamNetworkingUtils()->InitRelayNetworkAccess();
}

uint64_t GodotSteamNetworkingUtils::updateRelayNetworkStatus() {
  STEAM_FAIL_COND_V(!isSteamNetworkingUtilsReady(), -1);

  return (uint64_t) SteamNetworkingUtils()->GetRelayNetworkStatus(&network_status);
}

Dictionary GodotSteamNetworkingUtils::getRelayNetworkStatus() {
  Dictionary result;

  STEAM_FAIL_COND_V(!isSteamNetworkingUtilsReady(), result);

  result["availability"] = network_status.m_eAvail;
  result["ping_in_progress"] = network_status.m_bPingMeasurementInProgress;
  result["network_config_availability"] = network_status.m_eAvailNetworkConfig;
  result["any_relay_availability"] = network_status.m_eAvailAnyRelay;

  return result;
}

void GodotSteamNetworkingUtils::_relay_network_status_updated(SteamRelayNetworkStatus_t *pNetworkStatus) {

  network_status.m_eAvail = pNetworkStatus->m_eAvail;
  network_status.m_bPingMeasurementInProgress = pNetworkStatus->m_bPingMeasurementInProgress;
  network_status.m_eAvailNetworkConfig = pNetworkStatus->m_eAvailNetworkConfig;
  network_status.m_eAvailAnyRelay = pNetworkStatus->m_eAvailAnyRelay;

  emit_signal("network_status_updated");
}

void GodotSteamNetworkingUtils::_bind_methods() {

  ClassDB::bind_method("initRelayNetworkAccess", &GodotSteamNetworkingUtils::initRelayNetworkAccess);
  ClassDB::bind_method("updateRelayNetworkStatus", &GodotSteamNetworkingUtils::updateRelayNetworkStatus);
  ClassDB::bind_method("getRelayNetworkStatus", &GodotSteamNetworkingUtils::getRelayNetworkStatus);

  ADD_SIGNAL(MethodInfo("network_status_updated"));

  BIND_CONSTANT(NETWORKING_AVAILABILITY_CANNOT_TRY);
  BIND_CONSTANT(NETWORKING_AVAILABILITY_FAILED);
  BIND_CONSTANT(NETWORKING_AVAILABILITY_PREVIOUSLY);
  BIND_CONSTANT(NETWORKING_AVAILABILITY_RETRYING);
  BIND_CONSTANT(NETWORKING_AVAILABILITY_NEVER_TRIED);
  BIND_CONSTANT(NETWORKING_AVAILABILITY_WAITING);
  BIND_CONSTANT(NETWORKING_AVAILABILITY_ATTEMPTING);
  BIND_CONSTANT(NETWORKING_AVAILABILITY_CURRENT);
  BIND_CONSTANT(NETWORKING_AVAILABILITY_UNKNOWN);
  BIND_CONSTANT(NETWORKING_AVAILABILITY_FORCE32_BIT);
}
