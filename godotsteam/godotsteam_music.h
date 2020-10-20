#ifndef GODOTSTEAMMUSIC_H
#define GODOTSTEAMMUSIC_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "core/object.h"
#include "core/reference.h"
#include "core/dictionary.h"
#include "godotsteam_utils.h"

class GodotSteamMusic : public Object
{
public:
  static GodotSteamMusic *get_singleton();
  static void reset_singleton();

  GodotSteamMusic();
  ~GodotSteamMusic();

  bool isSteamMusicReady();
  bool musicIsEnabled();
  bool musicIsPlaying();
  float musicGetVolume();
  void musicPause();
  void musicPlay();
  void musicPlayNext();
  void musicPlayPrev();
  void musicSetVolume(float value);

protected:
  static void _bind_methods();
  static GodotSteamMusic *singleton;

private:
  OBJ_TYPE(GodotSteamMusic, Object);
  OBJ_CATEGORY("GodotSteamMusic");
};
#endif // GODOTSTEAMMUSIC_H
