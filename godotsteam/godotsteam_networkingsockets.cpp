#include "godotsteam_networkingsockets.h"

GodotSteamNetworkingSockets *GodotSteamNetworkingSockets::singleton = NULL;

GodotSteamNetworkingSockets::GodotSteamNetworkingSockets():
  callbackRelayConnectionStatusChanged(this, &GodotSteamNetworkingSockets::_connection_status_changed)
{
    singleton = this;
}

GodotSteamNetworkingSockets::~GodotSteamNetworkingSockets() { singleton = NULL; }

GodotSteamNetworkingSockets *GodotSteamNetworkingSockets::get_singleton() {

  if (GodotSteamNetworkingSockets::singleton == NULL) {
    GodotSteamNetworkingSockets::singleton = new GodotSteamNetworkingSockets();
  }

  return GodotSteamNetworkingSockets::singleton;
}

void GodotSteamNetworkingSockets::reset_singleton() {
  delete GodotSteamNetworkingSockets::singleton;

  GodotSteamNetworkingSockets::singleton = NULL;
}

bool GodotSteamNetworkingSockets::isSteamNetworkingSocketsReady() { return SteamNetworkingSockets() != NULL; }

SteamNetworkingConfigValue_t* GodotSteamNetworkingSockets::convertOptionsArray(Array options) {

  int numberOfOptions = options.size();
  SteamNetworkingConfigValue_t *configurationList = new SteamNetworkingConfigValue_t[numberOfOptions];
  STEAM_FAIL_COND_V(numberOfOptions < 1, configurationList);

  for(int index = 0; index < numberOfOptions; index++){
    Dictionary option = options[index];

    switch ((ESteamNetworkingConfigDataType)((uint32_t)option["type"])) {
      case k_ESteamNetworkingConfigDataType__Force32Bit:
      case k_ESteamNetworkingConfig_Int32:
        configurationList[index].SetInt32((ESteamNetworkingConfigValue)((uint32_t)option["config"]), (int32_t)option["value"]);
        break;

      case k_ESteamNetworkingConfig_Int64:
        configurationList[index].SetInt64((ESteamNetworkingConfigValue)((uint32_t)option["config"]), (int64_t)option["value"]);
        break;

      case k_ESteamNetworkingConfig_Float:
        configurationList[index].SetFloat((ESteamNetworkingConfigValue)((uint32_t)option["config"]), (float)option["value"]);
        break;

      case k_ESteamNetworkingConfig_String:
        configurationList[index].SetString((ESteamNetworkingConfigValue)((uint32_t)option["config"]), ((String)option["value"]).utf8().get_data());
        break;

      case k_ESteamNetworkingConfig_Ptr:
        configurationList[index].SetPtr((ESteamNetworkingConfigValue)((uint32_t)option["config"]), (void *)option["value"]);
        break;
    }
  }

  return configurationList;
}

SteamNetworkingIdentity GodotSteamNetworkingSockets::getOrCreateNetworkingIdentity(uint64_t steamId) {

  if (networkingIdentities.count(steamId) < 1) {
    networkingIdentities[steamId] = SteamNetworkingIdentity();
    networkingIdentities[steamId].SetSteamID((uint64)steamId);
  }

  return networkingIdentities[steamId];
}

void GodotSteamNetworkingSockets::clearAllIdentities() {

  networkingIdentities.clear();
}

uint32_t GodotSteamNetworkingSockets::createListenSocketP2P(uint64_t numberOfSockets, Array optionsArray) {

  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), -1);
  STEAM_FAIL_COND_V(numberOfSockets > 16, -1);

  const SteamNetworkingConfigValue_t *options = convertOptionsArray(optionsArray);

  return (uint32_t)SteamNetworkingSockets()->CreateListenSocketP2P(numberOfSockets, optionsArray.size(), options);
}

bool GodotSteamNetworkingSockets::closeListenSocket(uint32_t socketToClose) {

  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), false);

  return SteamNetworkingSockets()->CloseListenSocket((HSteamListenSocket)socketToClose);
}

uint32_t GodotSteamNetworkingSockets::connectP2P(uint64_t remoteSteamId, uint64_t numberOfSockets, Array optionsArray) {

  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), -1);
  STEAM_FAIL_COND_V(numberOfSockets > 16, -1);

  const SteamNetworkingConfigValue_t *options = convertOptionsArray(optionsArray);
  const SteamNetworkingIdentity remoteIdentity = getOrCreateNetworkingIdentity(remoteSteamId);

  return (uint32_t)SteamNetworkingSockets()->ConnectP2P(remoteIdentity, numberOfSockets, optionsArray.size(), options);
}

uint32_t GodotSteamNetworkingSockets::acceptConnection(uint64_t handleConnection) {

  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), false);

  return (uint32_t)SteamNetworkingSockets()->AcceptConnection((HSteamNetConnection)handleConnection);
}

bool GodotSteamNetworkingSockets::closeConnection(uint64_t handleConnection, uint32_t reason, const String &debugData, bool enableLinger) {

  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), false);
  
  return SteamNetworkingSockets()->CloseConnection((HSteamNetConnection)handleConnection, reason, debugData.utf8().get_data(), enableLinger);
}

Dictionary GodotSteamNetworkingSockets::sendMessageToConnection(uint32 handleConnection, const PackedByteArray data, int flags) {

  Dictionary result;
  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), result);
  int64 number = -1;
  ;

  result["result"] = SteamNetworkingSockets()->SendMessageToConnection((HSteamNetConnection)handleConnection, data.ptr(), data.size(), flags, &number);
  result["message_index"] = (uint64_t)number;

  return result;
}

uint64_t GodotSteamNetworkingSockets::sendMessages(int messages, const PackedByteArray data, uint32 handleConnection, int flags) {

  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), -1);
  SteamNetworkingMessage_t *networkMessage = SteamNetworkingUtils()->AllocateMessage(0);

  networkMessage->m_pData = (void *)data.ptr();
  networkMessage->m_cbSize = data.size();
  networkMessage->m_conn = (HSteamNetConnection)handleConnection;
  networkMessage->m_nFlags = flags;
  int64 result = -1;

  SteamNetworkingSockets()->SendMessages(messages, &networkMessage, &result);
  networkMessage->Release();

  return (uint64_t)result;
}

int GodotSteamNetworkingSockets::flushMessagesOnConnection(uint32 handleConnection) {

  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), -1);

  return SteamNetworkingSockets()->FlushMessagesOnConnection((HSteamNetConnection)handleConnection);
}

Array GodotSteamNetworkingSockets::receiveMessagesOnConnection(uint32 handleConnection, int maxMessages) {

  Array result;
  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), result);
  SteamNetworkingMessage_t** messageList = new SteamNetworkingMessage_t*[maxMessages];
  int numberOfMessages = SteamNetworkingSockets()->ReceiveMessagesOnConnection((HSteamNetConnection)handleConnection, messageList, maxMessages);

  for (int index = 0; index < numberOfMessages; index++) {
    PackedByteArray data;
    data.resize(messageList[index]->m_cbSize);
    memcpy(data.ptrw(), (uint8_t*)messageList[index]->m_pData, messageList[index]->m_cbSize);

    char identity[k_cchMaxString];
    messageList[index]->m_identityPeer.ToString(identity, k_cchMaxString);

    Dictionary message;
    message["payload"] = data;
    message["size"] = messageList[index]->m_cbSize;
    message["connection"] = messageList[index]->m_conn;
    message["identity"] = identity;
    message["user_data"] = (uint64_t)messageList[index]->m_nConnUserData;
    message["time_received"] = (uint64_t)messageList[index]->m_usecTimeReceived;
    message["message_number"] = (uint64_t)messageList[index]->m_nMessageNumber;
    message["channel"] = messageList[index]->m_nChannel;
    message["flags"] = messageList[index]->m_nFlags;
    message["user_data"] = (uint64_t)messageList[index]->m_nUserData;

    result.append(message);

    messageList[index]->Release();
  }

  delete[] messageList;

  return result;
}

uint32 GodotSteamNetworkingSockets::createPollGroup() {

  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), -1);

  return SteamNetworkingSockets()->CreatePollGroup();
}

bool GodotSteamNetworkingSockets::destroyPollGroup(uint32 pollGroup) {

  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), -1);

  return SteamNetworkingSockets()->DestroyPollGroup((HSteamNetPollGroup)pollGroup);
}

bool GodotSteamNetworkingSockets::setConnectionPollGroup(uint32 handleConnection, uint32 pollGroup) {

  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), false);

  return SteamNetworkingSockets()->SetConnectionPollGroup((HSteamNetConnection)handleConnection, (HSteamNetPollGroup)pollGroup);
}

bool GodotSteamNetworkingSockets::removeConnectionFromPollGroup(uint32 handleConnection) {
  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), false);

  return SteamNetworkingSockets()->SetConnectionPollGroup((HSteamNetConnection)handleConnection, k_HSteamNetPollGroup_Invalid);
}

Array GodotSteamNetworkingSockets::receiveMessagesOnPollGroup(uint32 pollGroup, int maxMessages) {

  Array result;
  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), result);
  SteamNetworkingMessage_t** messageList = new SteamNetworkingMessage_t*[maxMessages];
  int numberOfMessages = SteamNetworkingSockets()->ReceiveMessagesOnPollGroup((HSteamNetPollGroup)pollGroup, messageList, maxMessages);

  for (int index = 0; index < numberOfMessages; index++) {
    PackedByteArray data;
    data.resize(messageList[index]->m_cbSize);
    memcpy(data.ptrw(), (uint8_t*)messageList[index]->m_pData, messageList[index]->m_cbSize);

    char identity[k_cchMaxString];
    messageList[index]->m_identityPeer.ToString(identity, k_cchMaxString);

    Dictionary message;
    message["payload"] = data;
    message["size"] = messageList[index]->m_cbSize;
    message["connection"] = messageList[index]->m_conn;
    message["identity"] = identity;
    message["remote_steam_id"] = (uint64_t)messageList[index]->m_identityPeer.GetSteamID().ConvertToUint64();
    message["connection_user_data"] = (uint64_t)messageList[index]->m_nConnUserData;
    message["time_received"] = (uint64_t)messageList[index]->m_usecTimeReceived;
    message["message_number"] = (uint64_t)messageList[index]->m_nMessageNumber;
    message["channel"] = messageList[index]->m_nChannel;
    message["flags"] = messageList[index]->m_nFlags;
    message["user_data"] = (uint64_t)messageList[index]->m_nUserData;

    result.append(message);

    messageList[index]->Release();
  }

  delete[] messageList;

  return result;
}

Dictionary GodotSteamNetworkingSockets::getConnectionInfo(uint32 handleConnection) {

  Dictionary result;
  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), result);
  SteamNetConnectionInfo_t info;

  if (SteamNetworkingSockets()->GetConnectionInfo((HSteamNetConnection)handleConnection, &info)) {
    char identity[k_cchMaxString];
    char ip_address[k_cchMaxString];

    info.m_identityRemote.ToString(identity, k_cchMaxString);
    info.m_addrRemote.ToString(ip_address, k_cchMaxString, true);

    result["identity"] = identity;
    result["remote_steam_id"] = (uint64_t)info.m_identityRemote.GetSteamID().ConvertToUint64();
    result["user_data"] = (uint64_t)info.m_nUserData;
    result["listen_socket"] = info.m_hListenSocket;
    result["remote_address"] = ip_address;
    result["remote_pop"] = info.m_idPOPRemote;
    result["pop_relay"] = info.m_idPOPRelay;
    result["connection_state"] = info.m_eState;
    result["end_reason"] = info.m_eEndReason;
    result["end_debug"] = info.m_szEndDebug;
    result["debug_description"] = info.m_szConnectionDescription;
  }

  return result;
}

Dictionary GodotSteamNetworkingSockets::getDetailedConnectionStatus(uint32 connection) {
  Dictionary result;
  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), result);
  char buffer[8192];

  result["success"] = SteamNetworkingSockets()->GetDetailedConnectionStatus((HSteamNetConnection)connection, buffer, 8192);
  result["status"] = buffer;

  return result;
}

uint64_t GodotSteamNetworkingSockets::getConnectionUserData(uint32 peer) {

  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), -1);

  return SteamNetworkingSockets()->GetConnectionUserData((HSteamNetConnection)peer);
}

void GodotSteamNetworkingSockets::setConnectionName(uint32 peer, const String& name) {
  
  STEAM_FAIL_COND(!isSteamNetworkingSocketsReady());

  SteamNetworkingSockets()->SetConnectionName((HSteamNetConnection)peer, name.utf8().get_data());
}

String GodotSteamNetworkingSockets::getConnectionName(uint32 peer) {

  String connectName = "";
  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), connectName);
  char buffer[256];

  if (SteamNetworkingSockets()->GetConnectionName((HSteamNetConnection)peer, buffer, 256)) {
    connectName += buffer;  
  }

  return connectName;
}

Dictionary GodotSteamNetworkingSockets::getListenSocketAddress(uint32 socket) {

  Dictionary result;
  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), result);
  SteamNetworkingIPAddr address;
  
  if (SteamNetworkingSockets()->GetListenSocketAddress((HSteamListenSocket)socket, &address)) {
    char ip_address[k_cchMaxString];
    address.ToString(ip_address, k_cchMaxString, true);

    result["ip_address"] = ip_address;
    result["status"] = true;
  }

  return result;
}

String GodotSteamNetworkingSockets::getIdentity() {

  String result = "";
  STEAM_FAIL_COND_V(!isSteamNetworkingSocketsReady(), result);

  SteamNetworkingIdentity identity;
  if (SteamNetworkingSockets()->GetIdentity(&identity)) {
    char buffer[k_cchMaxString];
    identity.ToString(buffer, k_cchMaxString);

    result += buffer;
  }

  return result;
}


void GodotSteamNetworkingSockets::_connection_status_changed(SteamNetConnectionStatusChangedCallback_t *pConnectionStatus) {

  uint64_t connectionHandle = pConnectionStatus->m_hConn;
  uint32_t previousState = pConnectionStatus->m_eOldState;
  char identity[k_cchMaxString];
  char ip_address[k_cchMaxString];

  pConnectionStatus->m_info.m_identityRemote.ToString(identity, k_cchMaxString);
  pConnectionStatus->m_info.m_addrRemote.ToString(ip_address, k_cchMaxString, true);

  Dictionary connectionInformations;
  connectionInformations["identity"] = identity;
  connectionInformations["remote_steam_id"] = (uint64_t)pConnectionStatus->m_info.m_identityRemote.GetSteamID().ConvertToUint64();
  connectionInformations["user_data"] = (uint64_t)pConnectionStatus->m_info.m_nUserData;
  connectionInformations["listen_socket"] = pConnectionStatus->m_info.m_hListenSocket;
  connectionInformations["remote_address"] = ip_address;
  connectionInformations["remote_pop"] = pConnectionStatus->m_info.m_idPOPRemote;
  connectionInformations["pop_relay"] = pConnectionStatus->m_info.m_idPOPRelay;
  connectionInformations["connection_state"] = pConnectionStatus->m_info.m_eState;
  connectionInformations["end_reason"] = pConnectionStatus->m_info.m_eEndReason;
  connectionInformations["end_debug"] = pConnectionStatus->m_info.m_szEndDebug;
  connectionInformations["debug_description"] = pConnectionStatus->m_info.m_szConnectionDescription;

  switch (pConnectionStatus->m_info.m_eState) {
    case k_ESteamNetworkingConnectionState_Connecting:

      emit_signal("connection_arrived", connectionHandle, connectionInformations, previousState);
      break;

    case k_ESteamNetworkingConnectionState_Connected:
      emit_signal("connection_established", connectionHandle, connectionInformations, previousState);
      break;

    case k_ESteamNetworkingConnectionState_ClosedByPeer:
    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
      SteamNetworkingSockets()->CloseConnection(pConnectionStatus->m_hConn, pConnectionStatus->m_info.m_eEndReason, pConnectionStatus->m_info.m_szEndDebug, true);
      emit_signal("connection_closed", connectionHandle, connectionInformations, previousState);
      break;

    case k_ESteamNetworkingConnectionState_None:
    default:
      // No-op
      break;
  }
}

void GodotSteamNetworkingSockets::_bind_methods() {

  ClassDB::bind_method(D_METHOD("createListenSocketP2P", "numberOfSockets"), &GodotSteamNetworkingSockets::createListenSocketP2P, DEFVAL(0));
  ClassDB::bind_method("connectP2P", &GodotSteamNetworkingSockets::connectP2P);
  ClassDB::bind_method("acceptConnection", &GodotSteamNetworkingSockets::acceptConnection);
  ClassDB::bind_method("closeListenSocket", &GodotSteamNetworkingSockets::closeListenSocket);
  ClassDB::bind_method("closeConnection", &GodotSteamNetworkingSockets::closeConnection);
  ClassDB::bind_method("clearAllIdentities", &GodotSteamNetworkingSockets::clearAllIdentities);

  ClassDB::bind_method("sendMessageToConnection", &GodotSteamNetworkingSockets::sendMessageToConnection);
  ClassDB::bind_method("sendMessages", &GodotSteamNetworkingSockets::sendMessages);
  ClassDB::bind_method("flushMessagesOnConnection", &GodotSteamNetworkingSockets::flushMessagesOnConnection);
  ClassDB::bind_method("receiveMessagesOnConnection", &GodotSteamNetworkingSockets::receiveMessagesOnConnection);

  ClassDB::bind_method("createPollGroup", &GodotSteamNetworkingSockets::createPollGroup);
  ClassDB::bind_method("destroyPollGroup", &GodotSteamNetworkingSockets::destroyPollGroup);
  ClassDB::bind_method("setConnectionPollGroup", &GodotSteamNetworkingSockets::setConnectionPollGroup);
  ClassDB::bind_method("removeConnectionFromPollGroup", &GodotSteamNetworkingSockets::removeConnectionFromPollGroup);
  ClassDB::bind_method("receiveMessagesOnPollGroup", &GodotSteamNetworkingSockets::receiveMessagesOnPollGroup);

  ClassDB::bind_method("getConnectionInfo", &GodotSteamNetworkingSockets::getConnectionInfo);
  ClassDB::bind_method("getDetailedConnectionStatus", &GodotSteamNetworkingSockets::getDetailedConnectionStatus);
  ClassDB::bind_method("getConnectionUserData", &GodotSteamNetworkingSockets::getConnectionUserData);
  ClassDB::bind_method("setConnectionName", &GodotSteamNetworkingSockets::setConnectionName);
  ClassDB::bind_method("getConnectionName", &GodotSteamNetworkingSockets::getConnectionName);
  ClassDB::bind_method("getListenSocketAddress", &GodotSteamNetworkingSockets::getListenSocketAddress);
  ClassDB::bind_method("getIdentity", &GodotSteamNetworkingSockets::getIdentity);

  ADD_SIGNAL(MethodInfo("connection_arrived",
    PropertyInfo(Variant::INT, "connection_handle"),
    PropertyInfo(Variant::DICTIONARY, "connection_informations"),
    PropertyInfo(Variant::INT, "previous_state")
  ));

  ADD_SIGNAL(MethodInfo("connection_established",
    PropertyInfo(Variant::INT, "connection_handle"),
    PropertyInfo(Variant::DICTIONARY, "connection_informations"),
    PropertyInfo(Variant::INT, "previous_state")
  ));

  ADD_SIGNAL(MethodInfo("connection_closed",
    PropertyInfo(Variant::INT, "connection_handle"),
    PropertyInfo(Variant::DICTIONARY, "connection_informations"),
    PropertyInfo(Variant::INT, "previous_state")
  ));

  BIND_CONSTANT(NETWORKING_SOCKETS_CONFIG_INT32);
  BIND_CONSTANT(NETWORKING_SOCKETS_CONFIG_INT64);
  BIND_CONSTANT(NETWORKING_SOCKETS_CONFIG_FLOAT);
  BIND_CONSTANT(NETWORKING_SOCKETS_CONFIG_STRING);
  BIND_CONSTANT(NETWORKING_SOCKETS_CONFIG_PTR);

  BIND_CONSTANT(NETWORKING_SOCKETS_CONNECTIONSTATE_NONE);
  BIND_CONSTANT(NETWORKING_SOCKETS_CONNECTIONSTATE_CONNECTING);
  BIND_CONSTANT(NETWORKING_SOCKETS_CONNECTIONSTATE_FINDING_ROUTE);
  BIND_CONSTANT(NETWORKING_SOCKETS_CONNECTIONSTATE_CONNECTED);
  BIND_CONSTANT(NETWORKING_SOCKETS_CONNECTIONSTATE_CLOSED_BY_PEER);
  BIND_CONSTANT(NETWORKING_SOCKETS_CONNECTIONSTATE_PROBLEM_DETECTED_LOCALLY);
  BIND_CONSTANT(NETWORKING_SOCKETS_CONNECTIONSTATE_FIN_WAIT);
  BIND_CONSTANT(NETWORKING_SOCKETS_CONNECTIONSTATE_LINGER);
  BIND_CONSTANT(NETWORKING_SOCKETS_CONNECTIONSTATE_DEAD);
  BIND_CONSTANT(NETWORKING_SOCKETS_CONNECTIONSTATE__FORCE32BIT);

  BIND_CONSTANT(NETWORKING_SOCKETS_SEND_UNRELIABLE);
  BIND_CONSTANT(NETWORKING_SOCKETS_SEND_NO_NAGLE);
  BIND_CONSTANT(NETWORKING_SOCKETS_SEND_UNRELIABLE_NO_NAGLE);
  BIND_CONSTANT(NETWORKING_SOCKETS_SEND_NO_DELAY);
  BIND_CONSTANT(NETWORKING_SOCKETS_SEND_UNRELIABLE_NO_DELAY);
  BIND_CONSTANT(NETWORKING_SOCKETS_SEND_RELIABLE);
  BIND_CONSTANT(NETWORKING_SOCKETS_SEND_RELIABLE_NO_NAGLE);
  BIND_CONSTANT(NETWORKING_SOCKETS_SEND_USE_CURRENT_THREAD);
  BIND_CONSTANT(NETWORKING_SOCKETS_SEND_AUTO_RESTART_BROKEN_SESSION);
}
