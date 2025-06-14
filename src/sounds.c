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
/// \brief music/sound tables, and related sound routines

#ifndef _MSC_VER
#include <stdint.h>      /* C99+ */
#endif
#include "doomtype.h"
#include "i_sound.h"
#include "sounds.h"
#include "r_defs.h"
#include "r_things.h"
#include "z_zone.h"
#include "w_wad.h"

//
// Information about all the music
//

































































































musicinfo_t S_music[NUMMUSIC] =
{
    { 0 },
/*    { "e1m1\0\0", 0 },
    { "e1m2\0\0", 0 },
    { "e1m3\0\0", 0 },
    { "e1m4\0\0", 0 },
    { "e1m5\0\0", 0 },
    { "e1m6\0\0", 0 },
    { "e1m7\0\0", 0 },
    { "e1m8\0\0", 0 },
    { "e1m9\0\0", 0 },
    { "e2m1\0\0", 0 },
    { "e2m2\0\0", 0 },
    { "e2m3\0\0", 0 },
    { "e2m4\0\0", 0 },
    { "e2m5\0\0", 0 },
    { "e2m6\0\0", 0 },
    { "e2m7\0\0", 0 },
    { "e2m8\0\0", 0 },
    { "e2m9\0\0", 0 },
    { "e3m1\0\0", 0 },
    { "e3m2\0\0", 0 },
    { "e3m3\0\0", 0 },
    { "e3m4\0\0", 0 },
    { "e3m5\0\0", 0 },
    { "e3m6\0\0", 0 },
    { "e3m7\0\0", 0 },
    { "e3m8\0\0", 0 },
    { "e3m9\0\0", 0 },
    { "inter\0" , 0 },
    { "intro\0" , 0 },
    { "bunny\0" , 0 },
    { "victor"  , 0 },
    { "introa"  , 0 },*/
    { "gfz1\0\0\0"  , 0 },
    { "gfz2\0\0\0"  , 0 },
    { "boss\0\0"  , 0 },
    { "thz1\0\0\0", 0 },
    { "thz2\0\0\0", 0 },
    { "boss\0\0"  , 0 },
    { "dsz1\0\0\0", 0 },
    { "dsz2\0\0\0", 0 },
    { "boss\0\0"  , 0 },
    { "mmz1\0\0\0", 0 },
    { "mmz2\0\0\0", 0 },
    { "boss\0\0"  , 0 },
    { "rmz1\0\0\0", 0 },
    { "rmz2\0\0\0", 0 },
    { "boss\0\0"  , 0 },
    { "rvz1\0\0\0", 0 },
    { "rvz2\0\0\0", 0 },
    { "boss\0\0"  , 0 },
    { "dcz1\0\0\0", 0 },
    { "dcz2\0\0\0", 0 },
    { "boss\0\0"  , 0 },
    { "gez1\0\0\0", 0 },
    { "gez2\0\0\0", 0 },
    { "boss\0\0"  , 0 },
    { "rrz1\0\0\0", 0 },
    { "rrz2\0\0\0", 0 },
    { "final\0\0" , 0 },    
    { "final2"    , 0 },    
    { "wood\0\0\0", 0 },
    { "dushil"    , 0 },
    { "spdhwy"    , 0 },
    { "hdnplc"    , 0 },
    { "swtth\0"   , 0 },
    { "crcave"    , 0 },
    { "eggfac"    , 0 },
    { "slmvly"    , 0 },
    { "fsz1\0\0\0", 0 },
    { "fszbos"    , 0 },
    // End of level tracks
    { "dm2ttl"  , 0 }, // Moved it up Tails 01-06-2001
    { "read_m"  , 0 },
    { "dm2int"  , 0 },
    { "invinc"  , 0 }, // invincibility Tails
    { "drown\0" , 0 }, // drowning Tails 03-06-2000
    { "gmover"  , 0 }, // Tails 03-14-2000
    { "xtlife"  , 0 }, // Tails 03-14-2000
};


//
// Information about all the sfx
//

/*****
    Legacy doesn't use the PITCH variable, so now it is used to set
	which sound effects shouldn't be played together. If their pitch
	matches, the sound that wants to play will overwrite the other one
	that is already playing.

    42 is a magic pitch number which says that more than one sfx of this kind
	can be played on an origin at once.
*****/
  // S_sfx[0] needs to be a dummy for odd reasons. (don't modify this comment)
//  name, singularity, priority, pitch, volume, data, length, skinsound, usefulness, lumpnum, zname

// 

sfxinfo_t S_sfx[NUMSFX] =
{
  // S_sfx[0] needs to be a dummy for odd reasons.
//         singularity(U)        pitch      skinsound   (U) for UNUSED
//               |      priority(U) volume  |
//               |        |  link   |   data|
  { "none"     , false,   0, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "pistol"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "shotgn"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "sgcock"  , false,  120, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "dshtgn"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "dbopn\0"  , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "dbcls\0"  , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "dbload"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  //{ "plasma"   , false,  64, 0, -1, -1, 0, -1, -1, -1, NULL},
  { "bfg\0\0\0", false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "sawup"    , false,  64, 0, -1, DATA_NONE, 0, SKSSPIN, -1, -1, NULL},
  { "sawidl"   , false,  64, 0, -1, DATA_NONE, 0, SKSPUTPUT, -1, -1, NULL}, // not as high a priority
  { "sawful"   , false,  64, 0, -1, DATA_NONE, 0, SKSPUDPUD, -1, -1, NULL},
  { "sawhit"   , true,  120, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL}, // make sure you can hear the DING DING! Tails 03-08-2000
  { "rlaunc"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "rxplod"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "firsht"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "firxpl"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "pstart"   , false, 100, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "pstop\0"  , false, 100, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "doropn"   , false, 100, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "dorcls"   , false, 100, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "stnmov"   ,  false, 64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "swtchn"   , false,  78, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "swtchx"   , false,  78, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "plpain"   , false,  96, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "dmpain"   , false,  96, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "popain"   , false,  96, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "vipain"   , false,  96, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "mnpain"   , false,  96, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "pepain"   , false,  96, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "slop\0\0" , false,  78, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "itemup"   ,  true,  78, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "wpnup"    ,  true,  78, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "oof"      , false,  96, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "telept"   , false,  32, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "posit1"   , true,  120, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "posit2"   , true,  120, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "posit3"   ,  true,  98, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bgsit1"   ,  true,  98, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bgsit2"   ,  true,  98, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "sgtsit"   ,  true,  98, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "cacsit"   ,  true,  98, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "brssit"   ,  true,  94, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "cybsit"   ,  true,  92, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "spisit"   ,  true,  90, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bspsit"   ,  true,  90, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "kntsit"   ,  true,  90, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "vilsit"   ,  true,  90, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "mansit"   ,  true,  90, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "pesit\0"  ,  true,  90, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "sklatk"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "sgtatk"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "skepch"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "vilatk"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "claw\0\0" , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "skeswg"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "pldeth"   , false,  32, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "pdiehi"   , false,  32, 0, -1, DATA_NONE, 0, SKSTHOK, -1, -1, NULL},
  { "podth1"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "podth2"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "podth3"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bgdth1"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bgdth2"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "sgtdth"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "cacdth"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "skldth"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "brsdth"   , false,  32, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "cybdth"   , false,  32, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "spidth"   , false,  32, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bspdth"   , false,  32, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "vildth"   , false,  32, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "kntdth"   , false,  32, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "pedth\0"  , false,  32, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "skedth"   , false,  32, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "posact"   ,  true, 120, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bgact\0"  ,  true, 120, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "dmact\0"  ,  true, 120, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bspact"   ,  true, 100, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bspwlk"   ,  true, 100, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "vilact"   ,  true, 100, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "noway"    , false,  78, 0, -1, DATA_NONE, 0, SKSSPNDSH, -1, -1, NULL},
  { "barexp"   , false,  60, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "punch\0"  , false,  64, 0, -1, DATA_NONE, 0, SKSZOOM, -1, -1, NULL},
  { "hoof\0\0" , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "metal\0"  , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "chgun\0"  , false,  64, /*&S_sfx[sfx_pistol]*/ 0, 150, 0, 0, -1, -1, -1, NULL},
  { "tink\0\0" , false,  60, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bdopn\0"  , false, 100, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bdcls\0"  , false, 100, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "itmbk\0"  , false, 100, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "flame\0"  , false,  32, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "flamst"   , false,  32, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "getpow"   , false,  60, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bospit"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "boscub"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bossit"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bospn\0"  , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "bosdth"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "manatk"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "mandth"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "sssit\0"  , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "ssdth\0"  , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "keenpn"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "keendt"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "skeact"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "skesit"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "skeatk"   , false,  70, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "radio\0"  , false,  60, 0, -1, DATA_NONE, 0, SKSRADIO, -1, -1, NULL},

  //added:22-02-98: sound when the player avatar jumps in air 'hmpf!'
  { "jump\0\0" , false,  60, 0, -1, DATA_NONE, 0, SKSJUMP, -1, -1, NULL},
  { "ouch\0\0" , false,  60, 0, -1, DATA_NONE, 0, SKSOUCH, -1, -1, NULL},
  { "gasp\0\0" , false,  64, 0, -1, DATA_NONE, 0, SKSGASP, -1, -1, NULL},

  //added:09-08-98:test water sounds
  { "gloop\0"  , false,  60, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "splash"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "floush"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  //Ambient Water Sounds Tails 06-10-2000
  { "amwtr1"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "amwtr2"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "amwtr3"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "amwtr4"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "amwtr5"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "amwtr6"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "amwtr7"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "amwtr8"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL},
  { "splish"   , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL}, // Splish Tails 12-08-2000
  { "wslap\0"  , false,  64, 0, -1, DATA_NONE, 0, -1, -1, -1, NULL}, // Water Slap Tails 12-13-2000
  // skin sounds free slots to add sounds at run time (Boris HACK!!!)
  // initialized to NULL
};

char freeslotnames[sfx_freeslot0 + NUMSFXFREESLOTS + NUMSKINSFXSLOTS][7];

// Prepare free sfx slots to add sfx at run time
void S_InitRuntimeSounds (void)
{
	sfxenum_t i;
	int value;
	char soundname[7];

	for (i = sfx_freeslot0; i <= sfx_lastskinsoundslot; i++)
	{
		value = (i+1) - sfx_freeslot0;

		if (value < 10)
			sprintf(soundname, "fre00%d", value);
		else if (value < 100)
			sprintf(soundname, "fre0%d", value);
		else if (value < 1000)
			sprintf(soundname, "fre%d", value);
		else
			sprintf(soundname, "fr%d", value);

		strcpy(freeslotnames[value-1], soundname);

		S_sfx[i].name = freeslotnames[value-1];
		S_sfx[i].singularity = false;
		S_sfx[i].priority = 0;
		S_sfx[i].pitch = 0;
		S_sfx[i].volume = -1;
		S_sfx[i].data = NULL;
		S_sfx[i].length = 0;
		S_sfx[i].skinsound = -1;
		S_sfx[i].usefulness = -1;
		S_sfx[i].lumpnum = -1;
		S_sfx[i].zname = NULL;
	}
}

// Add a new sound fx into a free sfx slot.
//
int S_AddSoundFx(const char *name, int singularity, int pitch,
	boolean skinsound)
{
	int i, slot;

	if (skinsound)
		slot = sfx_skinsoundslot0;
	else
		slot = sfx_freeslot0;

	for (i = slot; i < NUMSFX; i++)
	{
		if (!S_sfx[i].priority)
		{
			S_sfx[i].name = S_sfx[i].zname =
				(char *)Z_Malloc(7, PU_STATIC, NULL);
			strncpy(S_sfx[i].zname, name, 6);
			S_sfx[i].zname[6] = '\0';
			S_sfx[i].singularity = singularity;
			S_sfx[i].priority = 60;
			S_sfx[i].pitch = pitch;
			S_sfx[i].volume = -1;
			S_sfx[i].lumpnum = -1;
			S_sfx[i].skinsound = -1;
			S_sfx[i].usefulness = -1;

			/// \todo if precached load it here
			S_sfx[i].data = 0;
			return i;
		}
	}
	CONS_Printf("\2No more free sound slots\n");
	return 0;
}

void S_RemoveSoundFx(int id)
{
	if (id >= sfx_freeslot0 && id <= sfx_lastskinsoundslot
		&& S_sfx[id].zname != NULL)
	{
		Z_Free(S_sfx[id].zname);
		S_sfx[id].name = S_sfx[id].zname = NULL;
		S_sfx[id].lumpnum = -1;
		I_FreeSfx(&S_sfx[id]);
	}
}
