#ifndef GODOTSTEAMUSERSTATS_H
#define GODOTSTEAMUSERSTATS_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/io/ip.h"
#include "core/io/ip_address.h"
#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/variant/dictionary.h"
#include "scene/resources/texture.h"

#include "godotsteam_utils.h"

class GodotSteamUserstats: public Object
{
public:
  enum {
    LEADERBOARD_GLOBAL = (uint64_t)k_ELeaderboardDataRequestGlobal,
    LEADERBOARD_AROUND_USER = (uint64_t)k_ELeaderboardDataRequestGlobalAroundUser,
    LEADERBOARD_FRIENDS = (uint64_t)k_ELeaderboardDataRequestFriends
  };

  static GodotSteamUserstats *get_singleton();
  static void reset_singleton();

  GodotSteamUserstats();
  ~GodotSteamUserstats();

  bool isSteamUserstatsReady();
  bool clearAchievement(const String &s_key);
  bool getAchievement(const String &s_key);
  float getStatFloat(const String &s_key);
  uint64_t getStatInt(const String &s_key);
  bool resetAllStats(bool bAchievementsToo = true);
  bool requestCurrentStats();
  bool setAchievement(const String &s_key);
  bool setStatFloat(const String &s_key, float value);
  bool setStatInt(const String &s_key, int32 value);
  bool storeStats();
  void findLeaderboard(const String &lName);
  String getLeaderboardName();
  String getAchievementName(uint32 iAchievement);
  uint64_t getNumAchievements();
  uint64_t getLeaderboardEntryCount();
  void downloadLeaderboardEntries(uint64_t rStart, uint64_t rEnd, uint64_t type = LEADERBOARD_GLOBAL);
  void downloadLeaderboardEntriesForUsers(Array usersID);
  void uploadLeaderboardScore(uint64_t score, bool keepBest = false);
  void getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t eHandle, uint64_t entryCount);
  void setLeaderboardHandle(uint64 lHandle);
  uint64 getLeaderboardHandle();
  Array getLeaderboardEntries();
  Dictionary getAchievementAndUnlockTime(const String &name);
  bool indicateAchievementProgress(const String &name, uint64_t curProgress, uint64_t maxProgress);

protected:
  static void _bind_methods();
  static GodotSteamUserstats *singleton;

private:
  SteamLeaderboard_t leaderboardHandle;
  Array leaderboard_entries;

  void OnFindLeaderboard(LeaderboardFindResult_t *pFindLearderboardResult, bool bIOFailure);
  CCallResult<GodotSteamUserstats, LeaderboardFindResult_t> callResultFindLeaderboard;

  void OnUploadScore(LeaderboardScoreUploaded_t *pFindLearderboardResult, bool bIOFailure);
  CCallResult<GodotSteamUserstats, LeaderboardScoreUploaded_t> callResultUploadScore;

  void OnLeaderboardEntriesLoaded(LeaderboardScoresDownloaded_t *callData, bool bIOFailure);
  CCallResult<GodotSteamUserstats, LeaderboardScoresDownloaded_t> callResultEntries;

  GDCLASS(GodotSteamUserstats, Object);
};
#endif // GODOTSTEAMUSERSTATS_H
