#ifndef GODOTSTEAMUSERSTATS_H
#define GODOTSTEAMUSERSTATS_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/io/ip.h"
#include "core/io/ip_address.h"
#include "dictionary.h"
#include "object.h"
#include "reference.h"
#include "scene/resources/texture.h"

class GodotSteamUserstats: public Object
{
public:
  enum {
    LEADERBOARD_GLOBAL = (int)k_ELeaderboardDataRequestGlobal,
    LEADERBOARD_AROUND_USER = (int)k_ELeaderboardDataRequestGlobalAroundUser,
    LEADERBOARD_FRIENDS = (int)k_ELeaderboardDataRequestFriends
  };

  static GodotSteamUserstats *get_singleton();
  static void reset_singleton();

  GodotSteamUserstats();
  ~GodotSteamUserstats();

  bool isSteamUserstatsReady();
  bool clearAchievement(const String &s_key);
  bool getAchievement(const String &s_key);
  float getStatFloat(const String &s_key);
  int getStatInt(const String &s_key);
  bool resetAllStats(bool bAchievementsToo = true);
  bool requestCurrentStats();
  bool setAchievement(const String &s_key);
  bool setStatFloat(const String &s_key, float value);
  bool setStatInt(const String &s_key, int value);
  bool storeStats();
  void findLeaderboard(const String &lName);
  String getLeaderboardName();
  String getAchievementName(uint32 iAchievement);
  int getNumAchievements();
  int getLeaderboardEntryCount();
  void downloadLeaderboardEntries(int rStart, int rEnd, int type = LEADERBOARD_GLOBAL);
  void downloadLeaderboardEntriesForUsers(Array usersID);
  void uploadLeaderboardScore(int score, bool keepBest = false);
  void getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t eHandle, int entryCount);
  void setLeaderboardHandle(uint64 lHandle);
  uint64 getLeaderboardHandle();
  Array getLeaderboardEntries();
  bool getAchievementAndUnlockTime(const String &name, bool achieved, int unlockTime);
  bool indicateAchievementProgress(const String &name, int curProgress, int maxProgress);

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


  OBJ_TYPE(GodotSteamUserstats, Object);
  OBJ_CATEGORY("GodotSteamUserstats");
};
#endif // GODOTSTEAMUSERSTATS_H
