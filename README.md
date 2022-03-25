# Godot Steam

Steam API for the Godot game engine. For the Windows, Linux, and Mac platforms. 

## Documentation
Refer to the code and check out the [Steam documentation](https://partner.steamgames.com/doc/api/ISteamUGC).

## Quick How-To
- Download this repository and unpack it.
- Download and unpack the [Steamworks SDK](https://partner.steamgames.com); this requires a Steam developer account.
- Download and unpack the [Godot source](https://github.com/binogure-studio/GodotSteam); branch 4.0
- Move the following to `godotsteam/sdk/`:
````
    sdk/public/
    sdk/redistributable_bin/
````
- Now move the **godotsteam** directory into the **modules** directory of the unpacked Godot Engine source.
- [Recompile for your platform](https://docs.godotengine.org/en/latest/development/compiling/index.html):
- When recompiling the engine is finished, copy the shared library (steam_api) from sdk/redistributable_bin/ folders to the Godot binary location (by default in the godot source /bin/ file but you can move them to a new folder). It should look like this:
  - Linux 32/64-bit
  ```
  linux32/libsteam_api.so
  linux64/libsteam_api.so
  ./godot.linux.tools.32 or ./godot.linux.tools.64
  ```
  - OSX
  ```
  osx/libsteam_api.dylib
  godot.osx.tools.64
  ```
  - Windows 32-bit
  ```
  win32/steam_api.dll
  win32/steam_api.lib
  godot.windows.tools.32.exe
  ```
  - Windows 64-bit
  ```
  win64/steam_api64.dll
  win64/steam_api64.lib
  godot.windows.tools.64.exe
  ```

- Your game must ship with the executable, Steam API DLL/SO/DyLIB, and steam_appid.txt to function. Lack of the Steam API DLL/SO/DyLib (for your respective OS) or the **steam_appid.txt** will cause it fail and crash.
  - **NOTE:** For OSX, the `libsteam_api.dylib` and `steam_appid.txt` must be in the `Content/MacOS/` folder in your application zip or the game will crash.

From here you should be able to call various functions of Steamworks. You should be able to look up the functions in Godot itself under the search section. In addition, you should be able to read the Steamworks API documentation to see what all is available and cross-reference with GodotSteam.

## License
MIT license
