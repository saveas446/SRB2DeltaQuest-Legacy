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
    mus_runnin,
    mus_stalks,
    mus_countd,
    mus_betwee,
    mus_doom,
    mus_the_da,
    mus_shawn,
    mus_ddtblu,
    mus_in_cit,
    mus_dead,
    mus_stlks2,
    mus_theda2,
    mus_doom2,
    mus_ddtbl2,
    mus_runni2,
    mus_dead2,
    mus_stlks3,
    mus_romero,
    mus_shawn2,
    mus_messag,
    mus_count2,
    mus_ddtbl3,
    mus_ampie,
    mus_theda3,
    mus_adrian,
    mus_messg2,
    mus_romer2,
    mus_tense,
    mus_shawn3,
    mus_openin,
    mus_evil,
    mus_ultima,
    mus_dm2ttl, // Moved it up Tails 01-06-2001
    mus_read_m, // Story intermission Save 15-07-2025
    mus_dm2int, // Level clear! Save 15-07-2025
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

// List of sounds (don't modify this comment!)
typedef enum
{
	sfx_None,
	sfx_menu1,
	sfx_shotgn,
	sfx_chchng,
	sfx_plasma,
	sfx_spin,
	sfx_putput,
	sfx_pudpud,
	sfx_wtrdng,
	sfx_strpst,
	sfx_rlaunc,
	sfx_rxplod,
	sfx_dmpain,
	sfx_lvpass,
	sfx_firxpl,
	sfx_pstart,
	sfx_pstop,
	sfx_doropn,
	sfx_dorcls,
	sfx_stnmov,
	sfx_swtchn,
	sfx_swtchx,
	sfx_pop,
	sfx_itemup,
	sfx_wpnup,
	sfx_spring,
	sfx_telept,
	sfx_thok,
	sfx_cybdth,
	sfx_spndsh,
	sfx_zoom,
	sfx_tink,
	sfx_bdopn,
	sfx_bdcls,
	sfx_shield,
	sfx_bkpoof,
	sfx_radio,
	sfx_jump,
	sfx_gasp,
	//test water
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
	sfx_amwtr9, // Tails
	sfx_splish, // Tails 12-08-2000
	sfx_wslap, // Water Slap Tails 12-13-2000
	sfx_steam1, // Tails 06-19-2001
	sfx_steam2, // Tails 06-19-2001
	sfx_cgot, // Got Emerald! Tails 09-02-2001
	sfx_crumbl, // Platform Crumble Tails 03-16-2001

	// Ambient/Active lightning Tails 08-25-2002
	sfx_litng1,
	sfx_litng2,
	sfx_litng3,
	sfx_litng4,
	sfx_athun1,
	sfx_athun2,
	sfx_rainin,

	// Bubble pops Tails 08-25-2002
	sfx_bubbl1,
	sfx_bubbl2,
	sfx_bubbl3,
	sfx_bubbl4,
	sfx_bubbl5,

	// Teleporter mixup sound Tails 08-28-2002
	sfx_mixup,

	// Demo end talking Tails 09-01-2002
	sfx_annon1,
	sfx_annon2,
	sfx_annon3,
	sfx_annon4,
	sfx_annon5,

	// Some new sounds Tails 09-06-2002
	sfx_supert,
	sfx_lose,
	sfx_drown,
	sfx_spkdth,

	// Player taunts/alternate msgs Tails 09-06-2002
	sfx_taunt1,
	sfx_taunt2,
	sfx_taunt3,
	sfx_taunt4,

	sfx_altow1,
	sfx_altow2,
	sfx_altow3,
	sfx_altow4,

	sfx_victr1,
	sfx_victr2,
	sfx_victr3,
	sfx_victr4,

	sfx_altdi1,
	sfx_altdi2,
	sfx_altdi3,
	sfx_altdi4,

	sfx_shldls, // Lost shield Tails 09-06-2002

	// NiGHTS Sounds Tails
	sfx_drill1,
	sfx_drill2,
	sfx_hoop1,
	sfx_hoop2,
	sfx_hoop3,
	sfx_ncitem,
	sfx_ideya,
	sfx_hidden,
	sfx_nbmper,
	sfx_ngskid,
	sfx_prloop,
	sfx_nghurt,
	sfx_ngdone,

	sfx_pogo,
	sfx_gspray,
	sfx_ghit,

	sfx_alarm,

	sfx_fizzle,
	sfx_turpop,
	sfx_trfire,
	sfx_turhit,
	sfx_trpowr,

	sfx_mario1,
	sfx_mario2,
	sfx_mario3,
	sfx_mario4,
	sfx_mario5,
	sfx_mario6,
	sfx_mario7,
	sfx_mario8,
	sfx_mario9,
	sfx_marioa,
	sfx_thwomp,
	sfx_koopfr,

	sfx_ambint,
	sfx_sahitg,
	sfx_spdpad,

	sfx_ncspec, // Tails 12-15-2003

	sfx_wdjump, // Wind shield jump

	sfx_rumble, // Rumbling
	sfx_ambin2, // Another ambient sound

	sfx_deton, // Deton chase sound

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
