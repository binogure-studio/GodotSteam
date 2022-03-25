#ifndef GODOTSTEAMSCREENSHOTS_H
#define GODOTSTEAMSCREENSHOTS_H

#include <steam/steam_api.h>

#include "core/object/object.h"
#include "core/object/ref_counted.h"

#include "godotsteam_utils.h"

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
  GDCLASS(GodotSteamScreenshots, Object);
};
#endif // GODOTSTEAMSCREENSHOTS_H
