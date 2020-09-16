#ifndef GODOTSTEAMWORKSHOP_H
#define GODOTSTEAMWORKSHOP_H

#include <steam/steam_api.h>

#include "object.h"

class GodotSteamWorkshop: public Object
{
public:
  enum {
    DESCRIPTION_MAX_LENGTH = 255,
    QUERY_MAX_LENGTH = 256
  };

  static GodotSteamWorkshop *get_singleton();
  static void reset_singleton();

  GodotSteamWorkshop();
  ~GodotSteamWorkshop();

  bool isSteamUGCReady();

  void addAppDependency(int publishedFileID, int appID);
  void addDependency(int publishedFileID, int childPublishedFileID);
  bool addExcludedTag(int queryHandle, const String& tagName);
  bool addItemKeyValueTag(int updateHandle, const String& key, const String& value);
  bool addItemPreviewFile(int queryHandle, const String& previewFile, int type);
  bool addItemPreviewVideo(int queryHandle, const String& videoID);
  void addItemToFavorite(int appID, int publishedFileID);
  bool addRequiredKeyValueTag(int queryHandle, const String& key, const String& value);
  bool addRequiredTag(int queryHandle, const String& tagName);

  void createItem(AppId_t appID, int fileType);
  int createQueryAllUGCRequest(int queryType, int matchingType, int creatorID, int consumerID, int page);
  int createQueryUGCDetailsRequest(Array publishedFileIDs);

  void deleteItem(int publishedFileID);
  bool downloadItem(int nPublishedFileID, bool bHighPriority);

  Dictionary getItemDownloadInfo(int publishedFileID);
  int getNumSubscribedItems();
  int getItemState(int publishedFileID);
  Dictionary getQueryUGCAdditionalPreview(int queryHandle, int index, int previewIndex);
  Dictionary getQueryUGCChildren(int queryHandle, int index);
  Dictionary getQueryUGCKeyValueTag(int queryHandle, int index, int keyValueTagIndex);
  String getQueryUGCMetadata(int queryHandle, int index);
  int getQueryUGCNumAdditionalPreviews(int queryHandle, int index);
  int getQueryUGCNumKeyValueTags(int queryHandle, int index);
  String getQueryUGCPreviewURL(int queryHandle, int index);
  Dictionary getQueryUGCResult(int queryHandle, int index);
  Dictionary getQueryUGCStatistic(int queryHandle, int index, int statType);
  Dictionary getItemInstallInfo(int publishedFileID);
  Dictionary getItemUpdateProgress(int updateHandle);
  Array getSubscribedItems();

  bool initWorkshopForGameServer(int workshopDepotID);

  void removeAppDependency(int publishedFileID, int appID);
  void removeDependency(int publishedFileID, int childPublishedFileID);
  void removeItemFromFavorites(int appID, int publishedFileID);
  bool removeItemKeyValueTags(int updateHandle, const String& key);
  bool removeItemPreview(int updateHandle, int index);

  bool setItemContent(int updateHandle, const String& contentFolder);
  bool setItemDescription(int updateHandle, const String& description);
  bool setItemMetadata(int updateHandle, const String& metadata);
  bool setItemPreview(int updateHandle, const String& previewFile);
  bool setItemTags(int updateHandle, Array tagArray);
  bool setItemTitle(int updateHandle, const String& title);
  bool setItemUpdateLanguage(int updateHandle, const String& language);
  bool setItemVisibility(int updateHandle, int visibility);
  bool setLanguage(int queryHandle, const String& language);
  void suspendDownloads(bool bSuspend);

protected:
  static void _bind_methods();
  static GodotSteamWorkshop *singleton;

private:
  CCallResult<GodotSteamWorkshop, AddAppDependencyResult_t> callResultAddAppDependency;
  void _add_app_dependency_result(AddAppDependencyResult_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, AddUGCDependencyResult_t> callResultAddUGCDependency;
  void _add_ugc_dependency_result(AddUGCDependencyResult_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, CreateItemResult_t> callResultItemCreate;
  void _item_created(CreateItemResult_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, GetAppDependenciesResult_t> callResultGetAppDependencies;
  void _get_app_dependencies_result(GetAppDependenciesResult_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, DeleteItemResult_t> callResultDeleteItem;
  void _item_deleted(DeleteItemResult_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, GetUserItemVoteResult_t> callResultGetUserItemVote;
  void _get_item_vote_result(GetUserItemVoteResult_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, RemoveAppDependencyResult_t> callResultRemoveAppDependency;
  void _remove_app_dependency_result(RemoveAppDependencyResult_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, RemoveUGCDependencyResult_t> callResultRemoveUGCDependency;
  void _remove_ugc_dependency_result(RemoveUGCDependencyResult_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, SetUserItemVoteResult_t> callResultSetUserItemVote;
  void _set_user_item_vote(SetUserItemVoteResult_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, StartPlaytimeTrackingResult_t> callResultStartPlaytimeTracking;
  void _start_playtime_tracking(StartPlaytimeTrackingResult_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, SteamUGCQueryCompleted_t> callResultUGCQueryCompleted;
  void _ugc_query_completed(SteamUGCQueryCompleted_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, StopPlaytimeTrackingResult_t> callResultStopPlaytimeTracking;
  void _stop_playtime_tracking(StopPlaytimeTrackingResult_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, SubmitItemUpdateResult_t> callResultItemUpdate;
  void _item_updated(SubmitItemUpdateResult_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, UserFavoriteItemsListChanged_t> callResultFavoriteItemListChanged;
  void _user_favorite_items_list_changed(UserFavoriteItemsListChanged_t *callData, bool bIOFailure);

  STEAM_CALLBACK(GodotSteamWorkshop, _item_downloaded, DownloadItemResult_t);
  STEAM_CALLBACK(GodotSteamWorkshop, _item_installed, ItemInstalled_t);

  OBJ_TYPE(GodotSteamWorkshop, Object);
  OBJ_CATEGORY("GodotSteamWorkshop");
};
#endif // GODOTSTEAMWORKSHOP_H
