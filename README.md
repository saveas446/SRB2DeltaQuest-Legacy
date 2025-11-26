# Demo 2 Ultimate
Demo 2 Ultimate is a mod for Demo 2 that overhauls the existing 4 levels and various assets as well as adding a plethora of new content (notably a full 32-map campaign).

It currently has two builds: SDL2 and Win32, both found in the `/dev/` folder.
While the Win32 build is functionally worse, such as being limited to 640x400, the SDL2 build has much better functionality, but currently has no MIDI or digital audio support. 

## Launcher
This mod has its own dedicated launcher based off of startSRB2 by Shardvexs.
The source code can be found in `/tools/launcher/` and the binary in `/dev/`.

> [!NOTE]  
> When a new build of the launcher is made, you will most likely have to delete your autosave.xml file or else saving will not work.

## Editing
Legacy Builder is currently the best option for editing Demo 2 Ultimate levels due to its ability to preview FOFs and other special linedefs.
Ultimate Doom Builder has been historically used due to its high stability with older SRB2 versions. A configuration for it is present in the `/tools/` folder.

## Building
While options for platforms besides Windows are planned, currently the only supported ways to build are through Visual Studio.

### Visual Studio 2008
This is the main way to build the game.
The VS2008 project files can be found at `src/win32/Srb2win.sln` and `src/sdl/Srb2SDL.sln` for the Win32 and SDL2 builds respectively. Once those have been located, *insert tutorial on how to add additional library directories and other stuff*

### Visual Studio 2022
The VS2022 project files can be found at `src/vs17/Srb2win.sln` and `src/sdl/Srb2SDL_vs17.sln` for the Win32 and SDL2 builds respectively. Once those have been located, *insert tutorial on how to add additional library directories and other stuff*
