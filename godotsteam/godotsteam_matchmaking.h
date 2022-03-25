#ifndef GODOTSTEAMMATCHMAKING_H
#define GODOTSTEAMMATCHMAKING_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/io/ip.h"
#include "core/io/ip_address.h"
#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/variant/dictionary.h"
#include "scene/resources/texture.h"

// Local dependencies
#include "godotsteam_utils.h"

class GodotSteamMatchmaking: public Object
{
public:
  enum {
    LOBBY_TYPE_PRIVATE = (uint64_t)k_ELobbyTypePrivate,
    LOBBY_TYPE_FRIENDS = (uint64_t)k_ELobbyTypeFriendsOnly,
    LOBBY_TYPE_PUBLIC = (uint64_t)k_ELobbyTypePublic,
    LOBBY_TYPE_INVISIBLE = (uint64_t)k_ELobbyTypeInvisible,

    LOBBY_CONNECTION_OK = (uint64_t)k_EResultOK,
    LOBBY_CONNECTION_NO_CONNECTION = (uint64_t)k_EResultNoConnection,
    LOBBY_CONNECTION_TIMEOUT = (uint64_t)k_EResultTimeout,
    LOBBY_CONNECTION_FAIL = (uint64_t)k_EResultFail,
    LOBBY_CONNECTION_ACCESS_DENIED = (uint64_t)k_EResultAccessDenied,
    LOBBY_CONNECTION_LIMIT_EXCEEDED = (uint64_t)k_EResultLimitExceeded,

    LOBBY_FILTER_DISTANCE_CLOSE = (uint64_t)k_ELobbyDistanceFilterClose,
    LOBBY_FILTER_DISTANCE_DEFAULT = (uint64_t)k_ELobbyDistanceFilterDefault,
    LOBBY_FILTER_DISTANCE_FAR = (uint64_t)k_ELobbyDistanceFilterFar,
    LOBBY_FILTER_DISTANCE_WORLDWIDE = (uint64_t)k_ELobbyDistanceFilterWorldwide,

    CHAT_MEMBER_STATE_CHANGE_ENTERED = (uint64_t) k_EChatMemberStateChangeEntered,
    CHAT_MEMBER_STATE_CHANGE_LEFT = (uint64_t) k_EChatMemberStateChangeLeft,
    CHAT_MEMBER_STATE_CHANGE_DISCONNECTED = (uint64_t) k_EChatMemberStateChangeDisconnected,
    CHAT_MEMBER_STATE_CHANGE_KICKED = (uint64_t) k_EChatMemberStateChangeKicked,
    CHAT_MEMBER_STATE_CHANGE_BANNED = (uint64_t) k_EChatMemberStateChangeBanned
  };

  static GodotSteamMatchmaking *get_singleton();
  static void reset_singleton();

  GodotSteamMatchmaking();
  ~GodotSteamMatchmaking();

  bool isSteamMatchmakingReady();

  void createLobby(uint64_t lobbyType, uint64_t cMaxMembers);
  void joinLobby(uint64_t steamIDLobby);
  void leaveLobby(uint64_t steamIDLobby);
  bool inviteUserToLobby(uint64_t steamIDLobby, uint64_t steamIDInvitee);

  void findLobbies(int distanceFilter);

  Dictionary getLobbyData(uint64_t steamIDLobby);
  uint64_t getLobbyOwner(uint64_t steamIDLobby);
  void setLobbyOwner(uint64_t steamIDLobby, uint64_t steamIDNewOwner);
  bool setLobbyType(uint64_t steamIDLobby, uint64_t lobby_type);
  bool setLobbyJoinable(uint64_t steamIDLobby, bool joinable);
  bool sendLobbyChatMessage(uint64_t steamIDLobby, const String &message);

protected:
  static void _bind_methods();
  static GodotSteamMatchmaking *singleton;

private:
  CCallResult<GodotSteamMatchmaking, LobbyCreated_t> callResultCreateLobby;
  void _lobby_created(LobbyCreated_t *callData, bool ioFailure);

  CCallResult<GodotSteamMatchmaking, LobbyMatchList_t> callResultFindLobby;
  void _lobbies_found(LobbyMatchList_t *callData, bool ioFailure);

  STEAM_CALLBACK(GodotSteamMatchmaking, _lobby_joined, LobbyEnter_t, callbackLobbyJoined);
  STEAM_CALLBACK(GodotSteamMatchmaking, _lobby_invite, LobbyInvite_t, callbackLobbyInvite);
  STEAM_CALLBACK(GodotSteamMatchmaking, _join_requested, GameRichPresenceJoinRequested_t, callbackJoinRequested);
  STEAM_CALLBACK(GodotSteamMatchmaking, _message_received, LobbyChatMsg_t, callbackMessageReceived);
  STEAM_CALLBACK(GodotSteamMatchmaking, _lobby_chat_update, LobbyChatUpdate_t, callbackLobbyChatUpdate);

  GDCLASS(GodotSteamMatchmaking, Object);
};
#endif // GODOTSTEAMMATCHMAKING_H
