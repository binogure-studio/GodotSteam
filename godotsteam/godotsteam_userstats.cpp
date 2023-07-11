#include "godotsteam_userstats.h"

GodotSteamUserstats *GodotSteamUserstats::singleton = NULL;

GodotSteamUserstats::GodotSteamUserstats() { singleton = this; }
GodotSteamUserstats::~GodotSteamUserstats() { singleton = NULL; }

GodotSteamUserstats *GodotSteamUserstats::get_singleton() {
  if (GodotSteamUserstats::singleton == NULL) {
    GodotSteamUserstats::singleton = new GodotSteamUserstats();
  }

  return GodotSteamUserstats::singleton;
}

void GodotSteamUserstats::reset_singleton() {
  delete GodotSteamUserstats::singleton;

  GodotSteamUserstats::singleton = NULL;
}

bool GodotSteamUserstats::isSteamUserstatsReady() {
  return SteamUserStats() != NULL;
}

bool GodotSteamUserstats::clearAchievement(const String &s_key) {
  return SteamUserStats()->ClearAchievement(s_key.utf8().get_data());
}

bool GodotSteamUserstats::getAchievement(const String &s_key) {
  bool achieved = false;
  SteamUserStats()->GetAchievement(s_key.utf8().get_data(), &achieved);
  return achieved;
}

float GodotSteamUserstats::getStatFloat(const String &s_key) {
  float statval = 0;
  SteamUserStats()->GetStat(s_key.utf8().get_data(), &statval);
  return statval;
}

uint64_t GodotSteamUserstats::getStatInt(const String &s_key) {
  int32 statval = 0;
  SteamUserStats()->GetStat(s_key.utf8().get_data(), &statval);
  return statval;
}

String GodotSteamUserstats::getAchievementName(uint32 iAchievement) {
  return SteamUserStats()->GetAchievementName(iAchievement);
}

uint64_t GodotSteamUserstats::getNumAchievements() {
  return SteamUserStats()->GetNumAchievements();
}

bool GodotSteamUserstats::resetAllStats(bool bAchievementsToo) {
  SteamUserStats()->ResetAllStats(bAchievementsToo);
  return SteamUserStats()->StoreStats();
}

bool GodotSteamUserstats::requestCurrentStats() {
  return SteamUserStats()->RequestCurrentStats();
}

bool GodotSteamUserstats::setAchievement(const String &s_key) {
  STEAM_FAIL_COND_V(!isSteamUserstatsReady(), false);

  SteamUserStats()->SetAchievement(s_key.utf8().get_data());
  return SteamUserStats()->StoreStats();
}

bool GodotSteamUserstats::setStatFloat(const String &s_key, float value) {
  return SteamUserStats()->SetStat(s_key.utf8().get_data(), value);
}

bool GodotSteamUserstats::setStatInt(const String &s_key, int32 value) {
  return SteamUserStats()->SetStat(s_key.utf8().get_data(), value);
}

bool GodotSteamUserstats::storeStats() {
  STEAM_FAIL_COND_V(!isSteamUserstatsReady(), false);

  SteamUserStats()->StoreStats();
  return SteamUserStats()->RequestCurrentStats();
}

void GodotSteamUserstats::findLeaderboard(const String &name) {
  if (!isSteamUserstatsReady()) {
    emit_signal("leaderboard_load_failed", "Steam user stats not initialized");

    return;
  }

  SteamAPICall_t apiCall =
      SteamUserStats()->FindLeaderboard(name.utf8().get_data());
  callResultFindLeaderboard.Set(apiCall, this, &GodotSteamUserstats::OnFindLeaderboard);
}

void GodotSteamUserstats::OnFindLeaderboard(
    LeaderboardFindResult_t *pFindLeaderboardResult, bool bIOFailure) {
  if (!pFindLeaderboardResult->m_bLeaderboardFound || bIOFailure) {
    emit_signal("leaderboard_load_failed", "Leaderboard not found");

  } else {
    setLeaderboardHandle(pFindLeaderboardResult->m_hSteamLeaderboard);

    uint8 sLeaderboard = (uint8)pFindLeaderboardResult->m_hSteamLeaderboard;

    emit_signal("leaderboard_loaded", sLeaderboard);
  }
}

String GodotSteamUserstats::getLeaderboardName() {
  STEAM_FAIL_COND_V(!isSteamUserstatsReady(), "");

  return SteamUserStats()->GetLeaderboardName(leaderboardHandle);
}

uint64_t GodotSteamUserstats::getLeaderboardEntryCount() {
  STEAM_FAIL_COND_V(!isSteamUserstatsReady(), -1);

  return SteamUserStats()->GetLeaderboardEntryCount(leaderboardHandle);
}

void GodotSteamUserstats::downloadLeaderboardEntries(uint64_t rStart, uint64_t rEnd,
                                                     uint64_t type) {
  if (!isSteamUserstatsReady()) {
    emit_signal("leaderboard_entries_load_failed", "Steam user stats not initialized");

    return;
  }

  // load the specified leaderboard data. We only display
  // k_nMaxLeaderboardEntries entries at a time
  SteamAPICall_t apiCall = SteamUserStats()->DownloadLeaderboardEntries(
      leaderboardHandle, ELeaderboardDataRequest(type), rStart, rEnd);

  // Register for the async callback
  callResultEntries.Set(apiCall, this,
                        &GodotSteamUserstats::OnLeaderboardEntriesLoaded);
}

void GodotSteamUserstats::uploadLeaderboardScore(uint64_t score, bool keepBest) {
  if (!isSteamUserstatsReady()) {
    emit_signal("leaderboard_upload_failed", "Steam user stats not initialized");

    return;
  }

  SteamAPICall_t apiCall = SteamUserStats()->UploadLeaderboardScore(
      leaderboardHandle,
      (keepBest) ? k_ELeaderboardUploadScoreMethodKeepBest
                 : k_ELeaderboardUploadScoreMethodForceUpdate,
      (uint64_t)score, NULL, 0);
  callResultUploadScore.Set(apiCall, this, &GodotSteamUserstats::OnUploadScore);
}

void GodotSteamUserstats::OnUploadScore(LeaderboardScoreUploaded_t *callData,
                               bool bIOFailure) {
  // Incorrect leaderboard
  if (callData->m_hSteamLeaderboard != leaderboardHandle) {
    emit_signal("leaderboard_upload_failed", "Invalid leaderboard loaded");

    return;
  }

  if (bIOFailure) {
    emit_signal("leaderboard_upload_failed", "IO faillure");

    return;
  }

  if (callData->m_bSuccess != 1) {
    emit_signal("leaderboard_upload_failed", "Request failed");

    return;
  }

  emit_signal("leaderboard_uploaded", callData->m_nScore, callData->m_bScoreChanged,
              callData->m_nGlobalRankNew, callData->m_nGlobalRankPrevious);
}

void GodotSteamUserstats::OnLeaderboardEntriesLoaded(
    LeaderboardScoresDownloaded_t *callData, bool bIOFailure) {
  // Incorrect leaderboard
  if (callData->m_hSteamLeaderboard != leaderboardHandle) {
    return;
  }

  getDownloadedLeaderboardEntry(callData->m_hSteamLeaderboardEntries,
                                callData->m_cEntryCount);
  emit_signal("leaderboard_entries_loaded");
}

void GodotSteamUserstats::getDownloadedLeaderboardEntry(
    SteamLeaderboardEntries_t eHandle, uint64_t entryCount) {
  STEAM_FAIL_COND(!isSteamUserstatsReady());

  leaderboard_entries.clear();
  for (uint64_t index = 0; index < entryCount; index++) {
    LeaderboardEntry_t entry;

    SteamUserStats()->GetDownloadedLeaderboardEntry(eHandle, index, &entry,
                                                    NULL, 0);

    Dictionary entryDict;
    entryDict["score"] = entry.m_nScore;
    entryDict["name"] =
        SteamFriends()->GetFriendPersonaName(entry.m_steamIDUser);
    entryDict["steam_id"] = entry.m_steamIDUser.GetAccountID();
    entryDict["global_rank"] = entry.m_nGlobalRank;

    leaderboard_entries.append(entryDict);
  }
}

void GodotSteamUserstats::setLeaderboardHandle(SteamLeaderboard_t lHandle) {
  leaderboardHandle = (uint64)lHandle;
}

uint64 GodotSteamUserstats::getLeaderboardHandle() { return leaderboardHandle; }

Array GodotSteamUserstats::getLeaderboardEntries() {
  return leaderboard_entries;
}

Dictionary GodotSteamUserstats::getAchievementAndUnlockTime(const String &name) {
  Dictionary achieve;
  STEAM_FAIL_COND_V(!isSteamUserstatsReady(), achieve);

  bool achieved = false;
  uint32 unlockTime = 0;

  // Get the data from Steam
  bool retrieved = SteamUserStats()->GetAchievementAndUnlockTime(name.utf8().get_data(), &achieved, &unlockTime);

  if (retrieved) {
    achieve["retrieve"] = retrieved;
    achieve["achieved"] = achieved;
    achieve["unlocked"] = unlockTime;
  }

  return achieve;
}

bool GodotSteamUserstats::indicateAchievementProgress(const String &name,
                                                      uint64_t curProgress,
                                                      uint64_t maxProgress) {
  STEAM_FAIL_COND_V(!isSteamUserstatsReady(), 0);

  return SteamUserStats()->IndicateAchievementProgress(
      name.utf8().get_data(), curProgress, maxProgress);
}

void GodotSteamUserstats::_bind_methods() {
  ClassDB::bind_method("clearAchievement",
                            &GodotSteamUserstats::clearAchievement);
  ClassDB::bind_method("getAchievement",
                            &GodotSteamUserstats::getAchievement);
  ClassDB::bind_method("getStatFloat", &GodotSteamUserstats::getStatFloat);
  ClassDB::bind_method("getStatInt", &GodotSteamUserstats::getStatInt);
  ClassDB::bind_method("getAchievementName", &GodotSteamUserstats::getAchievementName);
  ClassDB::bind_method("getNumAchievements", &GodotSteamUserstats::getNumAchievements);
  ClassDB::bind_method("resetAllStats",
                            &GodotSteamUserstats::resetAllStats);
  ClassDB::bind_method("requestCurrentStats",
                            &GodotSteamUserstats::requestCurrentStats);
  ClassDB::bind_method("setAchievement",
                            &GodotSteamUserstats::setAchievement);
  ClassDB::bind_method("setStatFloat", &GodotSteamUserstats::setStatFloat);
  ClassDB::bind_method("setStatInt", &GodotSteamUserstats::setStatInt);
  ClassDB::bind_method("storeStats", &GodotSteamUserstats::storeStats);
  ClassDB::bind_method(D_METHOD("findLeaderboard", "name"),
                            &GodotSteamUserstats::findLeaderboard);
  ClassDB::bind_method("getLeaderboardName",
                            &GodotSteamUserstats::getLeaderboardName);
  ClassDB::bind_method("getLeaderboardEntryCount",
                            &GodotSteamUserstats::getLeaderboardEntryCount);
  ClassDB::bind_method(
      D_METHOD("downloadLeaderboardEntries", "range_start", "range_end", "type"),
      &GodotSteamUserstats::downloadLeaderboardEntries, DEFVAL(LEADERBOARD_GLOBAL));
  ClassDB::bind_method(D_METHOD("uploadLeaderboardScore", "score", "keep_best"),
                            &GodotSteamUserstats::uploadLeaderboardScore,
                            DEFVAL(true));
  ClassDB::bind_method("getLeaderboardEntries",
                            &GodotSteamUserstats::getLeaderboardEntries);
  ClassDB::bind_method("getAchievementAndUnlockTime",
                            &GodotSteamUserstats::getAchievementAndUnlockTime);
  ClassDB::bind_method("indicateAchievementProgress",
                            &GodotSteamUserstats::indicateAchievementProgress);

  ADD_SIGNAL(MethodInfo("leaderboard_loaded", PropertyInfo(Variant::INT, "leaderboard_name")));
  ADD_SIGNAL(MethodInfo("leaderboard_load_failed", PropertyInfo(Variant::STRING, "reason")));

  ADD_SIGNAL(MethodInfo("leaderboard_entries_loaded"));
  ADD_SIGNAL(MethodInfo("leaderboard_entries_load_failed", PropertyInfo(Variant::STRING, "reason")));

  ADD_SIGNAL(MethodInfo("leaderboard_upload_failed", PropertyInfo(Variant::STRING, "reason")));
  ADD_SIGNAL(MethodInfo("leaderboard_uploaded", PropertyInfo(Variant::INT, "score"), PropertyInfo(Variant::BOOL, "score_changed"),
          PropertyInfo(Variant::INT, "global_rank_new"), PropertyInfo(Variant::INT, "global_rank_previous")));

  BIND_CONSTANT(LEADERBOARD_GLOBAL);
  BIND_CONSTANT(LEADERBOARD_AROUND_USER);
  BIND_CONSTANT(LEADERBOARD_FRIENDS);
}
