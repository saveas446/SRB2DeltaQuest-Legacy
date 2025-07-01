// d2udef.h
/// \file
/// \brief Various #define statements that allow you to easily 
///	enable/disable various Demo 2 Ultimate-specific things.
/// A control panel of sorts to configure the build
/// with different gameplay/interface options for mine and Sam's ease of use.

#ifndef D2UDEF
    #define D2UDEF
	
	///\section def Defines
	
    // This contains all of the #defines we've made 
    // So this file is basically a control panel of sorts to configure the build with different gameplay/interface options for mine and Sam's ease of use

    // More and more defines will be added to this over time, allowing us to tweak more and more things 

	/**
	\brief 
	Disables stuff that's only there for the sake of D2 IWAD compatibility
	\note Needed for OpenGL to work
	*/
    #define DEMO2ULTIMATE


    #define DEMO 0 /// \brief Enables public demo build functionality
    #define PLAYTEST_BUILD 1 /// \brief Enables playtester/private build functionality
    #define DEV_RELEASE 2 /// \brief Enables dev build functionality
    #define FULL_RELEASE 3/// \brief Enables full release functionality

	/**
	\brief Controls what type of release is built (demo, playtester build, dev build, etc.)
	*/
    #define BUILD_TYPE DEV_RELEASE

	/**
	\brief Disables map command for singleplayer
	*/
    //#define NOMAPCMD
	
	/**
	\brief Darkens menu
	*/	
    //#define DARKENEDMENU
	/**
	\brief Enables the character select menu
	*/	
	
    #define CHARACTERSELECT
	/**
	\brief Enables Sonic to multi-thok
	*/	
    #define MULTITHOK
	
	/**
	\brief Enables fall sprites for the characters
	*/	
    //#define FALL_SPRITES
	
    // What background does the intermission use?
    #define IS_NOBG 0 /// \brief No background, currently pretty incomplete but it kinda works
    #define IS_CHARBG 1 /// \brief Rotating character backgrounds, demo style
    #define IS_FDBG 2 /// \brief INTERSCR or INTERSCW, like in vanilla Final Demo
    // "This could be done with an enum!!!!" Well yeah but that wasn't fucking working

	/**
	\brief Controls what type of background the intermission screen uses
	*/	
	#define INTERMISSION_BG IS_CHARBG

	/**
	\brief Disables the Time Attack and Statistics menus
	*/	
    //#define FD_SPMENUOPTIONS

    

	/**
	\brief 
	Originally used when converting this code to be compatible with the Demo 2 IWAD
	Nowadays... don't touch.
	*/	
    #define TH_TOLERANCE 0

	/**
	\brief 
	Used for a part in win_dbg.c
	It's also probably just a good idea to have *something* like this
	*/	
    #if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) || defined(__aarch64__) || defined(__LP64__) || defined(_M_X64)
    #define ARCH_64BIT 1
    #else
    #define ARCH_64BIT 0
    #endif
	
	/**
	\brief 
	Controls major version number (x.0.0)
    0 = Not done
    1 = Initial relase
    2+ = Big, grand releases (MAJOR new features)
	
	\note Don't forget to update in Doxyfile!
	*/	
    #define MAJORVER 0

	/**
	\brief 
	Controls minor version number (0.x.0)
    Each time we add something that's kinda big (minecarts, d2 iwad support, clang support, working sdl, some other custom feature or object)
    increment this
	
	\note Don't forget to update in Doxyfile!
	*/	
    #define MINORVER 1

	/**
	\brief 
	Controls "tiny" version number (0.0.x)
	Increment this every day
	*/
    #define TINYVER 17

	/**
	\brief 
	Controls whether the title screen uses "TITLESKY" or "RSKY1" as its BG
	*/
    #define COMPAT_TITLESKY

	/**
	\brief 
	Controls whether we're using the LTFNT lumps from Final Demo
	*/
    #define LTFNT

    ///\section san_chk Sanity checks
	
    #if BUILD_TYPE > DEV_RELEASE || BUILD_TYPE < DEMO
    #error "Invalid build type!"
    #endif

    #if INTERMISSION_BG > IS_FDBG || INTERMISSION_BG < IS_NOBG
    #error "Invalid intermission background type!"
    #endif
#endif