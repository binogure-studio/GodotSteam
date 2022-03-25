#ifndef GODOTSTEAMWORKSHOP_H
#define GODOTSTEAMWORKSHOP_H

#include <steam/steam_api.h>

#include "core/object/object.h"
#include "core/object/ref_counted.h"

#include "godotsteam_utils.h"

class GodotSteamWorkshop: public Object
{
public:
  static GodotSteamWorkshop *get_singleton();
  static void reset_singleton();

  GodotSteamWorkshop();
  ~GodotSteamWorkshop();

  bool isSteamUGCReady();

  void addAppDependency(uint64_t publishedFileID, uint32 appID);
  void addDependency(uint64_t publishedFileID, uint64_t childPublishedFileID);
  bool addExcludedTag(uint64_t queryHandle, const String& tagName);
  bool addItemKeyValueTag(uint64_t updateHandle, const String& key, const String& value);
  bool addItemPreviewFile(uint64_t queryHandle, const String& previewFile, uint64_t type);
  bool addItemPreviewVideo(uint64_t queryHandle, const String& videoID);
  void addItemToFavorite(uint32 appID, uint64_t publishedFileID);
  bool addRequiredKeyValueTag(uint64_t queryHandle, const String& key, const String& value);
  bool addRequiredTag(uint64_t queryHandle, const String& tagName);

  void commitItemUpdate(uint64_t updateHandle, const String& note);
  void createItem(uint32 appID, uint64_t fileType);
  void createQueryAllUGCRequest(uint64_t queryType, uint64_t matchingType, uint32 creatorID, uint64_t page);
  void createQueryUGCDetailsRequest(Array publishedFileIDs);
  void createQueryUserUGCRequest(AccountID_t accountID, uint64_t listType, uint32 creatorID, uint32 page);

  void deleteItem(uint64_t publishedFileID);
  bool downloadItem(uint64_t nPublishedFileID, bool bHighPriority);

  Dictionary getItemDownloadInfo(uint64_t publishedFileID);
  uint64_t getNumSubscribedItems();
  uint64_t getItemState(uint64_t publishedFileID);
  Dictionary getQueryUGCAdditionalPreview(uint64_t queryHandle, uint64_t index, uint64_t previewIndex);
  Dictionary getQueryUGCChildren(uint64_t queryHandle, uint64_t index, uint64_t numChildren);
  Dictionary getQueryUGCKeyValueTag(uint64_t queryHandle, uint64_t index, uint64_t keyValueTagIndex);
  String getQueryUGCMetadata(uint64_t queryHandle, uint64_t index);
  uint64_t getQueryUGCNumAdditionalPreviews(uint64_t queryHandle, uint64_t index);
  uint64_t getQueryUGCNumKeyValueTags(uint64_t queryHandle, uint64_t index);
  String getQueryUGCPreviewURL(uint64_t queryHandle, uint64_t index);
  Dictionary getQueryUGCResult(uint64_t queryHandle, uint64_t index);
  Dictionary getQueryUGCStatistic(uint64_t queryHandle, uint64_t index, uint64_t statType);
  Dictionary getItemInstallInfo(uint64_t publishedFileID);
  Dictionary getItemUpdateProgress(uint64_t updateHandle);
  Array getSubscribedItems();

  bool initWorkshopForGameServer(DepotId_t workshopDepotID);

  bool releaseQueryUGCRequest(uint64_t queryHandle);

  void removeAppDependency(uint64_t publishedFileID, uint32 appID);
  void removeDependency(uint64_t publishedFileID, uint64_t childPublishedFileID);
  void removeItemFromFavorites(uint32 appID, uint64_t publishedFileID);
  bool removeItemKeyValueTags(uint64_t updateHandle, const String& key);
  bool removeItemPreview(uint64_t updateHandle, uint64_t index);

  bool setItemContent(uint64_t updateHandle, const String& contentFolder);
  bool setItemDescription(uint64_t updateHandle, const String& description);
  bool setItemMetadata(uint64_t updateHandle, const String& metadata);
  bool setItemPreview(uint64_t updateHandle, const String& previewFile);
  bool setItemTags(uint64_t updateHandle, Array tagArray);
  bool setItemTitle(uint64_t updateHandle, const String& title);
  bool setItemUpdateLanguage(uint64_t updateHandle, const String& language);
  bool setItemVisibility(uint64_t updateHandle, uint64_t visibility);
  bool setLanguage(uint64_t queryHandle, const String& language);

  uint64_t startItemUpdate(uint32 appID, uint64_t publishedFileID);

  void subscribeItem(uint64_t publishedFileID);
  void suspendDownloads(bool bSuspend);

  bool updateItemPreviewFile(uint64_t updateHandle, uint64_t index, const String& previewFile);
  void unsubscribeItem(uint64_t publishedFileID);

protected:
  static void _bind_methods();
  static GodotSteamWorkshop *singleton;

private:
  CCallResult<GodotSteamWorkshop, AddAppDependencyResult_t> callResultAddAppDependency;
  void _add_app_dependency_result(AddAppDependencyResult_t *callData, bool bIOFailure);

  CCallResult<GodotSteamWorkshop, AddUGCDependencyResult_t> callResultAddUGCDependency;
  void _add_ugc_dependency_result(AddUGCDependencyResult_t *callData, bool bIOFailure);

  CCallResult<GodotSteamWorkshop, CreateItemResult_t> callResultCreateItem;
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

  void _item_subscribed(RemoteStorageSubscribePublishedFileResult_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, RemoteStorageSubscribePublishedFileResult_t> callResultItemSubscribed;

  void _item_unsubscribed(RemoteStorageUnsubscribePublishedFileResult_t *callData, bool bIOFailure);
  CCallResult<GodotSteamWorkshop, RemoteStorageUnsubscribePublishedFileResult_t> callResultItemUnsubscribed;

  STEAM_CALLBACK(GodotSteamWorkshop, _item_downloaded, DownloadItemResult_t, callbackItemDownloaded);
  STEAM_CALLBACK(GodotSteamWorkshop, _item_installed, ItemInstalled_t, callbackItemInstalled);

  GDCLASS(GodotSteamWorkshop, Object);
};
#endif // GODOTSTEAMWORKSHOP_H
