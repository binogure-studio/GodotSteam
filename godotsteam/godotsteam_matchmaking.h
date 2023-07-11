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
#include "godotsteam_user.h"
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
    CHAT_MEMBER_STATE_CHANGE_BANNED = (uint64_t) k_EChatMemberStateChangeBanned,

    LOBBY_JOIN_SUCCESS = (uint64_t) k_EChatRoomEnterResponseSuccess,
    LOBBY_JOIN_DOESNT_EXIST = (uint64_t) k_EChatRoomEnterResponseDoesntExist,
    LOBBY_JOIN_NOT_ALLOWED = (uint64_t) k_EChatRoomEnterResponseNotAllowed,
    LOBBY_JOIN_FULL = (uint64_t) k_EChatRoomEnterResponseFull,
    LOBBY_JOIN_ERROR = (uint64_t) k_EChatRoomEnterResponseError,
    LOBBY_JOIN_BANNED = (uint64_t) k_EChatRoomEnterResponseBanned,
    LOBBY_JOIN_LIMITED = (uint64_t) k_EChatRoomEnterResponseLimited,
    LOBBY_JOIN_CLAN_DISABLED = (uint64_t) k_EChatRoomEnterResponseClanDisabled,
    LOBBY_JOIN_COMMUNITY_BAN = (uint64_t) k_EChatRoomEnterResponseCommunityBan,
    LOBBY_JOIN_MEMBER_BLOCKED_YOU = (uint64_t) k_EChatRoomEnterResponseMemberBlockedYou,
    LOBBY_JOIN_YOU_BLOCKED_MEMBER = (uint64_t) k_EChatRoomEnterResponseYouBlockedMember,
    LOBBY_JOIN_RATE_LIMITE_XCEEDED = (uint64_t) k_EChatRoomEnterResponseRatelimitExceeded,

    LOBBY_COMPARISON_EQUAL_TO_OR_LESS_THAN = (uint64_t) k_ELobbyComparisonEqualToOrLessThan,
    LOBBY_COMPARISON_LESS_THAN = (uint64_t) k_ELobbyComparisonLessThan,
    LOBBY_COMPARISON_EQUAL = (uint64_t) k_ELobbyComparisonEqual,
    LOBBY_COMPARISON_GREATER_THAN = (uint64_t) k_ELobbyComparisonGreaterThan,
    LOBBY_COMPARISON_EQUAL_TO_OR_GREATER_THAN = (uint64_t) k_ELobbyComparisonEqualToOrGreaterThan,
    LOBBY_COMPARISON_NOT_EQUAL = (uint64_t) k_ELobbyComparisonNotEqual,

    MESSAGE_KICK = 0,
    MESSAGE_TEXT = 1
  };

  struct ChatMessage {
    int type = MESSAGE_TEXT;
    uint64_t userID;
    uint32_t textLength;
    const char *text;
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

  void addRequestLobbyListStringFilter(const String &key, const String &value, uint64_t comparison_type);
  void addRequestLobbyListFilterSlotsAvailable(uint64_t slotsAvailable);
  void addRequestLobbyListDistanceFilter(uint64_t distanceFilter);
  void addRequestLobbyListResultCountFilter(uint64_t distanceFilter);
  void requestLobbyList();

  bool kickUserFromLobby(uint64_t steamIDLobby, uint64_t steamIDUser);
  String getLobbyMemberData(uint64_t steamIDLobby, uint64_t steamIDUser, const String &key);

  Array getLobbyMemberList(uint64_t steamIDLobby);
  Dictionary getLobbyData(uint64_t steamIDLobby);
  uint64_t getLobbyOwner(uint64_t steamIDLobby);

  bool deleteLobbyData(uint64_t steamIDLobby, const String &key);
  bool requestLobbyData(uint64_t steamIDLobby);
  bool setLobbyData(uint64_t steamIDLobby, const String &key, const String &value);
  void setLobbyOwner(uint64_t steamIDLobby, uint64_t steamIDNewOwner);
  bool setLobbyType(uint64_t steamIDLobby, uint64_t lobby_type);
  bool setLobbyJoinable(uint64_t steamIDLobby, bool joinable);
  void setLobbyMemberData(uint64_t steamIDLobby, const String &key, const String &value);
  bool sendLobbyChatMessage(uint64_t steamIDLobby, const String &message);

protected:
  static void _bind_methods();
  static GodotSteamMatchmaking *singleton;

private:
  CCallResult<GodotSteamMatchmaking, LobbyCreated_t> callResultCreateLobby;
  void _lobby_created(LobbyCreated_t *callData, bool ioFailure);

  CCallResult<GodotSteamMatchmaking, LobbyMatchList_t> callResultFindLobby;
  void _lobbies_found(LobbyMatchList_t *callData, bool ioFailure);

  CCallResult<GodotSteamMatchmaking, LobbyEnter_t> callResultJoinLobby;
  void _lobby_joined(LobbyEnter_t *callData, bool ioFailure);

  STEAM_CALLBACK(GodotSteamMatchmaking, _lobby_game_created, LobbyGameCreated_t, callbackLobbyGameCreated);
  STEAM_CALLBACK(GodotSteamMatchmaking, _lobby_kicked, LobbyKicked_t, callbackLobbyKicked);
  STEAM_CALLBACK(GodotSteamMatchmaking, _lobby_data_updated, LobbyDataUpdate_t, callbackLobbyDataUpdated);
  STEAM_CALLBACK(GodotSteamMatchmaking, _lobby_invite, LobbyInvite_t, callbackLobbyInvite);
  STEAM_CALLBACK(GodotSteamMatchmaking, _join_requested, GameRichPresenceJoinRequested_t, callbackJoinRequested);
  STEAM_CALLBACK(GodotSteamMatchmaking, _message_received, LobbyChatMsg_t, callbackMessageReceived);
  STEAM_CALLBACK(GodotSteamMatchmaking, _lobby_chat_update, LobbyChatUpdate_t, callbackLobbyChatUpdate);

  GDCLASS(GodotSteamMatchmaking, Object);
};
#endif // GODOTSTEAMMATCHMAKING_H
