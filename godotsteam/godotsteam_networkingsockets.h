#ifndef GODOTSTEAMNETWORKINGSOCKETS_H
#define GODOTSTEAMNETWORKINGSOCKETS_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/variant/dictionary.h"
#include "scene/resources/texture.h"

// Local dependencies
#include "godotsteam_utils.h"
#include "map"

class GodotSteamNetworkingSockets : public Object
{
public:
  enum {
    NETWORKING_SOCKETS_CONFIG_INT32 = (uint64_t)k_ESteamNetworkingConfig_Int32,
    NETWORKING_SOCKETS_CONFIG_INT64 = (uint64_t)k_ESteamNetworkingConfig_Int64,
    NETWORKING_SOCKETS_CONFIG_FLOAT = (uint64_t)k_ESteamNetworkingConfig_Float,
    NETWORKING_SOCKETS_CONFIG_STRING = (uint64_t)k_ESteamNetworkingConfig_String,
    NETWORKING_SOCKETS_CONFIG_PTR = (uint64_t)k_ESteamNetworkingConfig_Ptr,

    NETWORKING_SOCKETS_CONNECTIONSTATE_NONE = (uint64_t)k_ESteamNetworkingConnectionState_None,
    NETWORKING_SOCKETS_CONNECTIONSTATE_CONNECTING = (uint64_t)k_ESteamNetworkingConnectionState_Connecting,
    NETWORKING_SOCKETS_CONNECTIONSTATE_FINDING_ROUTE = (uint64_t)k_ESteamNetworkingConnectionState_FindingRoute,
    NETWORKING_SOCKETS_CONNECTIONSTATE_CONNECTED = (uint64_t)k_ESteamNetworkingConnectionState_Connected,
    NETWORKING_SOCKETS_CONNECTIONSTATE_CLOSED_BY_PEER = (uint64_t)k_ESteamNetworkingConnectionState_ClosedByPeer,
    NETWORKING_SOCKETS_CONNECTIONSTATE_PROBLEM_DETECTED_LOCALLY = (uint64_t)k_ESteamNetworkingConnectionState_ProblemDetectedLocally,
    NETWORKING_SOCKETS_CONNECTIONSTATE_FIN_WAIT = (uint64_t)k_ESteamNetworkingConnectionState_FinWait,
    NETWORKING_SOCKETS_CONNECTIONSTATE_LINGER = (uint64_t)k_ESteamNetworkingConnectionState_Linger,
    NETWORKING_SOCKETS_CONNECTIONSTATE_DEAD = (uint64_t)k_ESteamNetworkingConnectionState_Dead,
    NETWORKING_SOCKETS_CONNECTIONSTATE__FORCE32BIT = (uint64_t)k_ESteamNetworkingConnectionState__Force32Bit,

    NETWORKING_SOCKETS_SEND_UNRELIABLE = (uint64_t)k_nSteamNetworkingSend_Unreliable,
    NETWORKING_SOCKETS_SEND_NO_NAGLE = (uint64_t)k_nSteamNetworkingSend_NoNagle,
    NETWORKING_SOCKETS_SEND_UNRELIABLE_NO_NAGLE = (uint64_t)k_nSteamNetworkingSend_UnreliableNoNagle,
    NETWORKING_SOCKETS_SEND_NO_DELAY = (uint64_t)k_nSteamNetworkingSend_NoDelay,
    NETWORKING_SOCKETS_SEND_UNRELIABLE_NO_DELAY = (uint64_t)k_nSteamNetworkingSend_UnreliableNoDelay,
    NETWORKING_SOCKETS_SEND_RELIABLE = (uint64_t)k_nSteamNetworkingSend_Reliable,
    NETWORKING_SOCKETS_SEND_RELIABLE_NO_NAGLE = (uint64_t)k_nSteamNetworkingSend_ReliableNoNagle,
    NETWORKING_SOCKETS_SEND_USE_CURRENT_THREAD = (uint64_t)k_nSteamNetworkingSend_UseCurrentThread,
    NETWORKING_SOCKETS_SEND_AUTO_RESTART_BROKEN_SESSION = (uint64_t)k_nSteamNetworkingSend_AutoRestartBrokenSession,

    k_cchMaxString = 48
  };

  static GodotSteamNetworkingSockets *get_singleton();
  static void reset_singleton();

  GodotSteamNetworkingSockets();
  ~GodotSteamNetworkingSockets();

  bool isSteamNetworkingSocketsReady();

  void clearAllIdentities();

  uint64_t initAuthentication();
  uint64_t getAuthenticationStatus();
  uint32_t createListenSocketP2P(uint64_t numberOfSockets, Array optionsArray);
  bool closeListenSocket(uint32_t socketToClose);
  uint32_t connectP2P(uint64_t remoteSteamId, uint64_t numberOfSockets, Array optionsArray);
  uint32_t acceptConnection(uint64_t handleConnection);
  bool closeConnection(uint64_t handleConnection, uint32_t reason, const String &debugData = "", bool enableLinger = true);

  Dictionary sendMessageToConnection(uint32 handleConnection, const PackedByteArray data, int flags);
  uint64_t sendMessages(int messages, const PackedByteArray data, uint32 handleConnection, int flags);
  int flushMessagesOnConnection(uint32 handleConnection);
  Array receiveMessagesOnConnection(uint32 handleConnection, int maxMessages);

  uint32 createPollGroup();
  bool destroyPollGroup(uint32 pollGroup);
  bool setConnectionPollGroup(uint32 handleConnection, uint32 pollGroup);
  bool removeConnectionFromPollGroup(uint32 handleConnection);
  Array receiveMessagesOnPollGroup(uint32 pollGroup, int maxMessages);

  Dictionary getConnectionInfo(uint32 handleConnection);
  Dictionary getDetailedConnectionStatus(uint32 connection);
  Dictionary getConnectionRealTimeStatus(uint32 connection);
  uint64_t getConnectionUserData(uint32 peer);
  void setConnectionName(uint32 peer, const String& name);
  String getConnectionName(uint32 peer);
  Dictionary getListenSocketAddress(uint32 socket);
  String getIdentity();

  SteamNetworkingIdentity getOrCreateNetworkingIdentity(uint64_t steamId);
protected:
  static void _bind_methods();
  static GodotSteamNetworkingSockets *singleton;

  SteamNetworkingConfigValue_t* convertOptionsArray(Array options);
private:

  std::map<uint64_t, SteamNetworkingIdentity> networkingIdentities;

  STEAM_CALLBACK(GodotSteamNetworkingSockets, _connection_status_changed, SteamNetConnectionStatusChangedCallback_t, callbackRelayConnectionStatusChanged);
  STEAM_CALLBACK(GodotSteamNetworkingSockets, _authentication_status_changed, SteamNetAuthenticationStatus_t , callbackAuthenticationStatusChanged);

  GDCLASS(GodotSteamNetworkingSockets, Object);
};

#endif // GODOTSTEAMNETWORKINGSOCKETS_H
