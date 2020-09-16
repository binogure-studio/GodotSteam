# Godot Steam

Steam API for the Godot game engine. For the Windows, Linux, and Mac platforms. 

View the GodotSteam for GDNative here: https://github.com/Gramps/GodotSteam/tree/gdnative

Documentation
----------
Documentation is available here: https://gramps.github.io/GodotSteam/

You can also check out the Search Help section inside Godot Engine after compiling it with GodotSteam.

Current Build
----------
You can download pre-compiled versions of this repo here: https://github.com/Gramps/GodotSteam/releases

Version 1.1.0
- Changed: updated pre-compiles for Linux and Windows to Steamworks SDK to 1.41

Quick How-To
----------
- Download this repository and unpack it.
- Download and unpack the [Steamworks SDK](https://partner.steamgames.com); this requires a Steam developer account.
- Download and unpack the [Godot source](https://github.com/godotengine/godot); preferably 2.0.3 or higher.
- Move the following to godotsteam/sdk/:
````
    sdk/public/
    sdk/redistributable_bin/
````
- The repo's directory contents should now look like this:
````
    godotsteam/sdk/public/*
    godotsteam/sdk/redistributable_bin/*
    godotsteam/SCsub
    godotsteam/config.py
    godotsteam/godotsteam.cpp
    godotsteam/godotsteam.h
    godotsteam/register_types.cpp
    godotsteam/register_types.h
````
- Now move the "godotsteam" directory into the "modules" directory of the unpacked Godot Engine source.
- Recompile for your platform:
  - Windows ( http://docs.godotengine.org/en/stable/reference/compiling_for_windows.html )
  - Linux ( http://docs.godotengine.org/en/stable/reference/compiling_for_x11.html )
    - If not using Godot 2.0.3 or higher, you must add openssl=no when compiling because it has problems with libcrypto (class StreamPeerSSL can't use).
  - OSX ( http://docs.godotengine.org/en/stable/reference/compiling_for_osx.html )
    - When creating templates for this, please refer to this post for assistance as the documentation is a bit lacking ( http://steamcommunity.com/app/404790/discussions/0/364042703865087202/ ).
- When recompiling the engine is finished, copy the shared library (steam_api) from sdk/redistributable_bin/ folders to the Godot binary location (by default in the godot source /bin/ file but you can move them to a new folder). It should look like this:
  - Linux 32/64-bit
  ```
  libsteam_api.so
  ./godot.linux.tools.32 or ./godot.linux.tools.64
  ```
  - OSX
  ```
  libsteam_api.dylib
  ./godot.osx.tools.32 or ./godot.osx.tools.64
  ```
  - Windows 32-bit
  ```
  steam_api.dll
  ./godot.windows.tools.32.exe
  ```
  - Windows 64-bit
  ```
  steam_api64.dll
  ./godot.windows.tools.64.exe
  ```
- Your game must ship with the executable, Steam API DLL/SO/DyLIB, and steam_appid.txt to function. Lack of the Steam API DLL/SO/DyLib (for your respective OS) or the steam_appid.txt will cause it fail and crash.
  - **NOTE:** For OSX, the libsteam_api.dylib and steam_appid.txt must be in the Content/MacOS/ folder in your application zip or the game will crash.

From here you should be able to call various functions of Steamworks. You should be able to look up the functions in Godot itself under the search section. In addition, you should be able to read the Steamworks API documentation to see what all is available and cross-reference with GodotSteam.

To-Do
-------------
- Add additional functionality, ideally all of the Steamworks API.

License
-------------
MIT license
