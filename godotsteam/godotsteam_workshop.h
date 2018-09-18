#ifndef GODOTSTEAMWORKSHOP_H
#define GODOTSTEAMWORKSHOP_H

#include <steam/steam_api.h>

#include "object.h"

class GodotSteamWorkshop: public Object
{
public:
  static GodotSteamWorkshop *get_singleton();
  static void reset_singleton();

  GodotSteamWorkshop();
  ~GodotSteamWorkshop();

  bool isSteamUGCReady();
  int getNumSubscribedItems();
  int getItemState(int publishedFileID);
  bool downloadItem(int nPublishedFileID, bool bHighPriority);
  void suspendDownloads(bool bSuspend);

protected:
  static void _bind_methods();
  static GodotSteamWorkshop *singleton;

private:
  OBJ_TYPE(GodotSteamWorkshop, Object);
  OBJ_CATEGORY("GodotSteamWorkshop");
};
#endif // GODOTSTEAMWORKSHOP_H
