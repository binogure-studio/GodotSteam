#ifndef GODOTSTEAMSCREENSHOTS_H
#define GODOTSTEAMSCREENSHOTS_H

#include <steam/steam_api.h>

#include "godotsteam_utils.h"
#include "object.h"

class GodotSteamScreenshots : public Object
{
public:
  static GodotSteamScreenshots *get_singleton();
  static void reset_singleton();

  GodotSteamScreenshots();
  ~GodotSteamScreenshots();

  bool isSteamScreenshotsReady();
  void triggerScreenshot();

protected:
  static void _bind_methods();
  static GodotSteamScreenshots *singleton;

private:
  OBJ_TYPE(GodotSteamScreenshots, Object);
  OBJ_CATEGORY("GodotSteamScreenshots");
};
#endif // GODOTSTEAMSCREENSHOTS_H
