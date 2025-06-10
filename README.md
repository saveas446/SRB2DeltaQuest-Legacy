# Demo 2 Ultimate
Demo 2 Ultimate is a mod for Demo 2 that overhauls the existing 4 levels and various assets as well as adding a plethora of new content (notably a full 32-map campaign).

## Editing
Ultimate Doom Builder has been historically used due to its high stability with the Demo 2 executable and a configuration for it is present in the /tools/ folder, however Zone Builder has been known to work and will most likely be the better editor for the NewEXE due to its previews of water and FOFs.

## Building

Building the NewEXE currently has three ways: two completed, and one that is heavily incomplete.

### Visual Studio 6
Building the NewEXE will require a copy of Visual C++ or Visual Studio 6.0, MASM 6.xx for building zlib, and a build of one of the free versions of FMOD (I use 3.75.). A copy of MASM that has been known to work with this codebase is included in the /tools/ folder for your convenience.

#### Environment
Due to Visual C++/Visual Studio 6.0 not installing correctly on modern versions of Windows, it is recommended to use a virtual machine of Windows 2000 or Windows XP with the Demo 2 Ultimate repository mapped to the X: drive. You can do this using a variety of methods, including VirtualBox's Shared Folders feature and creating a network drive.

### Visual Studio 8
Building with Visual Studio 8 can be accomplished by copying your include and lib folders from VS6 into VS8 and not overwriting any duplicate files.

### CMake
TODO: THIS METHOD IS VERY INCOMPLETE
