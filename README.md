# Demo 2 Ultimate
Demo 2 Ultimate is a mod for Demo 2 that overhauls the existing 4 levels and various assets as well as adding a plethora of new content (notably a full 32-map campaign).
It currently has two builds: SDL and Win32, both found in the `/dev/` folder.
> [!NOTE]  
> The Win32 and SDL builds have pros and cons, with Win32 having digital music support but being limited to 640x400 in windowed mode and SDL being much more robust, but with worse mouse capturing and only MIDI support.

## Launcher
This mod has its own dedicated launcher based off of startSRB2 by Shardvexs.
The source code can be found in `/tools/launcher/` and the binary in `/dev/`.

> [!NOTE]  
> When a new build of the launcher is made, you will most likely have to delete your autosave.xml file or else saving will not work.

## Editing
Ultimate Doom Builder has been historically used due to its high stability with older SRB2 versions. A configuration for it is present in the `/tools/` folder.
Zone Builder has also been tested, however the maps it creates usually crash. If this is encountered, simply open the map with Ultimate Doom Builder and save it again. If it says "Building nodes...", the map has been rebuilt and will now work.
## Building

### Visual Studio 2005
The VS2005 project files can be found at `src/win32/Srb2win.sln` and `src/sdl/Srb2SDL.sln` for the Win32 and SDL builds respectively. Once those have been located, *insert tutorial on how to add additional library directories and shit*

### Visual Studio 2022
The VS2022 project files can be found at `src/vs17/Srb2win.sln` and `src/sdl/Srb2SDL_vs17.sln` for the Win32 and SDL builds respectively. Once those have been located, *insert tutorial on how to add additional library directories and shit*
