#include "core/config/engine.h"
#include "register_types.h"

#include "godotsteam.h"
#include "godotsteam_friends.h"
#include "godotsteam_matchmaking.h"
#include "godotsteam_music.h"
#include "godotsteam_networkingsockets.h"
#include "godotsteam_networkingutils.h"
#include "godotsteam_screenshots.h"
#include "godotsteam_user.h"
#include "godotsteam_userstats.h"
#include "godotsteam_utils.h"
#include "godotsteam_workshop.h"

void initialize_godotsteam_module(ModuleInitializationLevel p_level) {

  if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }

  GDREGISTER_CLASS(GodotSteam);
  GDREGISTER_CLASS(GodotSteamFriends);
  GDREGISTER_CLASS(GodotSteamMatchmaking);
  GDREGISTER_CLASS(GodotSteamMusic);
  GDREGISTER_CLASS(GodotSteamNetworkingSockets);
  GDREGISTER_CLASS(GodotSteamNetworkingUtils);
  GDREGISTER_CLASS(GodotSteamScreenshots);
  GDREGISTER_CLASS(GodotSteamUser);
  GDREGISTER_CLASS(GodotSteamUserstats);
  GDREGISTER_CLASS(GodotSteamUtils);
  GDREGISTER_CLASS(GodotSteamWorkshop);

  Engine::get_singleton()->add_singleton(Engine::Singleton("GodotSteam", GodotSteam::get_singleton()));
  Engine::get_singleton()->add_singleton(Engine::Singleton("GodotSteamFriends", GodotSteamFriends::get_singleton()));
  Engine::get_singleton()->add_singleton(Engine::Singleton("GodotSteamMatchmaking", GodotSteamMatchmaking::get_singleton()));
  Engine::get_singleton()->add_singleton(Engine::Singleton("GodotSteamMusic", GodotSteamMusic::get_singleton()));
  Engine::get_singleton()->add_singleton(Engine::Singleton("GodotSteamNetworkingSockets", GodotSteamNetworkingSockets::get_singleton()));
  Engine::get_singleton()->add_singleton(Engine::Singleton("GodotSteamNetworkingUtils", GodotSteamNetworkingUtils::get_singleton()));
  Engine::get_singleton()->add_singleton(Engine::Singleton("GodotSteamScreenshots", GodotSteamScreenshots::get_singleton()));
  Engine::get_singleton()->add_singleton(Engine::Singleton("GodotSteamUser", GodotSteamUser::get_singleton()));
  Engine::get_singleton()->add_singleton(Engine::Singleton("GodotSteamUserstats", GodotSteamUserstats::get_singleton()));
  Engine::get_singleton()->add_singleton(Engine::Singleton("GodotSteamUtils", GodotSteamUtils::get_singleton()));
  Engine::get_singleton()->add_singleton(Engine::Singleton("GodotSteamWorkshop", GodotSteamWorkshop::get_singleton()));
}

void uninitialize_godotsteam_module(ModuleInitializationLevel p_level) {

  if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }

  GodotSteam::reset_singleton();
  GodotSteamFriends::reset_singleton();
  GodotSteamMatchmaking::reset_singleton();
  GodotSteamMusic::reset_singleton();
  GodotSteamNetworkingSockets::reset_singleton();
  GodotSteamNetworkingUtils::reset_singleton();
  GodotSteamScreenshots::reset_singleton();
  GodotSteamUser::reset_singleton();
  GodotSteamUserstats::reset_singleton();
  GodotSteamUtils::reset_singleton();
  GodotSteamWorkshop::reset_singleton();
}
