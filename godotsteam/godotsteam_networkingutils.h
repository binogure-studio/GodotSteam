#ifndef GODOTSTEAMNETWORKINGUTILS_H
#define GODOTSTEAMNETWORKINGUTILS_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/variant/dictionary.h"
#include "scene/resources/texture.h"

// Local dependencies
#include "godotsteam_utils.h"

class GodotSteamNetworkingUtils : public Object
{
public:

  enum {
    NETWORKING_AVAILABILITY_CANNOT_TRY = (uint64_t)k_ESteamNetworkingAvailability_CannotTry,
    NETWORKING_AVAILABILITY_FAILED = (uint64_t)k_ESteamNetworkingAvailability_Failed,
    NETWORKING_AVAILABILITY_PREVIOUSLY = (uint64_t)k_ESteamNetworkingAvailability_Previously,
    NETWORKING_AVAILABILITY_RETRYING = (uint64_t)k_ESteamNetworkingAvailability_Retrying,
    NETWORKING_AVAILABILITY_NEVER_TRIED = (uint64_t)k_ESteamNetworkingAvailability_NeverTried,
    NETWORKING_AVAILABILITY_WAITING = (uint64_t)k_ESteamNetworkingAvailability_Waiting,
    NETWORKING_AVAILABILITY_ATTEMPTING = (uint64_t)k_ESteamNetworkingAvailability_Attempting,
    NETWORKING_AVAILABILITY_CURRENT = (uint64_t)k_ESteamNetworkingAvailability_Current,
    NETWORKING_AVAILABILITY_UNKNOWN = (uint64_t)k_ESteamNetworkingAvailability_Unknown,
    NETWORKING_AVAILABILITY_FORCE32_BIT = (uint64_t)k_ESteamNetworkingAvailability__Force32bit
  };

  static GodotSteamNetworkingUtils *get_singleton();
  static void reset_singleton();

  GodotSteamNetworkingUtils();
  ~GodotSteamNetworkingUtils();

  bool isSteamNetworkingUtilsReady();
  void initRelayNetworkAccess();
  uint64_t updateRelayNetworkStatus();
  Dictionary getRelayNetworkStatus();

protected:
  static void _bind_methods();
  static GodotSteamNetworkingUtils *singleton;

private:
  SteamRelayNetworkStatus_t network_status;

  STEAM_CALLBACK(GodotSteamNetworkingUtils, _relay_network_status_updated, SteamRelayNetworkStatus_t, callbackRelayNetworkStatusUpdated);

  GDCLASS(GodotSteamNetworkingUtils, Object);
};

#endif // GODOTSTEAMNETWORKINGUTILS_H
