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

int GodotSteamWorkshop::getNumSubscribedItems() {
  if (!isSteamUGCReady()) {
    return 0;
  }
  return SteamUGC()->GetNumSubscribedItems();
}

int GodotSteamWorkshop::getItemState(int publishedFileID) {
  if (!isSteamUGCReady()) {
    return 0;
  }
  PublishedFileId_t fileID = (int)publishedFileID;
  return SteamUGC()->GetItemState(fileID);
}

// Download new or update already installed item. If returns true, wait for
// DownloadItemResult_t. If item is already installed, then files on disk should
// not be used until callback received. If item is not subscribed to, it will be
// cached for some time. If bHighPriority is set, any other item download will
// be suspended and this item downloaded ASAP.
bool GodotSteamWorkshop::downloadItem(int publishedFileID, bool bHighPriority) {
  if (!isSteamUGCReady()) {
    return 0;
  }
  PublishedFileId_t fileID = (int)publishedFileID;
  return SteamUGC()->DownloadItem(fileID, bHighPriority);
}
// SuspendDownloads( true ) will suspend all workshop downloads until
// SuspendDownloads( false ) is called or the game ends
void GodotSteamWorkshop::suspendDownloads(bool bSuspend) {
  return SteamUGC()->SuspendDownloads(bSuspend);
}

void GodotSteamWorkshop::_bind_methods() {
  ObjectTypeDB::bind_method("getNumSubscribedItems",
                            &GodotSteamWorkshop::getNumSubscribedItems);
  ObjectTypeDB::bind_method("getItemState", &GodotSteamWorkshop::getItemState);
  ObjectTypeDB::bind_method("downloadItem", &GodotSteamWorkshop::downloadItem);
  ObjectTypeDB::bind_method("suspendDownloads", &GodotSteamWorkshop::suspendDownloads);
}
