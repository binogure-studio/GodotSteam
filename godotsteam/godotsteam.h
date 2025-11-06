#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/io/ip.h"
#include "core/io/ip_address.h"
#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/variant/dictionary.h"
#include "scene/resources/texture.h"

// Local dependencies
#include "godotsteam_friends.h"
#include "godotsteam_utils.h"

class GodotSteam : public Object
{
public:
  enum {
    STEAM_INIT_NO_CLIENT_ERROR,
    STEAM_INIT_API_NOT_INIT_ERROR,
    STEAM_INIT_NO_CONNECTION_ERROR,
    STEAM_INIT_OK,

    STEAM_RESULT_NONE = (uint64_t)k_EResultNone,
    STEAM_RESULT_OK = (uint64_t)k_EResultOK,
    STEAM_RESULT_FAIL = (uint64_t)k_EResultFail,
    STEAM_RESULT_NO_CONNECTION = (uint64_t)k_EResultNoConnection,
    STEAM_RESULT_NO_CONNECTION_RETRY = (uint64_t)4, // k_EResultNoConnectionRetry
    STEAM_RESULT_INVALID_PASSWORD = (uint64_t)k_EResultInvalidPassword,
    STEAM_RESULT_LOGGED_IN_ELSE_WHERE = (uint64_t)k_EResultLoggedInElsewhere,
    STEAM_RESULT_INVALID_PROTOCOL_VER = (uint64_t)k_EResultInvalidProtocolVer,
    STEAM_RESULT_INVALID_PARAM = (uint64_t)k_EResultInvalidParam,
    STEAM_RESULT_FILE_NOT_FOUND = (uint64_t)k_EResultFileNotFound,
    STEAM_RESULT_BUSY = (uint64_t)k_EResultBusy,
    STEAM_RESULT_INVALID_STATE = (uint64_t)k_EResultInvalidState,
    STEAM_RESULT_INVALID_NAME = (uint64_t)k_EResultInvalidName,
    STEAM_RESULT_INVALID_EMAIL = (uint64_t)k_EResultInvalidEmail,
    STEAM_RESULT_DUPLICATEN_AME = (uint64_t)k_EResultDuplicateName,
    STEAM_RESULT_ACCESS_DENIED = (uint64_t)k_EResultAccessDenied,
    STEAM_RESULT_TIMEOUT = (uint64_t)k_EResultTimeout,
    STEAM_RESULT_BANNED = (uint64_t)k_EResultBanned,
    STEAM_RESULT_ACCOUNT_NOT_FOUND = (uint64_t)k_EResultAccountNotFound,
    STEAM_RESULT_INVALID_STEAM_ID = (uint64_t)k_EResultInvalidSteamID,
    STEAM_RESULT_SERVICE_UNAVAILABLE = (uint64_t)k_EResultServiceUnavailable,
    STEAM_RESULT_NOT_LOGGED_ON = (uint64_t)k_EResultNotLoggedOn,
    STEAM_RESULT_PENDING = (uint64_t)k_EResultPending,
    STEAM_RESULT_ENCRYPTION_FAILURE = (uint64_t)k_EResultEncryptionFailure,
    STEAM_RESULT_INSUFFICIENT_PRIVILEGE = (uint64_t)k_EResultInsufficientPrivilege,
    STEAM_RESULT_LIMIT_EXCEEDED = (uint64_t)k_EResultLimitExceeded,
    STEAM_RESULT_REVOKED = (uint64_t)k_EResultRevoked,
    STEAM_RESULT_EXPIRED = (uint64_t)k_EResultExpired,
    STEAM_RESULT_ALREADY_REDEEMED = (uint64_t)k_EResultAlreadyRedeemed,
    STEAM_RESULT_DUPLICATE_REQUEST = (uint64_t)k_EResultDuplicateRequest,
    STEAM_RESULT_ALREADY_OWNED = (uint64_t)k_EResultAlreadyOwned,
    STEAM_RESULT_IP_NOT_FOUND = (uint64_t)k_EResultIPNotFound,
    STEAM_RESULT_PERSIST_FAILED = (uint64_t)k_EResultPersistFailed,
    STEAM_RESULT_LOCKING_FAILED = (uint64_t)k_EResultLockingFailed,
    STEAM_RESULT_LOGON_SESSION_REPLACED = (uint64_t)k_EResultLogonSessionReplaced,
    STEAM_RESULT_CONNECT_FAILED = (uint64_t)k_EResultConnectFailed,
    STEAM_RESULT_HANDSHAKE_FAILED = (uint64_t)k_EResultHandshakeFailed,
    STEAM_RESULT_IO_FAILURE = (uint64_t)k_EResultIOFailure,
    STEAM_RESULT_REMOTE_DISCONNECT = (uint64_t)k_EResultRemoteDisconnect,
    STEAM_RESULT_SHOPPING_CART_NOT_FOUND = (uint64_t)k_EResultShoppingCartNotFound,
    STEAM_RESULT_BLOCKED = (uint64_t)k_EResultBlocked,
    STEAM_RESULT_IGNORED = (uint64_t)k_EResultIgnored,
    STEAM_RESULT_NO_MATCH = (uint64_t)k_EResultNoMatch,
    STEAM_RESULT_ACCOUNT_DISABLED = (uint64_t)k_EResultAccountDisabled,
    STEAM_RESULT_SERVICE_READ_ONLY = (uint64_t)k_EResultServiceReadOnly,
    STEAM_RESULT_ACCOUNT_NOT_FEATURED = (uint64_t)k_EResultAccountNotFeatured,
    STEAM_RESULT_ADMINISTRATOR_OK = (uint64_t)k_EResultAdministratorOK,
    STEAM_RESULT_CONTENT_VERSION = (uint64_t)k_EResultContentVersion,
    STEAM_RESULT_TRY_ANOTHER_CM = (uint64_t)k_EResultTryAnotherCM,
    STEAM_RESULT_PASSWORD_REQUIRED_TO_KICK_SESSION = (uint64_t)k_EResultPasswordRequiredToKickSession,
    STEAM_RESULT_ALREADY_LOGGED_IN_ELSE_WHERE = (uint64_t)k_EResultAlreadyLoggedInElsewhere,
    STEAM_RESULT_SUSPENDED = (uint64_t)k_EResultSuspended,
    STEAM_RESULT_CANCELLED = (uint64_t)k_EResultCancelled,
    STEAM_RESULT_DATA_CORRUPTION = (uint64_t)k_EResultDataCorruption,
    STEAM_RESULT_DISK_FULL = (uint64_t)k_EResultDiskFull,
    STEAM_RESULT_REMOTE_CALL_FAILED = (uint64_t)k_EResultRemoteCallFailed,
    STEAM_RESULT_PASSWORD_UNSET = (uint64_t)k_EResultPasswordUnset,
    STEAM_RESULT_EXTERNAL_ACCOUNT_UNLINKED = (uint64_t)k_EResultExternalAccountUnlinked,
    STEAM_RESULT_PSN_TICKET_INVALID = (uint64_t)k_EResultPSNTicketInvalid,
    STEAM_RESULT_EXTERNAL_ACCOUNT_ALREADY_LINKED = (uint64_t)k_EResultExternalAccountAlreadyLinked,
    STEAM_RESULT_REMOTE_FILE_CONFLICT = (uint64_t)k_EResultRemoteFileConflict,
    STEAM_RESULT_ILLEGAL_PASSWORD = (uint64_t)k_EResultIllegalPassword,
    STEAM_RESULT_SAME_AS_PREVIOUS_VALUE = (uint64_t)k_EResultSameAsPreviousValue,
    STEAM_RESULT_ACCOUNT_LOG_ON_DENIED = (uint64_t)k_EResultAccountLogonDenied,
    STEAM_RESULT_CANNOT_USE_OLD_PASSWORD = (uint64_t)k_EResultCannotUseOldPassword,
    STEAM_RESULT_INVALID_LOGIN_AUTH_CODE = (uint64_t)k_EResultInvalidLoginAuthCode,
    STEAM_RESULT_ACCOUNT_LOG_ON_DENIED_NO_MAIL = (uint64_t)k_EResultAccountLogonDeniedNoMail,
    STEAM_RESULT_HARDWARE_NOT_CAPABLE_OF_IPT = (uint64_t)k_EResultHardwareNotCapableOfIPT,
    STEAM_RESULT_IPT_INIT_ERROR = (uint64_t)k_EResultIPTInitError,
    STEAM_RESULT_PARENTAL_CONTROL_RESTRICTED = (uint64_t)k_EResultParentalControlRestricted,
    STEAM_RESULT_FACEBOOK_QUERY_ERROR = (uint64_t)k_EResultFacebookQueryError,
    STEAM_RESULT_EXPIRED_LOGIN_AUTH_CODE = (uint64_t)k_EResultExpiredLoginAuthCode,
    STEAM_RESULT_IP_LOGIN_RESTRICTION_FAILED = (uint64_t)k_EResultIPLoginRestrictionFailed,
    STEAM_RESULT_ACCOUNT_LOCKED_DOWN = (uint64_t)k_EResultAccountLockedDown,
    STEAM_RESULT_ACCOUNT_LOGON_DENIED_VERIFIED_EMAIL_REQUIRED = (uint64_t)k_EResultAccountLogonDeniedVerifiedEmailRequired,
    STEAM_RESULT_NO_MATCHING_URL = (uint64_t)k_EResultNoMatchingURL,
    STEAM_RESULT_BAD_RESPONSE = (uint64_t)k_EResultBadResponse,
    STEAM_RESULT_REQUIRE_PASSWORD_REENTRY = (uint64_t)k_EResultRequirePasswordReEntry,
    STEAM_RESULT_VALUE_OUT_OF_RANGE = (uint64_t)k_EResultValueOutOfRange,
    STEAM_RESULT_UNEXPECTED_ERROR = (uint64_t)k_EResultUnexpectedError,
    STEAM_RESULT_DISABLED = (uint64_t)k_EResultDisabled,
    STEAM_RESULT_INVALID_CEG_SUBMISSION = (uint64_t)k_EResultInvalidCEGSubmission,
    STEAM_RESULT_RESTRICTED_DEVICE = (uint64_t)k_EResultRestrictedDevice,
    STEAM_RESULT_REGION_LOCKED = (uint64_t)k_EResultRegionLocked,
    STEAM_RESULT_RATE_LIMIT_EXCEEDED = (uint64_t)k_EResultRateLimitExceeded,
    STEAM_RESULT_ACCOUNT_LOGIN_DENIED_NEED_TWO_FACTOR = (uint64_t)k_EResultAccountLoginDeniedNeedTwoFactor,
    STEAM_RESULT_ITEM_DELETED = (uint64_t)k_EResultItemDeleted,
    STEAM_RESULT_ACCOUNT_LOGIN_DENIED_THROTTLE = (uint64_t)k_EResultAccountLoginDeniedThrottle,
    STEAM_RESULT_TWO_FACTOR_CODE_MISMATCH = (uint64_t)k_EResultTwoFactorCodeMismatch,
    STEAM_RESULT_TWO_FACTOR_ACTIVATION_CODE_MISMATCH = (uint64_t)k_EResultTwoFactorActivationCodeMismatch,
    STEAM_RESULT_ACCOUNT_ASSOCIATED_TO_MULTIPLE_PARTNERS = (uint64_t)k_EResultAccountAssociatedToMultiplePartners,
    STEAM_RESULT_NOT_MODIFIED = (uint64_t)k_EResultNotModified,
    STEAM_RESULT_NO_MOBILE_DEVICE = (uint64_t)k_EResultNoMobileDevice,
    STEAM_RESULT_TIME_NOT_SYNCED = (uint64_t)k_EResultTimeNotSynced,
    STEAM_RESULT_SMS_CODE_FAILED = (uint64_t)k_EResultSmsCodeFailed,
    STEAM_RESULT_ACCOUNT_LIMIT_EXCEEDED = (uint64_t)k_EResultAccountLimitExceeded,
    STEAM_RESULT_ACCOUNT_ACTIVITY_LIMIT_EXCEEDED = (uint64_t)k_EResultAccountActivityLimitExceeded,
    STEAM_RESULT_PHONE_ACTIVITY_LIMIT_EXCEEDED = (uint64_t)k_EResultPhoneActivityLimitExceeded,
    STEAM_RESULT_REFUND_TO_WALLET = (uint64_t)k_EResultRefundToWallet,
    STEAM_RESULT_EMAIL_SEND_FAILURE = (uint64_t)k_EResultEmailSendFailure,
    STEAM_RESULT_NOT_SETTLED = (uint64_t)k_EResultNotSettled,
    STEAM_RESULT_NEED_CAPTCHA = (uint64_t)k_EResultNeedCaptcha,
    STEAM_RESULT_GSLT_DENIED = (uint64_t)k_EResultGSLTDenied,
    STEAM_RESULT_GS_OWNER_DENIED = (uint64_t)k_EResultGSOwnerDenied,
    STEAM_RESULT_INVALID_ITEM_TYPE = (uint64_t)k_EResultInvalidItemType,
    STEAM_RESULT_IP_BANNED = (uint64_t)k_EResultIPBanned,
    STEAM_RESULT_GSLT_EXPIRED = (uint64_t)k_EResultGSLTExpired,
    STEAM_RESULT_INSUFFICIENT_FUNDS = (uint64_t)k_EResultInsufficientFunds,
    STEAM_RESULT_TOO_MANY_PENDING = (uint64_t)k_EResultTooManyPending,
    STEAM_RESULT_NO_SITE_LICENSES_FOUND = (uint64_t)k_EResultNoSiteLicensesFound,
    STEAM_RESULT_WG_NETWORK_SEND_EXCEEDED = (uint64_t)k_EResultWGNetworkSendExceeded,
    STEAM_RESULT_ACCOUNT_NOT_FRIENDS = (uint64_t)k_EResultAccountNotFriends,
    STEAM_RESULT_LIMITED_USER_ACCOUNT = (uint64_t)k_EResultLimitedUserAccount,
    STEAM_RESULT_CANT_REMOVE_ITEM = (uint64_t)k_EResultCantRemoveItem,
    STEAM_RESULT_ACCOUNT_DELETED = (uint64_t)k_EResultAccountDeleted,
    STEAM_RESULT_EXISTING_USER_CANCELLED_LICENSE = (uint64_t)k_EResultExistingUserCancelledLicense,
    STEAM_RESULT_COMMUNITY_COOLDOWN = (uint64_t)k_EResultCommunityCooldown,
    STEAM_RESULT_NO_LAUNCHER_SPECIFIED = (uint64_t)k_EResultNoLauncherSpecified,
    STEAM_RESULT_MUST_AGREE_TO_SSA = (uint64_t)k_EResultMustAgreeToSSA,
    STEAM_RESULT_LAUNCHER_MIGRATED = (uint64_t)k_EResultLauncherMigrated,
    STEAM_RESULT_STEAM_REAL_MMISMATCH = (uint64_t)k_EResultSteamRealmMismatch,
    STEAM_RESULT_INVALID_SIGNATURE = (uint64_t)k_EResultInvalidSignature,
    STEAM_RESULT_PARSE_FAILURE = (uint64_t)k_EResultParseFailure,
    STEAM_RESULT_NO_VERIFIED_PHONE = (uint64_t)k_EResultNoVerifiedPhone,
    STEAM_RESULT_IN_SUFFICIENT_BATTERY = (uint64_t)k_EResultInsufficientBattery,
    STEAM_RESULT_CHARGER_REQUIRED = (uint64_t)k_EResultChargerRequired,
    STEAM_RESULT_CACHED_CREDENTIAL_INVALID = (uint64_t)k_EResultCachedCredentialInvalid,
    STEAM_RESULT_PHONE_NUMBER_IS_VOIP = (uint64_t)K_EResultPhoneNumberIsVOIP
  };

  static GodotSteam *get_singleton();
  static void reset_singleton();
  GodotSteam();
  ~GodotSteam();

  bool isSteamAppReady();
  bool restartAppIfNecessary(uint64_t value);
  uint64_t steamInit(uint64_t appID, bool force);
  bool isSteamRunning();
  bool hasOtherApp(uint64_t value);
  uint64_t getDLCCount();
  bool isDLCInstalled(uint64_t value);
  void requestAppProofOfPurchaseKey(uint64_t value);
  bool isAppInstalled(uint64_t value);
  String getCurrentBetaName();
  String getCurrentGameLanguage();
  bool isVACBanned();
  uint64_t getEarliestPurchaseUnixTime(uint64_t value);
  bool isSubscribedFromFreeWeekend();
  void installDLC(uint64_t value);
  void uninstallDLC(uint64_t value);

protected:
  static void _bind_methods();
  static GodotSteam *singleton;

private:
  bool isInitSuccess;

  STEAM_CALLBACK(GodotSteam, _overlay_toggled, GameOverlayActivated_t, callbackOverlayToggled);
  STEAM_CALLBACK(GodotSteam, _low_power, LowBatteryPower_t, callbackLowPower);
  STEAM_CALLBACK(GodotSteam, _dlc_installed, DlcInstalled_t, callbackDLCInstalled);

  void run_callbacks() {
    SteamAPI_RunCallbacks();
  }

  void close_steam() {
    SteamAPI_Shutdown();
  }

  GDCLASS(GodotSteam, Object);
};
#endif // GODOTSTEAM_H
