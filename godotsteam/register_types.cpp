#include "globals.h"
#include "object_type_db.h"
#include "register_types.h"

#include "godotsteam.h"
#include "godotsteam_friends.h"
#include "godotsteam_matchmaking.h"
#include "godotsteam_music.h"
#include "godotsteam_screenshots.h"
#include "godotsteam_user.h"
#include "godotsteam_userstats.h"
#include "godotsteam_utils.h"
#include "godotsteam_workshop.h"

void register_godotsteam_types() {
  Globals::get_singleton()->add_singleton(Globals::Singleton("GodotSteam", GodotSteam::get_singleton()));
  Globals::get_singleton()->add_singleton(Globals::Singleton("GodotSteamFriends", GodotSteamFriends::get_singleton()));
  Globals::get_singleton()->add_singleton(Globals::Singleton("GodotSteamMatchmaking", GodotSteamMatchmaking::get_singleton()));
  Globals::get_singleton()->add_singleton(Globals::Singleton("GodotSteamMusic", GodotSteamMusic::get_singleton()));
  Globals::get_singleton()->add_singleton(Globals::Singleton("GodotSteamScreenshots", GodotSteamScreenshots::get_singleton()));
  Globals::get_singleton()->add_singleton(Globals::Singleton("GodotSteamUser", GodotSteamUser::get_singleton()));
  Globals::get_singleton()->add_singleton(Globals::Singleton("GodotSteamUserstats", GodotSteamUserstats::get_singleton()));
  Globals::get_singleton()->add_singleton(Globals::Singleton("GodotSteamUtils", GodotSteamUtils::get_singleton()));
  Globals::get_singleton()->add_singleton(Globals::Singleton("GodotSteamWorkshop", GodotSteamWorkshop::get_singleton()));

  ObjectTypeDB::register_virtual_type<GodotSteam>();
  ObjectTypeDB::register_virtual_type<GodotSteamFriends>();
  ObjectTypeDB::register_virtual_type<GodotSteamMatchmaking>();
  ObjectTypeDB::register_virtual_type<GodotSteamMusic>();
  ObjectTypeDB::register_virtual_type<GodotSteamScreenshots>();
  ObjectTypeDB::register_virtual_type<GodotSteamUser>();
  ObjectTypeDB::register_virtual_type<GodotSteamUserstats>();
  ObjectTypeDB::register_virtual_type<GodotSteamUtils>();
  ObjectTypeDB::register_virtual_type<GodotSteamWorkshop>();
}

void unregister_godotsteam_types() {
  GodotSteam::reset_singleton();
  GodotSteamFriends::reset_singleton();
  GodotSteamMatchmaking::reset_singleton();
  GodotSteamMusic::reset_singleton();
  GodotSteamScreenshots::reset_singleton();
  GodotSteamUser::reset_singleton();
  GodotSteamUserstats::reset_singleton();
  GodotSteamUtils::reset_singleton();
  GodotSteamWorkshop::reset_singleton();
}
