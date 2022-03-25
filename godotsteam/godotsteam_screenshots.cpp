#include "godotsteam_screenshots.h"

GodotSteamScreenshots *GodotSteamScreenshots::singleton = NULL;

GodotSteamScreenshots::GodotSteamScreenshots() { singleton = this; }

GodotSteamScreenshots::~GodotSteamScreenshots() { singleton = NULL; }

GodotSteamScreenshots *GodotSteamScreenshots::get_singleton() {
  if (GodotSteamScreenshots::singleton == NULL) {
    GodotSteamScreenshots::singleton = new GodotSteamScreenshots();
  }

  return GodotSteamScreenshots::singleton;
}

void GodotSteamScreenshots::reset_singleton() {
  delete GodotSteamScreenshots::singleton;

  GodotSteamScreenshots::singleton = NULL;
}

bool GodotSteamScreenshots::isSteamScreenshotsReady() { return SteamScreenshots() != NULL; }

void GodotSteamScreenshots::triggerScreenshot() {
  STEAM_FAIL_COND(!isSteamScreenshotsReady());

  SteamScreenshots()->TriggerScreenshot();
}

void GodotSteamScreenshots::_bind_methods() {
  ClassDB::bind_method("triggerScreenshot", &GodotSteamScreenshots::triggerScreenshot);
}
