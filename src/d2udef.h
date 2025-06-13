// d2udef.h
#ifndef D2UDEF
    #define D2UDEF
    // This contains all of the #defines we've made 
    // So this file is basically a control panel of sorts to configure the build with different gameplay/interface options for mine and Sam's ease of use

    // More and more defines will be added to this over time, allowing us to tweak more and more things 

    #define DEMO2ULTIMATE // Disables stuff that's only there for the sake of D2 IWAD compatibility
    // Needed for OpenGL

    #define DEMO 0 // Enables public demo build functionality
    #define PLAYTEST_BUILD 1 // Enables playtester/private build functionality
    #define DEV_RELEASE 2 // Enables dev build functionality
    #define FULL_RELEASE 3 // Enables full release functionality

    #define BUILD_TYPE DEV_RELEASE

    //#define NOMAPCMD // Disables map command for singleplayer
    //#define DARKENEDMENU // Darkens menu
    //#define CHARACTERSELECT // Enables the character select menu
    #define MULTITHOK // If enabled, Sonic can multi-thok
    //#define FALL_SPRITES // If enabled, Sonic will use fall sprites for some actions

    // What background does the intermission use?
    #define IS_NOBG 0 // No background, currently pretty incomplete but it kinda works
    #define IS_CHARBG 1 // Rotating character backgrounds, demo style
    #define IS_FDBG 2 // INTERSCR or INTERSCW, like in vanilla Final Demo
    // "This could be done with an enum!!!!" Well yeah but that wasn't fucking working

    #define INTERMISSION_BG IS_CHARBG


    //#define TH_TOLERANCE 0
    // Undefined: Strict, ONLY things that were in the original Demo 2 IWAD are allowed
    // 0: Enable some very useful stuff that isn't in the Demo 2 IWAD, but we will need for D2U
    // 1: Enable some possibly useful things that break compatibility with the Demo 2 IWAD

    // Used for a part in win_dbg.c
    // Also probably just a good idea
    #if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) || defined(__aarch64__) || defined(__LP64__) || defined(_M_X64)
    #define ARCH_64BIT 1
    #else
    #define ARCH_64BIT 0
    #endif
    // 0 = Not done
    // 1 = Initial relase
    // 2+ = Big, grand releases (MAJOR new features)
    #define MAJORVER 0

    // Each time we add something that's kinda big (minecarts, d2 iwad support, clang support, working sdl, some other custom feature or object)
    // increment this
    #define MINORVER 1


    // Sanity checks
    #if BUILD_TYPE > DEV_RELEASE || BUILD_TYPE < DEMO
    #error "Invalid build type!"
    #endif

    #if INTERMISSION_BG > IS_FDBG || INTERMISSION_BG < IS_NOBG
    #error "Invalid intermission background type!"
    #endif
#endif