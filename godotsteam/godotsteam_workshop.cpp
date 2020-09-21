#include "godotsteam_workshop.h"

GodotSteamWorkshop *GodotSteamWorkshop::singleton = NULL;

GodotSteamWorkshop::GodotSteamWorkshop() { singleton = this; }

GodotSteamWorkshop::~GodotSteamWorkshop() { singleton = NULL; }

GodotSteamWorkshop *GodotSteamWorkshop::get_singleton() {
  if (GodotSteamWorkshop::singleton == NULL) {
    GodotSteamWorkshop::singleton = new GodotSteamWorkshop();
  }

  return GodotSteamWorkshop::singleton;
}

void GodotSteamWorkshop::reset_singleton() {
  delete GodotSteamWorkshop::singleton;

  GodotSteamWorkshop::singleton = NULL;
}

bool GodotSteamWorkshop::isSteamUGCReady() { return SteamUGC() != NULL; }

// Adds a dependency between the given item and the appid. This list of dependencies can be retrieved by calling GetAppDependencies.
// This is a soft-dependency that is displayed on the web. It is up to the application to determine whether the item can actually be used or not.
void GodotSteamWorkshop::addAppDependency(uint64_t publishedFileID, uint64_t appID) {
  STEAM_FAIL_COND(!isSteamUGCReady());

  PublishedFileId_t fileID = (uint64_t)publishedFileID;
  AppId_t app = (uint32_t)appID;
  SteamAPICall_t apiCall = SteamUGC()->AddAppDependency(fileID, app);
  callResultAddAppDependency.Set(apiCall, this, &GodotSteamWorkshop::_add_app_dependency_result);
}

// Adds a workshop item as a dependency to the specified item. If the nParentPublishedFileID item is of type k_EWorkshopFileTypeCollection, than the nChildPublishedFileID is simply added to that collection.
// Otherwise, the dependency is a soft one that is displayed on the web and can be retrieved via the ISteamUGC API using a combination of the m_unNumChildren member variable of the SteamUGCDetails_t struct and GetQueryUGCChildren.
void GodotSteamWorkshop::addDependency(uint64_t publishedFileID, uint64_t childPublishedFileID) {
  STEAM_FAIL_COND(!isSteamUGCReady());

  PublishedFileId_t parent = (uint64_t)publishedFileID;
  PublishedFileId_t child = (uint64_t)childPublishedFileID;
  SteamAPICall_t apiCall = SteamUGC()->AddDependency(parent, child);
  callResultAddUGCDependency.Set(apiCall, this, &GodotSteamWorkshop::_add_ugc_dependency_result);
}

// Adds a excluded tag to a pending UGC Query. This will only return UGC without the specified tag.
bool GodotSteamWorkshop::addExcludedTag(int queryHandle, const String& tagName) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  UGCQueryHandle_t handle = (uint64_t)queryHandle;
  return SteamUGC()->AddExcludedTag(handle, tagName.utf8().get_data());
}

// Adds a key-value tag pair to an item. Keys can map to multiple different values (1-to-many relationship).
bool GodotSteamWorkshop::addItemKeyValueTag(int updateHandle, const String& key, const String& value) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  UGCUpdateHandle_t handle = (uint64_t)updateHandle;
  return SteamUGC()->AddItemKeyValueTag(handle, key.utf8().get_data(), value.utf8().get_data());
}

// Adds an additional preview file for the item.
bool GodotSteamWorkshop::addItemPreviewFile(int queryHandle, const String& previewFile, int type) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  EItemPreviewType previewType = k_EItemPreviewType_ReservedMax;
  UGCQueryHandle_t handle = (uint64_t)queryHandle;

  switch ((EItemPreviewType)type) {
    case k_EItemPreviewType_YouTubeVideo:
    case k_EItemPreviewType_Sketchfab:
      // Not supported by Steam:
      // https://partner.steamgames.com/doc/api/ISteamUGC#AddItemPreviewFile
      return false;
    break;

    case k_EItemPreviewType_Image:
    case k_EItemPreviewType_EnvironmentMap_HorizontalCross:
    case k_EItemPreviewType_EnvironmentMap_LatLong:
      previewType = (EItemPreviewType)type;
    break;
  }

  return SteamUGC()->AddItemPreviewFile(handle, previewFile.utf8().get_data(), previewType);
}

// Adds an additional video preview from YouTube for the item.
bool GodotSteamWorkshop::addItemPreviewVideo(int queryHandle, const String& videoID) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  UGCQueryHandle_t handle = (uint64_t)queryHandle;
  return SteamUGC()->AddItemPreviewVideo(handle, videoID.utf8().get_data());
}

// Adds a workshop item to the users favorites list.
void GodotSteamWorkshop::addItemToFavorite(AppId_t appID, uint64_t publishedFileID) {
  STEAM_FAIL_COND(!isSteamUGCReady());

  PublishedFileId_t fileID = (uint64_t)publishedFileID;
  SteamAPICall_t apiCall = SteamUGC()->AddItemToFavorites(appID, fileID);
  callResultFavoriteItemListChanged.Set(apiCall, this, &GodotSteamWorkshop::_user_favorite_items_list_changed);
}

// Adds a required key-value tag to a pending UGC Query. This will only return workshop items that have a key = pKey and a value = pValue.
bool GodotSteamWorkshop::addRequiredKeyValueTag(int queryHandle, const String& key, const String& value) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  UGCQueryHandle_t handle = (uint64_t)queryHandle;
  return SteamUGC()->AddRequiredKeyValueTag(handle, key.utf8().get_data(), value.utf8().get_data());
}

// Adds a required tag to a pending UGC Query. This will only return UGC with the specified tag.
bool GodotSteamWorkshop::addRequiredTag(int queryHandle, const String& tagName) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  UGCQueryHandle_t handle = (uint64_t)queryHandle;
  return SteamUGC()->AddRequiredTag(handle, tagName.utf8().get_data());
}

// Lets game servers set a specific workshop folder before issuing any UGC commands.
bool GodotSteamWorkshop::initWorkshopForGameServer(int workshopDepotID) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  DepotId_t workshop = (uint32_t)workshopDepotID;
  const char *folder = new char[QUERY_MAX_LENGTH];
  return SteamUGC()->BInitWorkshopForGameServer(workshop, (char*)folder);
}

// Creates a new workshop item with no content attached yet.
void GodotSteamWorkshop::createItem(AppId_t appID, int fileType) {
  STEAM_FAIL_COND(!isSteamUGCReady());

  SteamAPICall_t apiCall = SteamUGC()->CreateItem(appID, (EWorkshopFileType)fileType);
  callResultCreateItem.Set(apiCall, this, &GodotSteamWorkshop::_item_created);
}

// Query for all matching UGC. You can use this to list all of the available UGC for your app.
void GodotSteamWorkshop::createQueryAllUGCRequest(int queryType, int matchingType, AppId_t creatorID, int page) {
  STEAM_FAIL_COND(!isSteamUGCReady());

  EUGCQuery query = k_EUGCQuery_RankedByLifetimePlaytimeSessions;
  EUGCMatchingUGCType match = k_EUGCMatchingUGCType_GameManagedItems;

  switch ((EUGCQuery)queryType) {
    case k_EUGCQuery_RankedByVote:
    case k_EUGCQuery_RankedByPublicationDate:
    case k_EUGCQuery_AcceptedForGameRankedByAcceptanceDate:
    case k_EUGCQuery_RankedByTrend:
    case k_EUGCQuery_FavoritedByFriendsRankedByPublicationDate:
    case k_EUGCQuery_CreatedByFriendsRankedByPublicationDate:
    case k_EUGCQuery_RankedByNumTimesReported:
    case k_EUGCQuery_CreatedByFollowedUsersRankedByPublicationDate:
    case k_EUGCQuery_NotYetRated:
    case k_EUGCQuery_RankedByTotalVotesAsc:
    case k_EUGCQuery_RankedByVotesUp:
    case k_EUGCQuery_RankedByTextSearch:
    case k_EUGCQuery_RankedByTotalUniqueSubscriptions:
    case k_EUGCQuery_RankedByPlaytimeTrend:
    case k_EUGCQuery_RankedByTotalPlaytime:
    case k_EUGCQuery_RankedByAveragePlaytimeTrend:
    case k_EUGCQuery_RankedByLifetimeAveragePlaytime:
    case k_EUGCQuery_RankedByPlaytimeSessionsTrend:
      query = (EUGCQuery)queryType;
    break;
  }

  switch((EUGCMatchingUGCType)matchingType) {
    case k_EUGCMatchingUGCType_All:
    case k_EUGCMatchingUGCType_Items:
    case k_EUGCMatchingUGCType_Items_Mtx:
    case k_EUGCMatchingUGCType_Items_ReadyToUse:
    case k_EUGCMatchingUGCType_Collections:
    case k_EUGCMatchingUGCType_Artwork:
    case k_EUGCMatchingUGCType_Videos:
    case k_EUGCMatchingUGCType_Screenshots:
    case k_EUGCMatchingUGCType_AllGuides:
    case k_EUGCMatchingUGCType_WebGuides:
    case k_EUGCMatchingUGCType_IntegratedGuides:
    case k_EUGCMatchingUGCType_UsableInGame:
    case k_EUGCMatchingUGCType_ControllerBindings:
    case k_EUGCMatchingUGCType_GameManagedItems:
      match = (EUGCMatchingUGCType)matchingType;
    break;
  }

  UGCQueryHandle_t handle = SteamUGC()->CreateQueryAllUGCRequest(query, match, creatorID, creatorID, page);
  SteamAPICall_t apiCall = SteamUGC()->SendQueryUGCRequest(handle);

  callResultUGCQueryCompleted.Set(apiCall, this, &GodotSteamWorkshop::_ugc_query_completed);
}

void GodotSteamWorkshop::createQueryUserUGCRequest(AccountID_t accountID, int listType, AppId_t creatorID, uint32 page) {
  STEAM_FAIL_COND(!isSteamUGCReady());

  EUserUGCList list = k_EUserUGCList_Followed;

  switch((EUserUGCList)listType) {
    case k_EUserUGCList_Published:
    case k_EUserUGCList_VotedOn:
    case k_EUserUGCList_VotedUp:
    case k_EUserUGCList_VotedDown:
    case k_EUserUGCList_WillVoteLater:
    case k_EUserUGCList_Favorited:
    case k_EUserUGCList_Subscribed:
    case k_EUserUGCList_UsedOrPlayed:
      list = (EUserUGCList)listType;
    break;
  }

  UGCQueryHandle_t handle = SteamUGC()->CreateQueryUserUGCRequest(accountID, list, k_EUGCMatchingUGCType_Items, k_EUserUGCListSortOrder_LastUpdatedDesc, creatorID, creatorID, page);
  SteamAPICall_t apiCall = SteamUGC()->SendQueryUGCRequest(handle);

  callResultUGCQueryCompleted.Set(apiCall, this, &GodotSteamWorkshop::_ugc_query_completed);
}

// Query for the details of specific workshop items.
void GodotSteamWorkshop::createQueryUGCDetailsRequest(Array publishedFileIDs) {
  STEAM_FAIL_COND(!isSteamUGCReady());

  uint32 fileCount = publishedFileIDs.size();

  STEAM_FAIL_COND(fileCount == 0);

  PublishedFileId_t *fileIDs = new PublishedFileId_t[fileCount];

  for (uint32 index = 0; index < fileCount; index++) {

    fileIDs[index] = (uint64_t)publishedFileIDs[index];
  }

  UGCQueryHandle_t handle = SteamUGC()->CreateQueryUGCDetailsRequest(fileIDs, fileCount);
  SteamAPICall_t apiCall = SteamUGC()->SendQueryUGCRequest(handle);

  callResultUGCQueryCompleted.Set(apiCall, this, &GodotSteamWorkshop::_ugc_query_completed);
}

void GodotSteamWorkshop::deleteItem(uint64_t publishedFileID) {
  STEAM_FAIL_COND(!isSteamUGCReady());

  PublishedFileId_t fileID = (uint64_t)publishedFileID;
  SteamAPICall_t apiCall = SteamUGC()->DeleteItem(fileID);
  callResultDeleteItem.Set(apiCall, this, &GodotSteamWorkshop::_item_deleted);
}

// Download new or update already installed item. If returns true, wait for
// DownloadItemResult_t. If item is already installed, then files on disk should
// not be used until callback received. If item is not subscribed to, it will be
// cached for some time. If bHighPriority is set, any other item download will
// be suspended and this item downloaded ASAP.
bool GodotSteamWorkshop::downloadItem(uint64_t publishedFileID, bool bHighPriority) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  PublishedFileId_t fileID = (int)publishedFileID;
  return SteamUGC()->DownloadItem(fileID, bHighPriority);
}

// Get info about a pending download of a workshop item that has k_EItemStateNeedsUpdate set.
Dictionary GodotSteamWorkshop::getItemDownloadInfo(uint64_t publishedFileID) {
  Dictionary info;

  STEAM_FAIL_COND_V(!isSteamUGCReady(), info);

  uint64 downloaded = 0;
  uint64 total = 0;
  info["ret"] = SteamUGC()->GetItemDownloadInfo((PublishedFileId_t)publishedFileID, &downloaded, &total);

  if (info["ret"]) {
    info["downloaded"] = uint64_t(downloaded);
    info["total"] = uint64_t(total);
  }

  return info;
}

// Gets info about currently installed content on the disc for workshop items that have k_EItemStateInstalled set.
Dictionary GodotSteamWorkshop::getItemInstallInfo(uint64_t publishedFileID) {
  Dictionary info;

  info["ret"] = false;

  STEAM_FAIL_COND_V(!isSteamUGCReady(), info);

  PublishedFileId_t fileID = (int)publishedFileID;
  uint64 sizeOnDisk;
  char folder[1024] = { 0 };
  uint32 timeStamp;
  info["ret"] = SteamUGC()->GetItemInstallInfo((PublishedFileId_t)fileID, &sizeOnDisk, folder, sizeof(folder), &timeStamp);

  if (info["ret"]) {
    info["size"] = (int)sizeOnDisk;
    info["folder"] = folder;
    info["foldersize"] = (uint32)sizeof(folder);
    info["timestamp"] = timeStamp;
  }
  return info;
}

int GodotSteamWorkshop::getItemState(uint64_t publishedFileID) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), 0);

  PublishedFileId_t fileID = (int)publishedFileID;
  return SteamUGC()->GetItemState(fileID);
}

Dictionary GodotSteamWorkshop::getItemUpdateProgress(int updateHandle) {
  Dictionary updateProgress;

  STEAM_FAIL_COND_V(!isSteamUGCReady(), updateProgress);

  UGCUpdateHandle_t handle = (uint64_t)updateHandle;
  uint64 processed = 0;
  uint64 total = 0;

  EItemUpdateStatus status = SteamUGC()->GetItemUpdateProgress(handle, &processed, &total);
  updateProgress["status"] = status;
  updateProgress["processed"] = uint64_t(processed);
  updateProgress["total"] = uint64_t(total);

  return updateProgress;
}

int GodotSteamWorkshop::getNumSubscribedItems() {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), -1);

  return SteamUGC()->GetNumSubscribedItems();
}

// Retrieve the details of an additional preview associated with an individual workshop item after receiving a querying UGC call result.
Dictionary GodotSteamWorkshop::getQueryUGCAdditionalPreview(int queryHandle, int index, int previewIndex) {
  Dictionary preview;

  STEAM_FAIL_COND_V(!isSteamUGCReady(), preview);

  UGCQueryHandle_t handle = (uint64_t)queryHandle;
  char *urlOrVideoID = new char[QUERY_MAX_LENGTH];
  char *originalFilename = new char[QUERY_MAX_LENGTH];
  EItemPreviewType previewType;
  bool success = SteamUGC()->GetQueryUGCAdditionalPreview(handle, index, previewIndex, (char*)urlOrVideoID, QUERY_MAX_LENGTH, (char*)originalFilename, QUERY_MAX_LENGTH, &previewType);

  if (success) {
    preview["success"] = success;
    preview["handle"] = (uint64_t)handle;
    preview["index"] = index;
    preview["preview"] = previewIndex;
    preview["urlOrVideo"] = urlOrVideoID;
    preview["filename"] = originalFilename;
    preview["type"] = previewType;
  }

  return preview;
}

// Retrieve the ids of any child items of an individual workshop item after receiving a querying UGC call result. These items can either be a part of a collection or some other dependency (see AddDependency).
Dictionary GodotSteamWorkshop::getQueryUGCChildren(int queryHandle, int index) {
  Dictionary children;

  STEAM_FAIL_COND_V(!isSteamUGCReady(), children);

  UGCQueryHandle_t handle = (uint64_t)queryHandle;
  PublishedFileId_t *child = new PublishedFileId_t[100];
  bool success = SteamUGC()->GetQueryUGCChildren(handle, index, (PublishedFileId_t*)child, 100);

  if (success) {
    children["success"] = success;
    children["handle"] = (uint64_t)handle;
    children["index"] = index;
    children["children"] = child;
  }

  return children;
}

// Retrieve the details of a key-value tag associated with an individual workshop item after receiving a querying UGC call result.
Dictionary GodotSteamWorkshop::getQueryUGCKeyValueTag(int queryHandle, int index, int keyValueTagIndex) {
  Dictionary tag;

  STEAM_FAIL_COND_V(!isSteamUGCReady(), tag);

  UGCQueryHandle_t handle = (uint64_t)queryHandle;
  char *key = new char[QUERY_MAX_LENGTH];
  char *value = new char[QUERY_MAX_LENGTH];
  bool success = SteamUGC()->GetQueryUGCKeyValueTag(handle, index, keyValueTagIndex, (char*)key, QUERY_MAX_LENGTH, (char*)value, QUERY_MAX_LENGTH);

  if (success) {
    tag["success"] = success;
    tag["handle"] = (uint64_t)handle;
    tag["index"] = index;
    tag["tag"] = keyValueTagIndex;
    tag["key"] = key;
    tag["value"] = value;
  }

  return tag;
}
// Retrieve the developer set metadata of an individual workshop item after receiving a querying UGC call result.
String GodotSteamWorkshop::getQueryUGCMetadata(int queryHandle, int index) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), "");

  UGCQueryHandle_t handle = (uint64_t)queryHandle;
  char *metadata = new char[QUERY_MAX_LENGTH];
  bool success = SteamUGC()->GetQueryUGCMetadata(handle, index, (char*)metadata, QUERY_MAX_LENGTH);

  if (success) {
    String ugcMetadata = metadata;
    return ugcMetadata;
  }

  return "";
}
// Retrieve the number of additional previews of an individual workshop item after receiving a querying UGC call result.
int GodotSteamWorkshop::getQueryUGCNumAdditionalPreviews(int queryHandle, int index) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), -1);

  UGCQueryHandle_t handle = (uint64_t)queryHandle;
  return SteamUGC()->GetQueryUGCNumAdditionalPreviews(handle, index);
}
// Retrieve the number of key-value tags of an individual workshop item after receiving a querying UGC call result.
int GodotSteamWorkshop::getQueryUGCNumKeyValueTags(int queryHandle, int index) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), -1);

  UGCQueryHandle_t handle = (uint64_t)queryHandle;
  return SteamUGC()->GetQueryUGCNumKeyValueTags(handle, index);
}
// Retrieve the URL to the preview image of an individual workshop item after receiving a querying UGC call result.
String GodotSteamWorkshop::getQueryUGCPreviewURL(int queryHandle, int index) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), "");

  UGCQueryHandle_t handle = (uint64_t)queryHandle;
  char *url = new char[QUERY_MAX_LENGTH];

  STEAM_FAIL_COND_V(!SteamUGC()->GetQueryUGCPreviewURL(handle, index, (char*)url, QUERY_MAX_LENGTH), "");

  String previewURL = url;
  return previewURL;
}
// Retrieve the details of an individual workshop item after receiving a querying UGC call result.
Dictionary GodotSteamWorkshop::getQueryUGCResult(int queryHandle, int index) {
  Dictionary ugcResult;

  STEAM_FAIL_COND_V(!isSteamUGCReady(), ugcResult);

  UGCQueryHandle_t handle = (uint64_t)queryHandle;
  SteamUGCDetails_t pDetails;

  bool success = SteamUGC()->GetQueryUGCResult(handle, index, &pDetails);

  if (success) {
    ugcResult["result"] = (uint64_t)pDetails.m_eResult;
    ugcResult["fileID"] = (uint64_t)pDetails.m_nPublishedFileId;
    ugcResult["fileType"] = (uint64_t)pDetails.m_eFileType;
    ugcResult["creatorAppID"] = (uint64_t)pDetails.m_nCreatorAppID;
    ugcResult["consumerAppID"] = (uint64_t)pDetails.m_nConsumerAppID;
    ugcResult["title"] = pDetails.m_rgchTitle;
    ugcResult["description"] = pDetails.m_rgchDescription;
    ugcResult["steamIDOwner"] = (uint64_t)pDetails.m_ulSteamIDOwner;
    ugcResult["timeCreated"] = pDetails.m_rtimeCreated;
    ugcResult["timeUpdated"] = pDetails.m_rtimeUpdated;
    ugcResult["timeAddedToUserList"] = pDetails.m_rtimeAddedToUserList;
    ugcResult["visibility"] = (uint64_t)pDetails.m_eVisibility;
    ugcResult["banned"] = pDetails.m_bBanned;
    ugcResult["acceptedForUse"] = pDetails.m_bAcceptedForUse;
    ugcResult["tagsTruncated"] = pDetails.m_bTagsTruncated;
    ugcResult["tags"] = pDetails.m_rgchTags;
    ugcResult["handleFile"] = (uint64_t)pDetails.m_hFile;
    ugcResult["handlePreviewFile"] = (uint64_t)pDetails.m_hPreviewFile;
    ugcResult["fileName"] = pDetails.m_pchFileName;
    ugcResult["fileSize"] = pDetails.m_nFileSize;
    ugcResult["previewFileSize"] = pDetails.m_nPreviewFileSize;
    ugcResult["url"] = pDetails.m_rgchURL;
    ugcResult["votesUp"] = pDetails.m_unVotesUp;
    ugcResult["votesDown"] = pDetails.m_unVotesDown;
    ugcResult["score"] = pDetails.m_flScore;
    ugcResult["numChildren"] = pDetails.m_unNumChildren;
  }

  return ugcResult;
}

// Retrieve various statistics of an individual workshop item after receiving a querying UGC call result.
Dictionary GodotSteamWorkshop::getQueryUGCStatistic(int queryHandle, int index, int statType) {
  Dictionary ugcStat;

  STEAM_FAIL_COND_V(!isSteamUGCReady(), ugcStat);

  UGCQueryHandle_t handle = (uint64_t)queryHandle;
  EItemStatistic type = k_EItemStatistic_NumPlaytimeSessionsDuringTimePeriod;

  switch((EItemStatistic)statType) {
    case k_EItemStatistic_NumSubscriptions:
    case k_EItemStatistic_NumFavorites:
    case k_EItemStatistic_NumFollowers:
    case k_EItemStatistic_NumUniqueSubscriptions:
    case k_EItemStatistic_NumUniqueFavorites:
    case k_EItemStatistic_NumUniqueFollowers:
    case k_EItemStatistic_NumUniqueWebsiteViews:
    case k_EItemStatistic_ReportScore:
    case k_EItemStatistic_NumSecondsPlayed:
    case k_EItemStatistic_NumPlaytimeSessions:
    case k_EItemStatistic_NumComments:
    case k_EItemStatistic_NumSecondsPlayedDuringTimePeriod:
      type = (EItemStatistic)statType;
    break;
  }

  uint64 value = 0;
  bool success = SteamUGC()->GetQueryUGCStatistic(handle, index, type, &value);

  if (success) {
    ugcStat["success"] = success;
    ugcStat["handle"] = (uint64_t)handle;
    ugcStat["index"] = index;
    ugcStat["type"] = type;
    ugcStat["value"] = (uint64_t)value;
  } 
  return ugcStat;
}

Array GodotSteamWorkshop::getSubscribedItems() {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), Array());

  Array subscribed;
  int numItems = SteamUGC()->GetNumSubscribedItems();
  PublishedFileId_t *items = new PublishedFileId_t[numItems];
  int itemList = SteamUGC()->GetSubscribedItems(items, numItems);

  for (int index = 0; index < itemList; index++) {
    subscribed.append((int)items[index]);
  }

  delete items;
  return subscribed;
}

bool GodotSteamWorkshop::releaseQueryUGCRequest(int queryHandle) {

  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);
  UGCQueryHandle_t handle = (uint64_t)queryHandle;

  return SteamUGC()->ReleaseQueryUGCRequest(handle);
}

// Removes the dependency between the given item and the appid. This list of dependencies can be retrieved by calling GetAppDependencies.
void GodotSteamWorkshop::removeAppDependency(uint64_t publishedFileID, uint64_t appID) {
  STEAM_FAIL_COND(!isSteamUGCReady());

  PublishedFileId_t fileID = (uint64_t)publishedFileID;
  AppId_t app = (uint32_t)appID;
  SteamAPICall_t apiCall = SteamUGC()->RemoveAppDependency(fileID, app);
  callResultRemoveAppDependency.Set(apiCall, this, &GodotSteamWorkshop::_remove_app_dependency_result);
}

// Removes a workshop item as a dependency from the specified item.
void GodotSteamWorkshop::removeDependency(uint64_t publishedFileID, int childPublishedFileID) {
  STEAM_FAIL_COND(!isSteamUGCReady());

  PublishedFileId_t fileID = (uint64_t)publishedFileID;
  PublishedFileId_t childID = (uint64_t)childPublishedFileID;
  SteamAPICall_t apiCall = SteamUGC()->RemoveDependency(fileID, childID);
  callResultRemoveUGCDependency.Set(apiCall, this, &GodotSteamWorkshop::_remove_ugc_dependency_result);
}

// Removes a workshop item from the users favorites list.
void GodotSteamWorkshop::removeItemFromFavorites(uint64_t appID, uint64_t publishedFileID) {
  STEAM_FAIL_COND(!isSteamUGCReady());

  PublishedFileId_t fileID = (uint64_t)publishedFileID;
  AppId_t app = (uint32_t)appID;
  SteamAPICall_t apiCall = SteamUGC()->RemoveItemFromFavorites(app, fileID);
  callResultFavoriteItemListChanged.Set(apiCall, this, &GodotSteamWorkshop::_user_favorite_items_list_changed);
}

// Removes an existing key value tag from an item.
bool GodotSteamWorkshop::removeItemKeyValueTags(int updateHandle, const String& key) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  UGCUpdateHandle_t handle = uint64(updateHandle);
  return SteamUGC()->RemoveItemKeyValueTags(handle, key.utf8().get_data());
}

// Removes an existing preview from an item.
bool GodotSteamWorkshop::removeItemPreview(int updateHandle, int index) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  UGCUpdateHandle_t handle = uint64(updateHandle);
  return SteamUGC()->RemoveItemPreview(handle, index);
}

// Sets the folder that will be stored as the content for an item.
bool GodotSteamWorkshop::setItemContent(int updateHandle, const String& contentFolder) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  UGCUpdateHandle_t handle = uint64(updateHandle);
  return SteamUGC()->SetItemContent(handle, contentFolder.utf8().get_data());
}

// Sets a new description for an item.
bool GodotSteamWorkshop::setItemDescription(int updateHandle, const String& description) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  if (description.length() > DESCRIPTION_MAX_LENGTH) {
    printf("Description cannot have more than %d ASCII characters. Description not set.", DESCRIPTION_MAX_LENGTH);
    return false;
  }

  UGCUpdateHandle_t handle = uint64(updateHandle);
  return SteamUGC()->SetItemDescription(handle, description.utf8().get_data());
}

// Sets arbitrary metadata for an item. This metadata can be returned from queries without having to download and install the actual content.
bool GodotSteamWorkshop::setItemMetadata(int updateHandle, const String& metadata) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  if (metadata.length() > DESCRIPTION_MAX_LENGTH) {
    printf("Metadata cannot have more than %d ASCII characters. Metadata not set.", DESCRIPTION_MAX_LENGTH);
    return false;
  }

  UGCUpdateHandle_t handle = uint64(updateHandle);
  return SteamUGC()->SetItemMetadata(handle, metadata.utf8().get_data());
}

// Sets the primary preview image for the item.
bool GodotSteamWorkshop::setItemPreview(int updateHandle, const String& previewFile) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  UGCUpdateHandle_t handle = uint64(updateHandle);
  return SteamUGC()->SetItemPreview(handle, previewFile.utf8().get_data());
}

// Sets arbitrary developer specified tags on an item.
bool GodotSteamWorkshop::setItemTags(int updateHandle, Array tagArray) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  UGCUpdateHandle_t handle = uint64(updateHandle);
  SteamParamStringArray_t *pTags = new SteamParamStringArray_t();
  pTags->m_ppStrings = new const char*[tagArray.size()];
  uint32 strCount = tagArray.size();

  for (uint32 index = 0; index < strCount; index++) {
    String str = (String)tagArray[index];
    pTags->m_ppStrings[index] = str.utf8().get_data();
  }

  pTags->m_nNumStrings = tagArray.size();

  return SteamUGC()->SetItemTags(handle, pTags);
}

// Sets a new title for an item.
bool GodotSteamWorkshop::setItemTitle(int updateHandle, const String& title) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  if (title.length() > DESCRIPTION_MAX_LENGTH) {
    printf("Title cannot have more than %d ASCII characters. Title not set.", DESCRIPTION_MAX_LENGTH);

    return false;
  }

  UGCUpdateHandle_t handle = uint64(updateHandle);
  return SteamUGC()->SetItemTitle(handle, title.utf8().get_data());
}

// Sets the language of the title and description that will be set in this item update.
bool GodotSteamWorkshop::setItemUpdateLanguage(int updateHandle, const String& language) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  UGCUpdateHandle_t handle = uint64(updateHandle);
  return SteamUGC()->SetItemUpdateLanguage(handle, language.utf8().get_data());
}

// Sets the visibility of an item.
bool GodotSteamWorkshop::setItemVisibility(int updateHandle, int visibility) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  UGCUpdateHandle_t handle = uint64(updateHandle);
  return SteamUGC()->SetItemVisibility(handle, (ERemoteStoragePublishedFileVisibility)visibility);
}

// Sets the language to return the title and description in for the items on a pending UGC Query.
bool GodotSteamWorkshop::setLanguage(int queryHandle, const String& language) {
  STEAM_FAIL_COND_V(!isSteamUGCReady(), false);

  UGCQueryHandle_t handle = (uint64_t)queryHandle;
  return SteamUGC()->SetLanguage(handle, language.utf8().get_data());
}

// SuspendDownloads( true ) will suspend all workshop downloads until
// SuspendDownloads( false ) is called or the game ends
void GodotSteamWorkshop::suspendDownloads(bool bSuspend) {
  return SteamUGC()->SuspendDownloads(bSuspend);
}

void GodotSteamWorkshop::subscribeItem(uint64_t publishedFileID) {
  STEAM_FAIL_COND(!isSteamUGCReady());

  PublishedFileId_t fileID = (uint64_t)publishedFileID;
  SteamAPICall_t apiCall = SteamUGC()->SubscribeItem(fileID);
  callResultItemSubscribed.Set(apiCall, this, &GodotSteamWorkshop::_item_subscribed);
}

void GodotSteamWorkshop::unsubscribeItem(uint64_t publishedFileID) {
  STEAM_FAIL_COND(!isSteamUGCReady());

  PublishedFileId_t fileID = (uint64_t)publishedFileID;
  SteamAPICall_t apiCall = SteamUGC()->UnsubscribeItem(fileID);
  callResultItemUnsubscribed.Set(apiCall, this, &GodotSteamWorkshop::_item_unsubscribed);
}

// CALLBACKS
void GodotSteamWorkshop::_add_app_dependency_result(AddAppDependencyResult_t* callData, bool ioFailure) {

  EResult result = callData->m_eResult;
  PublishedFileId_t fileID = callData->m_nPublishedFileId;
  AppId_t appID = callData->m_nAppID;

  emit_signal("workshop_add_app_dependency_result", result, (uint64_t)fileID, (uint64_t)appID);
}

// The result of a call to AddDependency.
void GodotSteamWorkshop::_add_ugc_dependency_result(AddUGCDependencyResult_t* callData, bool ioFailure) {

  EResult result = callData->m_eResult;
  PublishedFileId_t fileID = callData->m_nPublishedFileId;
  PublishedFileId_t childID = callData->m_nChildPublishedFileId;

  emit_signal("workshop_add_ugc_dependency_result", result, (uint64_t)fileID, (uint64_t)childID);
}

// Result of a workshop item being created.
void GodotSteamWorkshop::_item_created(CreateItemResult_t *callData, bool ioFailure) {

  EResult result = callData->m_eResult;
  PublishedFileId_t fileID = callData->m_nPublishedFileId;
  bool acceptTOS = callData->m_bUserNeedsToAcceptWorkshopLegalAgreement;

  emit_signal("workshop_item_created", result, (uint64_t)fileID, acceptTOS);
}

// Called when getting the app dependencies for an item.
void GodotSteamWorkshop::_get_app_dependencies_result(GetAppDependenciesResult_t* callData, bool ioFailure) {

  EResult result = callData->m_eResult;
  PublishedFileId_t fileID = callData->m_nPublishedFileId;

  uint32 appDependencies = callData->m_nNumAppDependencies;
  uint32 totalAppDependencies = callData->m_nTotalNumAppDependencies;

  emit_signal("workshop_app_dependencies_result", result, (uint64_t)fileID, appDependencies, totalAppDependencies);
}

// Called when getting the users vote status on an item.
void GodotSteamWorkshop::_get_item_vote_result(GetUserItemVoteResult_t* callData, bool ioFailure) {

  EResult result = callData->m_eResult;
  PublishedFileId_t fileID = callData->m_nPublishedFileId;
  bool voteUp = callData->m_bVotedUp;
  bool voteDown = callData->m_bVotedDown;
  bool voteSkipped = callData->m_bVoteSkipped;

  emit_signal("workshop_item_vote_result", result, (uint64_t)fileID, voteUp, voteDown, voteSkipped);
}

// Purpose: The result of a call to RemoveAppDependency.
void GodotSteamWorkshop::_remove_app_dependency_result(RemoveAppDependencyResult_t* callData, bool ioFailure) {

  EResult result = callData->m_eResult;
  PublishedFileId_t fileID = callData->m_nPublishedFileId;
  AppId_t appID = callData->m_nAppID;

  emit_signal("workshop_remove_app_dependency_result", result, (uint64_t)fileID, (uint64_t)appID);
}

// Purpose: The result of a call to RemoveDependency.
void GodotSteamWorkshop::_remove_ugc_dependency_result(RemoveUGCDependencyResult_t* callData, bool ioFailure) {

  EResult result = callData->m_eResult;
  PublishedFileId_t fileID = callData->m_nPublishedFileId;
  PublishedFileId_t childID = callData->m_nChildPublishedFileId;

  emit_signal("workshop_remove_ugc_dependency_result", result, (uint64_t)fileID, (uint64_t)childID);
}

// Called when the user has voted on an item.
void GodotSteamWorkshop::_set_user_item_vote(SetUserItemVoteResult_t* callData, bool ioFailure) {

  EResult result = callData->m_eResult;
  PublishedFileId_t fileID = callData->m_nPublishedFileId;
  bool voteUp = callData->m_bVoteUp;

  emit_signal("workshop_set_user_item_vote", result, (uint64_t)fileID, voteUp);
}

// Called when workshop item playtime tracking has started.
void GodotSteamWorkshop::_start_playtime_tracking(StartPlaytimeTrackingResult_t* callData, bool ioFailure) {
  EResult result = callData->m_eResult;
  emit_signal("workshop_start_playtime_tracking", (uint64_t)result);
}

// Called when a UGC query request completes.
void GodotSteamWorkshop::_ugc_query_completed(SteamUGCQueryCompleted_t* callData, bool ioFailure) {

  UGCQueryHandle_t handle = callData->m_handle;
  EResult result = callData->m_eResult;
  uint32 resultsReturned = callData->m_unNumResultsReturned;
  uint32 totalMatching = callData->m_unTotalMatchingResults;
  bool cached = callData->m_bCachedData;

  emit_signal("workshop_ugc_query_completed", (int)handle, result, resultsReturned, totalMatching, cached);
}

// Called when workshop item playtime tracking has stopped.
void GodotSteamWorkshop::_stop_playtime_tracking(StopPlaytimeTrackingResult_t* callData, bool ioFailure) {

  EResult result = callData->m_eResult;

  emit_signal("workshop_stop_playtime_tracking", result);
}

// Result of a workshop item being updated.
void GodotSteamWorkshop::_item_updated(SubmitItemUpdateResult_t *callData, bool ioFailure) {

  EResult result = callData->m_eResult;
  bool acceptTOS = callData->m_bUserNeedsToAcceptWorkshopLegalAgreement;

  emit_signal("workshop_item_updated", result, acceptTOS);
}

// Called when the user has added or removed an item to/from their favorites.
void GodotSteamWorkshop::_user_favorite_items_list_changed(UserFavoriteItemsListChanged_t* callData, bool ioFailure) {

  EResult result = callData->m_eResult;
  PublishedFileId_t fileID = callData->m_nPublishedFileId;
  bool wasAddRequest = callData->m_bWasAddRequest;

  emit_signal("workshop_user_favorite_items_list_changed", result, (uint64_t)fileID, wasAddRequest);
}

void GodotSteamWorkshop::_item_deleted(DeleteItemResult_t* callData, bool ioFailure) {

  EResult result = callData->m_eResult;
  PublishedFileId_t fileID = callData->m_nPublishedFileId;

  emit_signal("workshop_item_deleted", result, (uint64_t)fileID, ioFailure);
}

void GodotSteamWorkshop::_item_downloaded(DownloadItemResult_t* callData) {

  EResult result = callData->m_eResult;
  PublishedFileId_t fileID = callData->m_nPublishedFileId;
  AppId_t appID = callData->m_unAppID;

  emit_signal("workshop_item_downloaded", result, (uint64_t)fileID, (uint64_t)appID);
}

void GodotSteamWorkshop::_item_installed(ItemInstalled_t* callData) {

  AppId_t appID = callData->m_unAppID;
  PublishedFileId_t fileID = callData->m_nPublishedFileId;

  emit_signal("workshop_item_installed", (uint64_t)fileID, (uint64_t)appID);
}

void GodotSteamWorkshop::_item_subscribed(RemoteStorageSubscribePublishedFileResult_t *callData, bool bIOFailure) {

  EResult result = callData->m_eResult;
  PublishedFileId_t fileID = callData->m_nPublishedFileId;
  emit_signal("workshop_item_subscribed", result, (uint64_t)fileID);
}

void GodotSteamWorkshop::_item_unsubscribed(RemoteStorageUnsubscribePublishedFileResult_t *callData, bool bIOFailure) {

  EResult result = callData->m_eResult;
  PublishedFileId_t fileID = callData->m_nPublishedFileId;
  emit_signal("workshop_item_unsubscribed", result, (uint64_t)fileID);
}

void GodotSteamWorkshop::_bind_methods() {

  ObjectTypeDB::bind_method("addAppDependency",&GodotSteamWorkshop::addAppDependency);
  ObjectTypeDB::bind_method("addDependency",&GodotSteamWorkshop::addDependency);
  ObjectTypeDB::bind_method("addExcludedTag",&GodotSteamWorkshop::addExcludedTag);
  ObjectTypeDB::bind_method("addItemKeyValueTag",&GodotSteamWorkshop::addItemKeyValueTag);
  ObjectTypeDB::bind_method("addItemPreviewFile",&GodotSteamWorkshop::addItemPreviewFile);
  ObjectTypeDB::bind_method("addItemPreviewVideo",&GodotSteamWorkshop::addItemPreviewVideo);
  ObjectTypeDB::bind_method("addItemToFavorite",&GodotSteamWorkshop::addItemToFavorite);
  ObjectTypeDB::bind_method("addRequiredKeyValueTag",&GodotSteamWorkshop::addRequiredKeyValueTag);
  ObjectTypeDB::bind_method("addRequiredTag",&GodotSteamWorkshop::addRequiredTag);

  ObjectTypeDB::bind_method("initWorkshopForGameServer",&GodotSteamWorkshop::initWorkshopForGameServer);

  ObjectTypeDB::bind_method("createItem",&GodotSteamWorkshop::createItem);
  ObjectTypeDB::bind_method("createQueryAllUGCRequest",&GodotSteamWorkshop::createQueryAllUGCRequest);
  ObjectTypeDB::bind_method("createQueryUGCDetailsRequest",&GodotSteamWorkshop::createQueryUGCDetailsRequest);
  ObjectTypeDB::bind_method("createQueryUserUGCRequest",&GodotSteamWorkshop::createQueryUserUGCRequest);

  ObjectTypeDB::bind_method("deleteItem",&GodotSteamWorkshop::deleteItem);
  ObjectTypeDB::bind_method("downloadItem",&GodotSteamWorkshop::downloadItem);

  ObjectTypeDB::bind_method("getItemDownloadInfo",&GodotSteamWorkshop::getItemDownloadInfo);
  ObjectTypeDB::bind_method("getItemInstallInfo",&GodotSteamWorkshop::getItemInstallInfo);
  ObjectTypeDB::bind_method("getItemState",&GodotSteamWorkshop::getItemState);
  ObjectTypeDB::bind_method("getItemUpdateProgress",&GodotSteamWorkshop::getItemUpdateProgress);
  ObjectTypeDB::bind_method("getNumSubscribedItems",&GodotSteamWorkshop::getNumSubscribedItems);
  ObjectTypeDB::bind_method("getQueryUGCAdditionalPreview",&GodotSteamWorkshop::getQueryUGCAdditionalPreview);
  ObjectTypeDB::bind_method("getQueryUGCChildren",&GodotSteamWorkshop::getQueryUGCChildren);
  ObjectTypeDB::bind_method("getQueryUGCKeyValueTag",&GodotSteamWorkshop::getQueryUGCKeyValueTag);
  ObjectTypeDB::bind_method("getQueryUGCMetadata",&GodotSteamWorkshop::getQueryUGCMetadata);
  ObjectTypeDB::bind_method("getQueryUGCNumAdditionalPreviews",&GodotSteamWorkshop::getQueryUGCNumAdditionalPreviews);
  ObjectTypeDB::bind_method("getQueryUGCNumKeyValueTags",&GodotSteamWorkshop::getQueryUGCNumKeyValueTags);
  ObjectTypeDB::bind_method("getQueryUGCPreviewURL",&GodotSteamWorkshop::getQueryUGCPreviewURL);
  ObjectTypeDB::bind_method("getQueryUGCResult",&GodotSteamWorkshop::getQueryUGCResult);
  ObjectTypeDB::bind_method("getQueryUGCStatistic",&GodotSteamWorkshop::getQueryUGCStatistic);
  ObjectTypeDB::bind_method("getSubscribedItems",&GodotSteamWorkshop::getSubscribedItems);

  ObjectTypeDB::bind_method("releaseQueryUGCRequest",&GodotSteamWorkshop::releaseQueryUGCRequest);
  ObjectTypeDB::bind_method("removeAppDependency",&GodotSteamWorkshop::removeAppDependency);
  ObjectTypeDB::bind_method("removeDependency",&GodotSteamWorkshop::removeDependency);
  ObjectTypeDB::bind_method("removeItemFromFavorites",&GodotSteamWorkshop::removeItemFromFavorites);
  ObjectTypeDB::bind_method("removeItemKeyValueTags",&GodotSteamWorkshop::removeItemKeyValueTags);
  ObjectTypeDB::bind_method("removeItemPreview",&GodotSteamWorkshop::removeItemPreview);

  ObjectTypeDB::bind_method("setItemContent",&GodotSteamWorkshop::setItemContent);
  ObjectTypeDB::bind_method("setItemDescription",&GodotSteamWorkshop::setItemDescription);
  ObjectTypeDB::bind_method("setItemMetadata",&GodotSteamWorkshop::setItemMetadata);
  ObjectTypeDB::bind_method("setItemPreview",&GodotSteamWorkshop::setItemPreview);
  ObjectTypeDB::bind_method("setItemTags",&GodotSteamWorkshop::setItemTags);
  ObjectTypeDB::bind_method("setItemTitle",&GodotSteamWorkshop::setItemTitle);
  ObjectTypeDB::bind_method("setItemUpdateLanguage",&GodotSteamWorkshop::setItemUpdateLanguage);
  ObjectTypeDB::bind_method("setItemVisibility",&GodotSteamWorkshop::setItemVisibility);
  ObjectTypeDB::bind_method("setLanguage",&GodotSteamWorkshop::setLanguage);
  ObjectTypeDB::bind_method("subscribeItem",&GodotSteamWorkshop::subscribeItem);
  ObjectTypeDB::bind_method("suspendDownloads",&GodotSteamWorkshop::suspendDownloads);

  ObjectTypeDB::bind_method("unsubscribeItem",&GodotSteamWorkshop::unsubscribeItem);


  ADD_SIGNAL(MethodInfo("workshop_item_deleted", 
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::INT, "fileID"),
    PropertyInfo(Variant::BOOL, "failure")
  ));

  ADD_SIGNAL(MethodInfo("workshop_item_subscribed", 
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::INT, "fileID")
  ));

  ADD_SIGNAL(MethodInfo("workshop_item_unsubscribed", 
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::INT, "fileID")
  ));

  ADD_SIGNAL(MethodInfo("workshop_item_downloaded", 
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::INT, "fileID"),
    PropertyInfo(Variant::INT, "appID")
  ));

  ADD_SIGNAL(MethodInfo("workshop_item_installed", 
    PropertyInfo(Variant::INT, "fileID"),
    PropertyInfo(Variant::INT, "appID")
  ));

  ADD_SIGNAL(MethodInfo("workshop_add_app_dependency_result",
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::INT, "fileID"),
    PropertyInfo(Variant::INT, "appID")
  ));

  ADD_SIGNAL(MethodInfo("workshop_add_ugc_dependency_result",
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::INT, "fileID"),
    PropertyInfo(Variant::INT, "childID")
  ));

  ADD_SIGNAL(MethodInfo("workshop_item_created",
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::INT, "fileID"),
    PropertyInfo(Variant::BOOL, "acceptTOS")
  ));

  ADD_SIGNAL(MethodInfo("workshop_app_dependencies_result",
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::INT, "fileID"),
    PropertyInfo(Variant::INT, "appDependencies"),
    PropertyInfo(Variant::INT, "totalAppDependencies")
  ));

  ADD_SIGNAL(MethodInfo("workshop_item_vote_result",
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::INT, "fileID"),
    PropertyInfo(Variant::BOOL, "voteUp"),
    PropertyInfo(Variant::BOOL, "voteDown"),
    PropertyInfo(Variant::BOOL, "voteSkipped")
  ));

  ADD_SIGNAL(MethodInfo("workshop_remove_app_dependency_result",
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::INT, "fileID"),
    PropertyInfo(Variant::INT, "appID")
  ));

  ADD_SIGNAL(MethodInfo("workshop_remove_ugc_dependency_result",
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::INT, "fileID"),
    PropertyInfo(Variant::INT, "childID")
  ));

  ADD_SIGNAL(MethodInfo("workshop_set_user_item_vote",
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::INT, "fileID"),
    PropertyInfo(Variant::BOOL, "voteUp")
  ));

  ADD_SIGNAL(MethodInfo("workshop_start_playtime_tracking",
    PropertyInfo(Variant::INT, "result")
  ));

  ADD_SIGNAL(MethodInfo("workshop_ugc_query_completed",
    PropertyInfo(Variant::INT, "handle"),
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::INT, "resultsReturned"),
    PropertyInfo(Variant::INT, "totalMatching"),
    PropertyInfo(Variant::BOOL, "cached")
  ));

  ADD_SIGNAL(MethodInfo("workshop_stop_playtime_tracking",
    PropertyInfo(Variant::INT, "result")
  ));

  ADD_SIGNAL(MethodInfo("workshop_item_updated",
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::BOOL, "acceptTOS")
  ));

  ADD_SIGNAL(MethodInfo("workshop_user_favorite_items_list_changed",
    PropertyInfo(Variant::INT, "result"),
    PropertyInfo(Variant::INT, "fileID"),
    PropertyInfo(Variant::BOOL, "wasAddRequest")
  ));
}
