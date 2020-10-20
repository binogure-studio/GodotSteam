#ifndef GODOTSTEAMMATCHMAKING_H
#define GODOTSTEAMMATCHMAKING_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/io/ip.h"
#include "core/io/ip_address.h"
#include "core/dictionary.h"
#include "core/object.h"
#include "core/reference.h"
#include "scene/resources/texture.h"

// Local dependencies
#include "godotsteam_utils.h"

class GodotSteamMatchmaking: public Object
{
public:
  enum {
    LOBBY_TYPE_PRIVATE = (int)k_ELobbyTypePrivate,
    LOBBY_TYPE_FRIENDS = (int)k_ELobbyTypeFriendsOnly,
    LOBBY_TYPE_PUBLIC = (int)k_ELobbyTypePublic,
    LOBBY_TYPE_INVISIBLE = (int)k_ELobbyTypeInvisible,

    LOBBY_CONNECTION_OK = (int)k_EResultOK,
    LOBBY_CONNECTION_NO_CONNECTION = (int)k_EResultNoConnection,
    LOBBY_CONNECTION_TIMEOUT = (int)k_EResultTimeout,
    LOBBY_CONNECTION_FAIL = (int)k_EResultFail,
    LOBBY_CONNECTION_ACCESS_DENIED = (int)k_EResultAccessDenied,
    LOBBY_CONNECTION_LIMIT_EXCEEDED = (int)k_EResultLimitExceeded
  };

  static GodotSteamMatchmaking *get_singleton();
  static void reset_singleton();

  GodotSteamMatchmaking();
  ~GodotSteamMatchmaking();

  bool isSteamMatchmakingReady();
  void createLobby(int lobbyType, int cMaxMembers);
  void joinLobby(int steamIDLobby);
  void leaveLobby(int steamIDLobby);
  bool inviteUserToLobby(int steamIDLobby, int steamIDInvitee);

protected:
  static void _bind_methods();
  static GodotSteamMatchmaking *singleton;

private:
  CCallResult<GodotSteamMatchmaking, LobbyCreated_t> callResultCreateLobby;
  void _lobby_created(LobbyCreated_t *callData, bool ioFailure);

  STEAM_CALLBACK(GodotSteamMatchmaking, _lobby_joined, LobbyEnter_t);
  STEAM_CALLBACK(GodotSteamMatchmaking, _lobby_invite, LobbyInvite_t);
  STEAM_CALLBACK(GodotSteamMatchmaking, _join_requested, GameRichPresenceJoinRequested_t);
  STEAM_CALLBACK(GodotSteamMatchmaking, _server_connected, SteamServersConnected_t);
  STEAM_CALLBACK(GodotSteamMatchmaking, _server_disconnected, SteamServersDisconnected_t);

  OBJ_TYPE(GodotSteamMatchmaking, Object);
  OBJ_CATEGORY("GodotSteamMatchmaking");
};
#endif // GODOTSTEAMMATCHMAKING_H
