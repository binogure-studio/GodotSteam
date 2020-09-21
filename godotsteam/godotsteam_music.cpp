#include "godotsteam_music.h"

GodotSteamMusic *GodotSteamMusic::singleton = NULL;

GodotSteamMusic::GodotSteamMusic() { singleton = this; }
GodotSteamMusic::~GodotSteamMusic() { singleton = NULL; }

GodotSteamMusic *GodotSteamMusic::get_singleton() {
  if (GodotSteamMusic::singleton == NULL) {
    GodotSteamMusic::singleton = new GodotSteamMusic();
  }

  return GodotSteamMusic::singleton;
}

void GodotSteamMusic::reset_singleton() {
  delete GodotSteamMusic::singleton;

  GodotSteamMusic::singleton = NULL;
}

bool GodotSteamMusic::isSteamMusicReady() { return SteamMusic() != NULL; }

bool GodotSteamMusic::musicIsEnabled() {
  STEAM_FAIL_COND_V(!isSteamMusicReady(), false);

  return SteamMusic()->BIsEnabled();
}

bool GodotSteamMusic::musicIsPlaying() {
  STEAM_FAIL_COND_V(!isSteamMusicReady(), false);

  return SteamMusic()->BIsPlaying();
}

float GodotSteamMusic::musicGetVolume() {
  STEAM_FAIL_COND_V(!isSteamMusicReady(), 0);

  return SteamMusic()->GetVolume();
}

void GodotSteamMusic::musicPause() {
  STEAM_FAIL_COND(!isSteamMusicReady());

  return SteamMusic()->Pause();
}

void GodotSteamMusic::musicPlay() {
  STEAM_FAIL_COND(!isSteamMusicReady());

  return SteamMusic()->Play();
}

void GodotSteamMusic::musicPlayNext() {
  STEAM_FAIL_COND(!isSteamMusicReady());

  return SteamMusic()->PlayNext();
}

void GodotSteamMusic::musicPlayPrev() {
  STEAM_FAIL_COND(!isSteamMusicReady());

  return SteamMusic()->PlayPrevious();
}

void GodotSteamMusic::musicSetVolume(float value) {
  STEAM_FAIL_COND(!isSteamMusicReady());

  return SteamMusic()->SetVolume(value);
}
void GodotSteamMusic::_bind_methods() {
  ObjectTypeDB::bind_method("musicIsEnabled", &GodotSteamMusic::musicIsEnabled);
  ObjectTypeDB::bind_method("musicIsPlaying", &GodotSteamMusic::musicIsPlaying);
  ObjectTypeDB::bind_method("musicGetVolume", &GodotSteamMusic::musicGetVolume);
  ObjectTypeDB::bind_method("musicPause", &GodotSteamMusic::musicPause);
  ObjectTypeDB::bind_method("musicPlay", &GodotSteamMusic::musicPlay);
  ObjectTypeDB::bind_method("musicPlayNext", &GodotSteamMusic::musicPlayNext);
  ObjectTypeDB::bind_method("musicPlayPrev", &GodotSteamMusic::musicPlayPrev);
  ObjectTypeDB::bind_method("musicSetVolume", &GodotSteamMusic::musicSetVolume);
}
