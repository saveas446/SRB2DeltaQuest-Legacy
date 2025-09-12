// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// Copyright (C) 1993-1996 by id Software, Inc.
// Copyright (C) 1998-2000 by DooM Legacy Team.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//-----------------------------------------------------------------------------
/// \file
/// \brief Sound and music info

#ifndef __SOUNDS__
#define __SOUNDS__

// 10 customisable sounds for Skins
typedef enum
{
	SKSSPIN,
	SKSPUTPUT,
	SKSPUDPUD,
	SKSPLPAN1, // Ouchies
	SKSPLPAN2,
	SKSPLPAN3,
	SKSPLPAN4,
	SKSPLDET1, // Deaths
	SKSPLDET2,
	SKSPLDET3,
	SKSPLDET4,
	SKSPLTNT1, // Taunts
	SKSPLTNT2,
	SKSPLTNT3,
	SKSPLTNT4,
	SKSPLVCT1, // Victories
	SKSPLVCT2,
	SKSPLVCT3,
	SKSPLVCT4,
	SKSTHOK,
	SKSSPNDSH,
	SKSZOOM,
	SKSRADIO,
	SKSGASP,
	SKSJUMP,
    SKSOUCH,
	NUMSKINSOUNDS
} skinsound_t;

// free sfx for S_AddSoundFx()
#define NUMSFXFREESLOTS 800 // Matches SOC Editor.
#define NUMSKINSFXSLOTS (32*NUMSKINSOUNDS)

//
// SoundFX struct.
//
typedef struct sfxinfo_struct sfxinfo_t;

struct sfxinfo_struct
{
	// up to 6-character name
	const char *name;

	// Sfx singularity (only one at a time)
	int singularity;

	// Sfx priority
	int priority;

	// pitch if a link
	int pitch;

	// volume if a link
	int volume;

	// sound data
	void *data;

	// length of sound data
	int length;

	// sound that can be remapped for a skin, indexes skins[].skinsounds
	// 0 up to (NUMSKINSOUNDS-1), -1 = not skin specifc
	int skinsound;

	// this is checked every second to see if sound
	// can be thrown out (if 0, then decrement, if -1,
	// then throw out, if > 0, then it is in use)
	int usefulness;

	// lump number of sfx
	int lumpnum;

	// Zmemory string
	char *zname;
};

//
// MusicInfo struct.
//
typedef struct
{
	// up to 6-character name
	const char *name;

	// lump number of music
	int lumpnum;

	// music data
	void *data;

	// music handle once registered
	int handle;
} musicinfo_t;

// the complete set of sound effects
extern sfxinfo_t S_sfx[];

// the complete set of music
extern musicinfo_t S_music[];

//
// Identifiers for all music in game.
//

// Music list (don't edit this comment!)
typedef enum
{
    mus_None,
 /*   mus_e1m1,
    mus_e1m2,
    mus_e1m3,
    mus_e1m4,
    mus_e1m5,
    mus_e1m6,
    mus_e1m7,
    mus_e1m8,
    mus_e1m9,
    mus_e2m1,
    mus_e2m2,
    mus_e2m3,
    mus_e2m4,
    mus_e2m5,
    mus_e2m6,
    mus_e2m7,
    mus_e2m8,
    mus_e2m9,
    mus_e3m1,
    mus_e3m2,
    mus_e3m3,
    mus_e3m4,
    mus_e3m5,
    mus_e3m6,
    mus_e3m7,
    mus_e3m8,
    mus_e3m9,
    mus_inter,
    mus_intro,
    mus_bunny,
    mus_victor,
    mus_introa,*/
    mus_gfz1,
    mus_gfz2,
    mus_gfz3,
    mus_thz1,
    mus_thz2,
    mus_thz3,
    mus_dsz1,
    mus_dsz2,
    mus_dsz3,
    mus_mmz1,
    mus_mmz2,
    mus_mmz3,
    mus_rmz1,
    mus_rmz2,
    mus_rmz3,
    mus_rvz1,
    mus_rvz2,
    mus_rvz3,
    mus_dcz1,
    mus_dcz2,
    mus_dcz3,
    mus_gez1,
    mus_gez2,
    mus_gez3,
    mus_rrz1,
    mus_rrz2,
    mus_rrz3,
    mus_ffz,
    mus_wood,
    mus_dushil,
    mus_spdhwy,
    mus_hdnplc,
    mus_swtth,
    mus_crcave,
    mus_eggfac,
    mus_slmvly,
    mus_fsz1,
    mus_fsz2,
    mus_fszbos, // FSZ2 has 2 songs, so make the credits level use a different song later
    mus_ttlscr, // Moved it up Tails 01-06-2001
    mus_inter, // Intermission - Sam 12/09/25 1:45 PM
	mus_lvclr, // Level Clear - Sam 12/09/25 1:46 PM
    mus_invinc, // invincibility Tails
    mus_drown, // drowning Tails 03-06-2000
    mus_gmover, // Tails 03-14-2000
    mus_xtlife, // Tails 03-14-2000
    NUMMUSIC
} musicenum_t;

// Note: If you make music slots go up to 2048 or higher you'll have to change
// how the mapmusic variable is used.

//
// Identifiers for all sfx in game.
//

// VS2002 and VS2003 are untested but this works for VS6 and VS8/VS2005
#if defined(_MSC_VER) && _MSC_VER < 1400
#define DATA_NONE -1
#else
#define DATA_NONE ((void*)(intptr_t)-1)
#endif

// List of sounds (don't modify this comment!)
typedef enum
{
	sfx_None,
    sfx_pistol,
    sfx_shotgn,
    sfx_sgcock,
    sfx_sawup,
    sfx_sawidl,
    sfx_sawful,
    sfx_sawhit,
    sfx_rlaunc,
    sfx_rxplod,
    sfx_firsht,
    sfx_pstart,
    sfx_pstop,
    sfx_doropn,
    sfx_dorcls,
    sfx_stnmov,
    sfx_swtchn,
    sfx_swtchx,
    sfx_plpain,
    sfx_dmpain,
    sfx_slopain,
    sfx_vipain,
    sfx_mnpain,
    sfx_pepain,
    sfx_slop,
    sfx_itemup,
    sfx_wpnup,
    sfx_oof,
    sfx_telept,
    sfx_posit1,
    sfx_posit2,
    sfx_posit3,
    sfx_bgsit1,
    sfx_bgsit2,
    sfx_sgtsit,
    sfx_cacsit,
    sfx_brssit,
    sfx_cybsit,
    sfx_spisit,
    sfx_bspsit,
    sfx_kntsit,
    sfx_vilsit,
    sfx_mansit,
    sfx_pesit,
    sfx_sklatk,
    sfx_sgtatk,
    sfx_skepch,
    sfx_vilatk,
    sfx_claw,
    sfx_skeswg,
    sfx_pldeth,
    sfx_pdiehi,
    sfx_podth1,
    sfx_podth2,
    sfx_podth3,
    sfx_bgdth1,
    sfx_bgdth2,
    sfx_cacdth,
    sfx_skldth,
    sfx_brsdth,
    sfx_cybdth,
    sfx_spidth,
    sfx_bspdth,
    sfx_vildth,
    sfx_kntdth,
    sfx_pedth,
    sfx_skedth,
    sfx_posact,
    sfx_bgact,
    sfx_dmact,
    sfx_bspact,
    sfx_bspwlk,
    sfx_vilact,
    sfx_noway,
    sfx_barexp,
    sfx_punch,
    sfx_hoof,
    sfx_metal,
    sfx_chgun,
    sfx_tink,
    sfx_bdopn,
    sfx_bdcls,
    sfx_itmbk,
    sfx_flame,
    sfx_flamst,
    sfx_getpow,
    sfx_bospit,
    sfx_boscub,
    sfx_bossit,
    sfx_bospn,
    sfx_bosdth,
    sfx_manatk,
    sfx_mandth,
    sfx_sssit,
    sfx_ssdth,
    sfx_keenpn,
    sfx_keendt,
    sfx_skeact,
    sfx_skesit,
    sfx_skeatk,
    sfx_radio,
    sfx_jump,
    sfx_ouch,
    sfx_gloop,
    sfx_splash,
    sfx_floush,
    sfx_amwtr1, // Tails
    sfx_amwtr2, // Tails
    sfx_amwtr3, // Tails
    sfx_amwtr4, // Tails
    sfx_amwtr5, // Tails
    sfx_amwtr6, // Tails
    sfx_amwtr7, // Tails
    sfx_amwtr8, // Tails
	sfx_splish, // Tails 12-08-2000
	sfx_wslap, // Water Slap Tails 12-13-2000
    sfx_steam1,
    sfx_steam2,
    sfx_crumbl,
    sfx_now,
    sfx_strpst,

	// free slots for S_AddSoundFx() at run-time --------------------
	sfx_freeslot0,
	//
	// ... 60 free sounds here ...
	//
	sfx_lastfreeslot = sfx_freeslot0 + NUMSFXFREESLOTS-1,
	// end of freeslots ---------------------------------------------

	sfx_skinsoundslot0,
	sfx_lastskinsoundslot = sfx_skinsoundslot0 + NUMSKINSFXSLOTS-1,
	NUMSFX
} sfxenum_t;


void S_InitRuntimeSounds(void);
int S_AddSoundFx(const char *name, int singularity, int pitch, boolean skinsound);
void S_RemoveSoundFx(int id);

#endif
