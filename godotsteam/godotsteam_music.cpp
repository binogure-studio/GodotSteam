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
  if (!isSteamMusicReady()) {
    return false;
  }
  return SteamMusic()->BIsEnabled();
}

bool GodotSteamMusic::musicIsPlaying() {
  if (!isSteamMusicReady()) {
    return false;
  }
  return SteamMusic()->BIsPlaying();
}

float GodotSteamMusic::musicGetVolume() {
  if (!isSteamMusicReady()) {
    return 0;
  }
  return SteamMusic()->GetVolume();
}

void GodotSteamMusic::musicPause() {
  if (!isSteamMusicReady()) {
    return;
  }
  return SteamMusic()->Pause();
}

void GodotSteamMusic::musicPlay() {
  if (!isSteamMusicReady()) {
    return;
  }
  return SteamMusic()->Play();
}

void GodotSteamMusic::musicPlayNext() {
  if (!isSteamMusicReady()) {
    return;
  }
  return SteamMusic()->PlayNext();
}

void GodotSteamMusic::musicPlayPrev() {
  if (!isSteamMusicReady()) {
    return;
  }
  return SteamMusic()->PlayPrevious();
}

void GodotSteamMusic::musicSetVolume(float value) {
  if (!isSteamMusicReady()) {
    return;
  }
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
