#ifndef GODOTSTEAMUSER_H
#define GODOTSTEAMUSER_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/io/ip.h"
#include "core/io/ip_address.h"
#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/variant/dictionary.h"
#include "scene/resources/texture.h"

// Local dependencies
#include "godotsteam_networkingsockets.h"
#include "godotsteam_utils.h"

class GodotSteamUser : public Object
{
public:

  enum {
    BEGIN_AUTH_SESSION_RESULT_OK = (uint64_t)k_EBeginAuthSessionResultOK,
    BEGIN_AUTH_SESSION_RESULT_INVALID_TICKET = (uint64_t)k_EBeginAuthSessionResultInvalidTicket,
    BEGIN_AUTH_SESSION_RESULT_DUPLICATE_REQUEST = (uint64_t)k_EBeginAuthSessionResultDuplicateRequest, 
    BEGIN_AUTH_SESSION_RESULT_INVALID_VERSION = (uint64_t)k_EBeginAuthSessionResultInvalidVersion,
    BEGIN_AUTH_SESSION_RESULT_GAME_MISMATCH = (uint64_t)k_EBeginAuthSessionResultGameMismatch,
    BEGIN_AUTH_SESSION_RESULT_EXPIRED_TICKET = (uint64_t)k_EBeginAuthSessionResultExpiredTicket,

    AUTH_SESSION_RESPONSE_OK = (uint64_t) k_EAuthSessionResponseOK,
    AUTH_SESSION_RESPONSE_USER_NOT_CONNECTED_TO_STEAM = (uint64_t) k_EAuthSessionResponseUserNotConnectedToSteam,
    AUTH_SESSION_RESPONSE_NO_LICENSE_OR_EXPIRED = (uint64_t) k_EAuthSessionResponseNoLicenseOrExpired,
    AUTH_SESSION_RESPONSE_VAC_BANNED = (uint64_t) k_EAuthSessionResponseVACBanned,
    AUTH_SESSION_RESPONSE_LOGGED_IN_ELSEWHERE = (uint64_t) k_EAuthSessionResponseLoggedInElseWhere,
    AUTH_SESSION_RESPONSE_VAC_CHECK_TIMED_OUT = (uint64_t) k_EAuthSessionResponseVACCheckTimedOut,
    AUTH_SESSION_RESPONSE_AUTH_TICKET_CANCELED = (uint64_t) k_EAuthSessionResponseAuthTicketCanceled,
    AUTH_SESSION_RESPONSE_AUTH_TICKET_INVALID_ALREADY_USED = (uint64_t) k_EAuthSessionResponseAuthTicketInvalidAlreadyUsed,
    AUTH_SESSION_RESPONSE_AUTH_TICKET_INVALID = (uint64_t) k_EAuthSessionResponseAuthTicketInvalid,
    AUTH_SESSION_RESPONSE_PUBLISHER_ISSUED_BAN = (uint64_t) k_EAuthSessionResponsePublisherIssuedBan,
    AUTH_SESSION_RESPONSE_AUTH_TICKET_NETWORK_IDENTITY_FAILURE = (uint64_t) k_EAuthSessionResponseAuthTicketNetworkIdentityFailure,

    AUTH_TICKET_SIZE = 1024
  };

  static GodotSteamUser *get_singleton();
  static void reset_singleton();

  GodotSteamUser();
  ~GodotSteamUser();

  bool isSteamUserReady();
  uint64_t getSteamID();
  bool loggedOn();
  uint64_t getPlayerSteamLevel();
  String getUserDataFolder();
  void advertiseGame(const String &serverIp, uint64_t port);
  uint64_t getGameBadgeLevel(uint64_t series, bool foil);

  uint64_t beginAuthSession(PackedByteArray ticket, uint64_t ticketSize, uint64_t steamId);
  void cancelAuthTicket(uint64_t authTicket);
  void endAuthSession(uint64_t steamId);
  Dictionary getAuthSessionTicket();

protected:
  static void _bind_methods();
  static GodotSteamUser *singleton;

private:
  STEAM_CALLBACK(GodotSteamUser, _get_auth_session_ticket_response, GetAuthSessionTicketResponse_t, callbackGetAuthSessionTicketResponse);
  STEAM_CALLBACK(GodotSteamUser, _validate_auth_ticket_response, ValidateAuthTicketResponse_t, callbackValidateAuthTicketResponse);

  GDCLASS(GodotSteamUser, Object);
};
#endif // GODOTSTEAMUSER_H
