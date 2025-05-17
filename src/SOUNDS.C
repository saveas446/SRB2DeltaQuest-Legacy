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
    { "runnin"  , 0 },
    { "stalks"  , 0 },
    { "countd"  , 0 },
    { "betwee"  , 0 },
    { "doom\0\0", 0 },
    { "the_da"  , 0 },
    { "shawn\0" , 0 },
    { "ddtblu"  , 0 },
    { "in_cit"  , 0 },
    { "dead\0\0", 0 },
    { "stlks2"  , 0 },
    { "theda2"  , 0 },
    { "doom2\0" , 0 },
    { "ddtbl2"  , 0 },
    { "runni2"  , 0 },
    { "dead2\0" , 0 },
    { "stlks3"  , 0 },
    { "romero"  , 0 },
    { "shawn2"  , 0 },
    { "messag"  , 0 },
    { "count2"  , 0 },
    { "ddtbl3"  , 0 },
    { "ampie\0" , 0 },
    { "theda3"  , 0 },
    { "adrian"  , 0 },
    { "messg2"  , 0 },
    { "romer2"  , 0 },
    { "tense\0" , 0 },
    { "shawn3"  , 0 },
    { "openin"  , 0 },
    { "evil\0\0", 0 },
    { "ultima"  , 0 },
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

sfxinfo_t S_sfx[NUMSFX] =
{

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
  {"none" , false,   0,  0, -1, NULL, 0, -1,         -1, -1, NULL},

  {"menu1", true,   64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"shotgn", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"chchng", false, 120,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"plasma", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"spin" , false,  64,  0, -1, NULL, 0, SKSSPIN,    -1, -1, NULL},
  {"putput", false,  64,  0, -1, NULL, 0, SKSPUTPUT,  -1, -1, NULL}, // not as high a priority
  {"pudpud", false,  64,  0, -1, NULL, 0, SKSPUDPUD,  -1, -1, NULL},
  {"wtrdng", true,  120,  0, -1, NULL, 0, -1,         -1, -1, NULL}, // make sure you can hear the DING DING! Tails 03-08-2000
  {"strpst", true,  120,  0, -1, NULL, 0, -1,         -1, -1, NULL}, // Starpost Sound Tails 07-04-2002
  {"rlaunc", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"rxplod", false,  70,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"dmpain", false,  96,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"lvpass", false,  70,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"firxpl", false,  70,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"pstart", false, 100,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"pstop", false, 100,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"doropn", false, 100,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"dorcls", false, 100,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"stnmov", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"swtchn", false,  78,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"swtchx", false,  78,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"pop"  , false,  78,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"itemup", true,   78,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"wpnup", true,   78,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"spring", false,  96,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"telept", false,  32,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"thok" , false,  32,  0, -1, NULL, 0, SKSTHOK,    -1, -1, NULL},
  {"cybdth", false,  32,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"spndsh", false,  64,  1, -1, NULL, 0, SKSSPNDSH,  -1, -1, NULL},
  {"zoom" , false, 120,  1, -1, NULL, 0, SKSZOOM,    -1, -1, NULL},
  {"tink" , false,  60,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"bdopn", false, 100,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"bdcls", false, 100,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"shield", false,  60,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"bkpoof", false,  70,  8, -1, NULL, 0, -1,         -1, -1, NULL},
  {"radio", false,  60,  0, -1, NULL, 0, SKSRADIO,   -1, -1, NULL},

  //added : 22-02-98: sound when the player avatar jumps in air 'hmpf!'
  {"jump" , false,  60,  0, -1, NULL, 0, SKSJUMP,    -1, -1, NULL},
  {"gasp" , false,  64,  0, -1, NULL, 0, SKSGASP,    -1, -1, NULL},

  //added : 09-08-98 : test water sounds
  {"gloop", false,  60,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"splash", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"floush", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  //Ambient Water Sounds Tails 06-10-2000
  {"amwtr1", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"amwtr2", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"amwtr3", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"amwtr4", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"amwtr5", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"amwtr6", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"amwtr7", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"amwtr8", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"amwtr9", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"splish", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL}, // Splish Tails 12-08-2000
  {"wslap", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL}, // Water Slap Tails 12-13-2000
  {"steam1", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL}, // Tails 06-19-2001
  {"steam2", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL}, // Tails 06-19-2001
  {"cgot" , true,  120,  0, -1, NULL, 0, -1,         -1, -1, NULL}, // Got Emerald! Tails 09-02-2001
  {"crumbl", true,  127,  0, -1, NULL, 0, -1,         -1, -1, NULL}, // Platform Crumble Tails 03-16-2001

  // Ambient/Active lightning Tails 08-25-2002
  {"litng1", false, 126,  2, -1, NULL, 0, -1,         -1, -1, NULL},
  {"litng2", false, 126,  2, -1, NULL, 0, -1,         -1, -1, NULL},
  {"litng3", false, 126,  2, -1, NULL, 0, -1,         -1, -1, NULL},
  {"litng4", false, 126,  2, -1, NULL, 0, -1,         -1, -1, NULL},
  {"athun1", false, 126,  2, -1, NULL, 0, -1,         -1, -1, NULL},
  {"athun2", false, 126,  2, -1, NULL, 0, -1,         -1, -1, NULL},
  {"rainin", true,  127,  4, -1, NULL, 0, -1,         -1, -1, NULL},

  // Bubble pops Tails 08-25-2002
  {"bubbl1", false,  48,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"bubbl2", false,  48,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"bubbl3", false,  48,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"bubbl4", false,  48,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"bubbl5", false,  48,  0, -1, NULL, 0, -1,         -1, -1, NULL},

  // Teleporter mixup sound Tails 08-28-2002
  {"mixup", true,  127,  0, -1, NULL, 0, -1,         -1, -1, NULL},

  // Demo end talking Tails 09-01-2002
  {"annon1", true,  127,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"annon2", true,  127,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"annon3", true,  127,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"annon4", true,  127,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"annon5", true,  127,  0, -1, NULL, 0, -1,         -1, -1, NULL},

  // Some new sounds Tails 09-06-2002
  {"supert", false, 127,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"lose" , false, 127,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"drown", false, 127,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"spkdth", false, 127,  0, -1, NULL, 0, -1,         -1, -1, NULL},

  // Player taunts/alternate msgs Tails 09-06-2002
  {"taunt1", false,  48, 24, -1, NULL, 0, SKSPLTNT1,  -1, -1, NULL},
  {"taunt2", false,  48, 24, -1, NULL, 0, SKSPLTNT2,  -1, -1, NULL},
  {"taunt3", false,  48, 24, -1, NULL, 0, SKSPLTNT3,  -1, -1, NULL},
  {"taunt4", false,  48, 24, -1, NULL, 0, SKSPLTNT4,  -1, -1, NULL},

  {"altow1", false,  48, 16, -1, NULL, 0, SKSPLPAN1,  -1, -1, NULL},
  {"altow2", false,  48, 16, -1, NULL, 0, SKSPLPAN2,  -1, -1, NULL},
  {"altow3", false,  48, 16, -1, NULL, 0, SKSPLPAN3,  -1, -1, NULL},
  {"altow4", false,  48, 16, -1, NULL, 0, SKSPLPAN4,  -1, -1, NULL},

  {"victr1", false,  48, 16, -1, NULL, 0, SKSPLVCT1,  -1, -1, NULL},
  {"victr2", false,  48, 16, -1, NULL, 0, SKSPLVCT2,  -1, -1, NULL},
  {"victr3", false,  48, 16, -1, NULL, 0, SKSPLVCT3,  -1, -1, NULL},
  {"victr4", false,  48, 16, -1, NULL, 0, SKSPLVCT4,  -1, -1, NULL},

  {"altdi1", false,  64, 16, -1, NULL, 0, SKSPLDET1,  -1, -1, NULL},
  {"altdi2", false,  64, 16, -1, NULL, 0, SKSPLDET2,  -1, -1, NULL},
  {"altdi3", false,  64, 16, -1, NULL, 0, SKSPLDET3,  -1, -1, NULL},
  {"altdi4", false,  64, 16, -1, NULL, 0, SKSPLDET4,  -1, -1, NULL},

  {"shldls", false,  64,  0, -1, NULL, 0, -1,        -1, -1, NULL},

  {"drill1", false,  64,  0, -1, NULL, 0, -1,        -1, -1, NULL},
  {"drill2", false,  64,  0, -1, NULL, 0, -1,        -1, -1, NULL},

  {"hoop1", false,  64,  0, -1, NULL, 0, -1,        -1, -1, NULL},
  {"hoop2", false,  64,  0, -1, NULL, 0, -1,        -1, -1, NULL},
  {"hoop3", false,  64,  0, -1, NULL, 0, -1,        -1, -1, NULL},
  {"ncitem", false,  64,  0, -1, NULL, 0, -1,        -1, -1, NULL},

  {"ideya", false, 127,  0, -1, NULL, 0, -1,        -1, -1, NULL},
  {"hidden", false,  64,  0, -1, NULL, 0, -1,        -1, -1, NULL},
  {"nbmper", false, 127,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"ngskid", false,  64,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"prloop", false, 127,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"nghurt", false, 127,  0, -1, NULL, 0, -1,         -1, -1, NULL},
  {"ngdone", true,  127,  0, -1, NULL, 0, -1,         -1, -1, NULL},

  {"pogo" , false,  64,  0, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"gspray", false,  64,  0, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"ghit" , false,  64,  0, -1, NULL, 0,  -1,        -1, -1, NULL},

  {"alarm", false, 127,  0, -1, NULL, 0,  -1,        -1, -1, NULL},

  {"fizzle", false, 127,  8, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"pop"  , false, 127,  8, -1, NULL, 0,  -1,        -1, -1, NULL},

  {"trfire", true,  127,  8, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"turhit", false, 127,  8, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"trpowr", true,  127,  8, -1, NULL, 0,  -1,        -1, -1, NULL},

  {"mario1", false,  60,  0, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"mario2", false, 127,  0, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"mario3", false,  60,  0, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"mario4", true,   78,  0, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"mario5", false,  78,  0, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"mario6", false,  60,  0, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"mario7", false,  32,  0, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"mario8", false,  48,  8, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"mario9", true,  120,  0, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"marioa", true,  127,  0, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"thwomp", true,  127,  8, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"koopfr", true,  127,  8, -1, NULL, 0,  -1,        -1, -1, NULL},

  {"ambint", true,  127,  8, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"sahitg", false,  72,  0, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"spdpad", false, 127,  0, -1, NULL, 0,  -1,        -1, -1, NULL},

  {"ncspec", false,  64,  0, -1, NULL, 0,  -1,        -1, -1, NULL}, // Tails 12-15-2003
  {"wdjump", false,  60,  0, -1, NULL, 0,  -1,        -1, -1, NULL},

  {"rumble", false,  64, 24, -1, NULL, 0,  -1,        -1, -1, NULL},
  {"ambin2", false,  64,  8, -1, NULL, 0,  -1,        -1, -1, NULL},

  {"deton", true,    64,  0, -1, NULL, 0,  -1,        -1, -1, NULL},

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
