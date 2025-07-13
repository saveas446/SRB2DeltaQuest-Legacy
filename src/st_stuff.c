// Emacs style mode select   -*- C -*- 
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
//
// DESCRIPTION:
//      Status bar code.
//      Does the face/direction indicator animatin.
//      Does palette indicators as well (red pain/berserk, bright pickup)
//
//-----------------------------------------------------------------------------
/// \file
/// \brief Status bar code
/// 
///	Does the face/direction indicator animatin.
///	Does palette indicators as well (red pain/berserk, bright pickup)

#include "doomdef.h"
#include "g_game.h"
#include "r_local.h"
#include "p_local.h"
#include "f_finale.h"
#include "st_stuff.h"
#include "i_video.h"
#include "v_video.h"
#include "z_zone.h"
#include "hu_stuff.h"
#include "s_sound.h"
#include "i_system.h"
#include "m_menu.h"


#ifdef HWRENDER
#include "hardware/hw_main.h"
#endif

//
// STATUS BAR DATA
//

// Palette indices.
#define STARTBONUSPALS 9
#define NUMBONUSPALS 4

patch_t *tallnum[10]; // 0-9, tall numbers
static patch_t *nightsnum[10]; // NiGHTS timer numbers

patch_t *faceprefix[MAXSKINS]; // face status patches
patch_t *facenameprefix[MAXSKINS]; // face background

// ------------------------------------------
//             status bar overlay
// ------------------------------------------

// icons for overlay
patch_t *sboscore; // Score logo
patch_t *sbotime; // Time logo
patch_t *sbocolon; // Colon for time
static patch_t *sborings;
static patch_t *sboover;
static patch_t *timeover;
static patch_t *stlivex;
static patch_t *rrings;
static patch_t *getall; // Special Stage HUD
static patch_t *timeup; // Special Stage HUD
static patch_t *homing1; // Emerald hunt indicators
static patch_t *homing2; // Emerald hunt indicators
static patch_t *homing3; // Emerald hunt indicators
static patch_t *homing4; // Emerald hunt indicators
static patch_t *homing5; // Emerald hunt indicators
static patch_t *homing6; // Emerald hunt indicators
static patch_t *supersonic;
static patch_t *ttlnum;
static patch_t *nightslink;
static patch_t *count5;
static patch_t *count4;
static patch_t *count3;
static patch_t *count2;
static patch_t *count1;
static patch_t *count0;
static patch_t *jumpshield;
static patch_t *fireshield;
static patch_t *ringshield;
static patch_t *greenshield;
static patch_t *watershield;
static patch_t *bombshield;
static patch_t *invincibility;
static patch_t *sneakers;
static patch_t *rflagico;
static patch_t *bflagico;
static patch_t *gotrflag;
static patch_t *gotbflag;
static patch_t *nonicon;
static patch_t *bluestat;
static patch_t *byelstat;
static patch_t *orngstat;
static patch_t *redstat;
static patch_t *yelstat;
static patch_t *nbracket;
static patch_t *nhud[12];
static patch_t *narrow[9];
static patch_t *minicaps;
static patch_t *lvlttl1; // GFZ
static patch_t *lvlttl2; // THZ
static patch_t *lvlttl3; // DSZ
static patch_t *lvlttl4; // MMZ
static patch_t *lvlttl5; // RMZ
static patch_t *lvlttl6; // RVZ
static patch_t *lvlttl7; // DCZ
static patch_t *lvlttl8; // GEZ
static patch_t *lvlttl9; // RRZ
static patch_t *lvlttl10; // FFZ
static patch_t *lvlttl11; // WDZ
static patch_t *lvlttl12; // DHZ
static patch_t *lvlttl13; // SHZ
static patch_t *lvlttl14; // HPZ
static patch_t *lvlttl15; // STZ
static patch_t *lvlttl16; // CCZ
static patch_t *lvlttl17; // EFZ
static patch_t *lvlttl18; // SVZ
static patch_t *lvlttl19; // FSZ
static patch_t *ttlzone; // ZONE
static patch_t *act1;
static patch_t *act2;
static patch_t *act3;


static boolean facefreed[MAXPLAYERS];
static boolean prefixfreed[MAXPLAYERS];

boolean devmodedraw[NUMDEVMODEOPTIONS];

hudinfo_t hudinfo[NUMHUDITEMS] = 
{
	{	52,		192	},			// HUD_LIVESNAME
	{   16,		192	},			// HUD_LIVESPIC
	{   88,		192	},		// HUD_LIVESNUM
	{   56,		192	},		// HUD_LIVESX
	{   220,	10	},		// HUD_RINGSSPLIT
	{   288,	10	},		// HUD_RINGSNUMSPLIT
	{   16,		42	},			// HUD_RINGS
	{   112,	42	},		// HUD_RINGSNUM
	{   16,		10	},			// HUD_SCORE
	{   128,	10	},		// HUD_SCORENUM
	{   136,	10	},		// HUD_TIMESPLIT
	{   204,	10	},		// HUD_LOWSECONDSSPLIT
	{   212,	10	},		// HUD_SECONDSSPLIT
	{   188,	10	},		// HUD_MINUTESSPLIT
	{   188,	10	},		// HUD_TIMECOLONSPLIT
	{   17,		26	},			// HUD_TIME
	{   104,	26	},		// HUD_LOWSECONDS
	{   112,	26	},		// HUD_SECONDS
	{   88,		26	},			// HUD_MINUTES
	{   88,		26	},			// HUD_TIMECOLON
	{   288,	40	},		// HUD_SS_TOTALRINGS_SPLIT
	{   112,	56	},		// HUD_SS_TOTALRINGS
	{   110,	93	},		// HUD_GETRINGS
	{   160,	93	},		// HUD_GETRINGSNUM
	{   124,	160	},	// HUD_TIMELEFT
	{   168,	176	},	// HUD_TIMELEFTNUM
	{   130,	93	},		// HUD_TIMEUP
	{   132,	168	},	// HUD_HUNTPIC1
	{   152,	168	},	// HUD_HUNTPIC2
	{   172,	168	},	// HUD_HUNTPIC3
	{   224,	6	},		// HUD_LIGHTDASHBOX
	{   260,	8	},		// HUD_LIGHTDASHSTRING1
	{   260,	20	},		// HUD_LIGHTDASHSTRING2
};


// title card stuff

int ttlnumxpos,ttlnumypos, ttlnumscroll, ttlnumtargetscroll;
int lvlttlxpos,lvlttlypos, lvlttlscroll, lvlttltargetscroll;
int zonexpos,zoneypos, zonescroll, zonetargetscroll;

int actnum;

boolean nonumber = false;
boolean targetreached = false;

patch_t* lvlttl; // Patch that will point to whatever level title is appropriate for this level
patch_t* ttlnum;


//
// STATUS BAR CODE
//

boolean ST_SameTeam(player_t *a, player_t *b)
{
	if (gametype == GT_CTF)
		return a->skincolor == b->skincolor;

	switch (cv_teamplay.value)
	{
		case 0:
			return false;
		case 1:
			return a->skincolor == b->skincolor;
		case 2:
			return a->skin == b->skin;
	}
	return false;
}

static boolean st_stopped = true;

void ST_Ticker(void)
{
	if (st_stopped)
		return;
}

static int st_palette = 0;

void ST_doPaletteStuff(void)
{
	int palette;

	if (stplyr && stplyr->bonuscount)
	{
		palette = (stplyr->bonuscount+7)>>3;

		if (palette >= NUMBONUSPALS)
			palette = NUMBONUSPALS - 1;

		palette += STARTBONUSPALS;
	}
	else
		palette = 0;

	if (palette != st_palette)
	{
		st_palette = palette;

#ifdef HWRENDER
		if ((rendermode == render_opengl) || (rendermode == render_d3d))

		//faB - NOW DO ONLY IN SOFTWARE MODE, LETS FIX THIS FOR GOOD OR NEVER
		//      idea: use a true color gradient from frame to frame, because we
		//            are in true color in HW3D, we can have smoother palette change
		//            than the palettes defined in the wad

			HWR_SetPaletteColor(0x0);
		else
#endif
		if (rendermode != render_none)
		{
			// No more FLASHPAL, it doesn't exist in Demo 2.
			// Save 25-05-2025
			V_SetPaletteLump("PLAYPAL");

			if (!cv_splitscreen.value || !palette)
				V_SetPalette(palette);
		}
	}
}

static void ST_overlayDrawer(void);

void ST_Drawer(boolean refresh)
{
	// force a set of the palette by using doPaletteStuff()
	refresh = 0; //?
	if (vid.recalc)
		st_palette = -1;

	// Do red-/gold-shifts from damage/items
#ifdef HWRENDER
	//25/08/99: Hurdler: palette changes is done for all players,
	//                   not only player1! That's why this part
	//                   of code is moved somewhere else.
	if (rendermode == render_soft)
#endif
		if (rendermode != render_none) ST_doPaletteStuff();

	if (st_overlay)
	{
		// No deadview!
		stplyr = &players[displayplayer];
		ST_overlayDrawer();

		if (cv_splitscreen.value)
		{
			stplyr = &players[secondarydisplayplayer];
			ST_overlayDrawer();
		}
	}
}

static void ST_LoadGraphics(void)
{
	int i;
	char namebuf[9];

	// Load the numbers, tall and short
	for (i = 0; i < 10; i++)
	{
		sprintf(namebuf, "STTNUM%d", i);
		tallnum[i] = (patch_t *)W_CachePatchName(namebuf, PU_STATIC);
		sprintf(namebuf, "NGTNUM%d", i);
		nightsnum[i] = (patch_t *) W_CachePatchName(namebuf, PU_STATIC);
	}

	// the original Doom uses 'STF' as base name for all face graphics
	// Graue 04-08-2004: face/name graphics are now indexed by skins
	//                   but load them in R_AddSkins, that gets called
	//                   first anyway
}

// made separate so that skins code can reload custom face graphics
// Graue 04-07-2004: index by skins
void ST_LoadFaceGraphics(char *facestr, int skinnum)
{
	char namelump[9];

	// hack: make sure base face name is no more than 8 chars
	if (strlen(facestr) > 8)
		facestr[8] = '\0';
	strcpy(namelump, facestr); // copy base name

	faceprefix[skinnum] = W_CachePatchName(namelump, PU_STATIC);
	facefreed[skinnum] = false;
}

// Tails 03-15-2002
// made separate so that skins code can reload custom face graphics
// Graue 04-07-2004: index by skins
void ST_LoadFaceNameGraphics(char *facestr, int skinnum)
{
	char namelump[9];

	// hack: make sure base face name is no more than 8 chars
	if (strlen(facestr) > 8)
		facestr[8] = '\0';
	strcpy(namelump, facestr); // copy base name

	facenameprefix[skinnum] = W_CachePatchName(namelump, PU_STATIC);
	prefixfreed[skinnum] = false;
}

static inline void ST_InitData(void)
{
	// 'link' the statusbar display to a player, which could be
	// another player than consoleplayer, for example, when you
	// change the view in a multiplayer demo with F12.
	stplyr = &players[displayplayer];

	st_palette = -1;
}

static void ST_Stop(void)
{
	if (st_stopped)
		return;

	V_SetPalette(0);

	st_stopped = true;
}

boolean actdraw;

void ST_InitTitleCard(void)
{
	// shoot me in the head
	// this could really use some pointers or some shit like that but i don't care
	switch(gamemap) {
		case 1:
		case 2:
		case 3:
			lvlttl = lvlttl1;
			break;
		case 4:
		case 5:
		case 6:
			lvlttl = lvlttl2;
			break;
		case 7:
		case 8:
		case 9:
			lvlttl = lvlttl3;
			break;
		case 10:
		case 11:
		case 12:
			lvlttl = lvlttl4;
			break;
		case 13:
		case 14:
		case 15:
			lvlttl = lvlttl5;
			break;
		case 16:
		case 17:
		case 18:
			lvlttl = lvlttl6;
			break;
		case 19:
		case 20:
		case 21:
			lvlttl = lvlttl7;
			break;
		case 22:
		case 23:
		case 24:
			lvlttl = lvlttl8;
			break;
		case 25:
		case 26:
		case 27:
			lvlttl = lvlttl9;
			break;
		case 28:
			lvlttl = lvlttl10;
			break;
		case 29:
			lvlttl = lvlttl11;
			break;
		case 30:
			lvlttl = lvlttl12;
			break;
		case 31:
			lvlttl = lvlttl13;
			break;
		case 32:
			lvlttl = lvlttl14;
			break;
		case 33:
			lvlttl = lvlttl15;
			break;
		case 34:
			lvlttl = lvlttl16;
			break;
		case 35:
			lvlttl = lvlttl17;
			break;
		case 36:
			lvlttl = lvlttl18;
			break;
		case 37:
		case 38:
			lvlttl = lvlttl19;
			break;
		default:
			lvlttl = lvlttl1;
			break;
	}

	// horrifying mix of fd and demo 2 logic

	actnum = mapheaderinfo[gamemap - 1].actnum;

#ifdef NEWTITLECARDS
	if (gamemap == 1)
	lvlttlxpos = ((BASEVIDWIDTH/3) - (lvlttl->width/2));
	else
	lvlttlxpos = ((BASEVIDWIDTH/2) - (lvlttl->width/2));
#else
	lvlttlxpos = ((BASEVIDWIDTH/2) - (lvlttl->width/2));
#endif
	// Could probably just be done with noactdraw = (actnum > 0); or something
	if (actnum > 0)
		actdraw = true;
	else
		actdraw = false;

	ttlnumxpos = lvlttlxpos + lvlttl->width;
	zonexpos = ttlnumxpos - ttlzone->width;

	#ifdef NEWTITLECARDS
	if (gamemap == 1) {
		// I think this is how you deallocate things here?
		if (ttlnum != NULL)
			Z_ChangeTag(ttlnum, PU_CACHE);
		if (lvlttl != NULL)
			Z_ChangeTag(lvlttl, PU_CACHE);

		lvlttl = W_CachePatchName("TCGFZ", PU_LEVEL);
		ttlnum = W_CachePatchName("TCGFZ1", PU_LEVEL);
	} else {
	#endif
	if (!(actnum == 1 || actnum == 2 || actnum == 3)) {
		if (gamemap % 3 == 1)
			ttlnum = act1;
		else if (gamemap % 3 == 2)
			ttlnum = act2;
		else if (!(gamemap % 3))
			ttlnum = act3;
	} else {
		if (actnum == 1)
			ttlnum = act1;
		else if (actnum == 2)
			ttlnum = act2;
		else if (actnum == 3)
			ttlnum = act3;
		else
			actdraw = false;
	}
	#ifdef NEWTITLECARDS
	}
	#endif

	#ifdef NEWTITLECARDS
	if (gamemap == 1) {
		lvlttlypos = 40;
		zoneypos = 104;
		ttlnumypos = 104;
	
		lvlttlscroll = 120;
		zonescroll = 96;
		ttlnumscroll = 96;
	} else {
		#endif
		lvlttlypos = 80;
		zoneypos = 104;
		ttlnumypos = 104;

		lvlttlscroll = 80;
		zonescroll = 96;
		ttlnumscroll = 96;
	#ifdef NEWTITLECARDS
	}
	#endif
}

void ST_Start(void)
{

	ST_InitTitleCard();

	if (!st_stopped)
		ST_Stop();

	ST_InitData();
	st_stopped = false;
}

//
// Initializes the status bar, sets the defaults border patch for the window borders.
//

// used by Glide mode, holds lumpnum of flat used to fill space around the viewwindow
int st_borderpatchnum;

void ST_Init(void) {
	int i;

	for (i = 0; i < MAXPLAYERS; i++)
	{
		facefreed[i] = true;
		prefixfreed[i] = true;
	}

	if (dedicated)
		return;
	
	// screens[4] is allocated at videomode setup, and
	// set at V_Init(), the first time being at SCR_Recalc()

	// SRB2 border patch
	st_borderpatchnum = W_GetNumForName("FLOOR0_3");
	scr_borderpatch = W_CacheLumpNum(st_borderpatchnum, PU_STATIC);

	ST_LoadGraphics();

	// cache the status bar overlay icons (fullscreen mode)

	sborings = W_CachePatchName("SBOHEALT", PU_STATIC); // No more constantly red RINGS graphic! Save 24-05-2025
	sboscore = W_CachePatchName("SBOFRAGS", PU_STATIC); // There's a different name for this in Demo 2
	sboover = W_CachePatchName("SBOOVER", PU_STATIC);
	timeover = W_CachePatchName("TIMEOVER", PU_STATIC);
	stlivex = W_CachePatchName("STLIVEX", PU_STATIC);
	rrings = W_CachePatchName("SBORINGS", PU_STATIC); // There's a different name for this in Demo 2
	sbotime = W_CachePatchName("SBOTIME", PU_STATIC); // Time logo
	sbocolon = W_CachePatchName("SBOCOLON", PU_STATIC); // Colon for time
	getall = W_CachePatchName("GETALL", PU_STATIC); // Special Stage HUD
	timeup = W_CachePatchName("TIMEUP", PU_STATIC); // Special Stage HUD
	homing1	= W_CachePatchName("HOMING1", PU_STATIC); // Emerald hunt indicators
	homing2	= W_CachePatchName("HOMING2", PU_STATIC); // Emerald hunt indicators
	homing3	= W_CachePatchName("HOMING3", PU_STATIC); // Emerald hunt indicators
	homing4	= W_CachePatchName("HOMING4", PU_STATIC); // Emerald hunt indicators
	homing5	= W_CachePatchName("HOMING5", PU_STATIC); // Emerald hunt indicators
	homing6	= W_CachePatchName("HOMING6", PU_STATIC); // Emerald hunt indicators
	supersonic = W_CachePatchName("SUPERICO", PU_STATIC);
	nightslink = W_CachePatchName("NGHTLINK", PU_STATIC);
	count5 = W_CachePatchName("CNTFA0", PU_STATIC);
	count4 = W_CachePatchName("CNTEA0", PU_STATIC);
	count3 = W_CachePatchName("CNTDA0", PU_STATIC);
	count2 = W_CachePatchName("CNTCA0", PU_STATIC);
	count1 = W_CachePatchName("CNTBA0", PU_STATIC);
	count0 = W_CachePatchName("CNTAA0", PU_STATIC);

	jumpshield = W_CachePatchName("WHTVB0", PU_STATIC);
	fireshield = W_CachePatchName("RDTVB0", PU_STATIC);
	ringshield = W_CachePatchName("YLTVB0", PU_STATIC);
	greenshield = W_CachePatchName("GRTVB0", PU_STATIC);
	watershield = W_CachePatchName("BLTVB0", PU_STATIC);
	bombshield = W_CachePatchName("BKTVB0", PU_STATIC);
	invincibility = W_CachePatchName("PINVB0", PU_STATIC);
	sneakers = W_CachePatchName("SHTVB0", PU_STATIC);

	lvlttl1 = W_CachePatchName ("LVLTTL1", PU_STATIC);
	lvlttl2 = W_CachePatchName ("LVLTTL2", PU_STATIC);
	lvlttl3 = W_CachePatchName ("LVLTTL3", PU_STATIC);
	lvlttl4 = W_CachePatchName ("LVLTTL4", PU_STATIC);
	lvlttl5 = W_CachePatchName ("LVLTTL5", PU_STATIC);
	lvlttl6 = W_CachePatchName ("LVLTTL6", PU_STATIC);
	lvlttl7 = W_CachePatchName ("LVLTTL7", PU_STATIC);
	lvlttl8 = W_CachePatchName ("LVLTTL8", PU_STATIC);
	lvlttl9 = W_CachePatchName ("LVLTTL9", PU_STATIC);
	lvlttl10 = W_CachePatchName ("LVLTTL10", PU_STATIC);
	lvlttl11 = W_CachePatchName ("LVLTTL11", PU_STATIC);
	lvlttl12 = W_CachePatchName ("LVLTTL12", PU_STATIC);
	lvlttl13 = W_CachePatchName ("LVLTTL13", PU_STATIC);
	lvlttl14 = W_CachePatchName ("LVLTTL14", PU_STATIC);
	lvlttl15 = W_CachePatchName ("LVLTTL15", PU_STATIC);
	lvlttl16 = W_CachePatchName ("LVLTTL16", PU_STATIC);
	lvlttl17 = W_CachePatchName ("LVLTTL17", PU_STATIC);
	lvlttl18 = W_CachePatchName ("LVLTTL18", PU_STATIC);
	lvlttl19 = W_CachePatchName ("LVLTTL19", PU_STATIC);



	ttlzone = W_CachePatchName ("TTLZONE", PU_STATIC);

	act1 = W_CachePatchName ("TTLONE", PU_STATIC);
	act2 = W_CachePatchName ("TTLTWO", PU_STATIC);
	act3 = W_CachePatchName ("TTLTHREE", PU_STATIC);

	// NiGHTS HUD things
	bluestat = W_CachePatchName("BLUESTAT", PU_STATIC);
	byelstat = W_CachePatchName("BYELSTAT", PU_STATIC);
	orngstat = W_CachePatchName("ORNGSTAT", PU_STATIC);
	redstat = W_CachePatchName("REDSTAT", PU_STATIC);
	yelstat = W_CachePatchName("YELSTAT", PU_STATIC);
	nbracket = W_CachePatchName("NBRACKET", PU_STATIC);
	nhud[0] = W_CachePatchName("NHUD1", PU_STATIC);
	nhud[1] = W_CachePatchName("NHUD2", PU_STATIC);
	nhud[2] = W_CachePatchName("NHUD3", PU_STATIC);
	nhud[3] = W_CachePatchName("NHUD4", PU_STATIC);
	nhud[4] = W_CachePatchName("NHUD5", PU_STATIC);
	nhud[5] = W_CachePatchName("NHUD6", PU_STATIC);
	nhud[6] = W_CachePatchName("NHUD7", PU_STATIC);
	nhud[7] = W_CachePatchName("NHUD8", PU_STATIC);
	nhud[8] = W_CachePatchName("NHUD9", PU_STATIC);
	nhud[9] = W_CachePatchName("NHUD10", PU_STATIC);
	nhud[10] = W_CachePatchName("NHUD11", PU_STATIC);
	nhud[11] = W_CachePatchName("NHUD12", PU_STATIC);
	minicaps = W_CachePatchName("MINICAPS", PU_STATIC);

	narrow[0] = W_CachePatchName("NARROW1", PU_STATIC);
	narrow[1] = W_CachePatchName("NARROW2", PU_STATIC);
	narrow[2] = W_CachePatchName("NARROW3", PU_STATIC);
	narrow[3] = W_CachePatchName("NARROW4", PU_STATIC);
	narrow[4] = W_CachePatchName("NARROW5", PU_STATIC);
	narrow[5] = W_CachePatchName("NARROW6", PU_STATIC);
	narrow[6] = W_CachePatchName("NARROW7", PU_STATIC);
	narrow[7] = W_CachePatchName("NARROW8", PU_STATIC);

	// non-animated version
	narrow[8] = W_CachePatchName("NARROW9", PU_STATIC);
}

// change the status bar too, when pressing F12 while viewing a demo.
void ST_changeDemoView(void)
{
	// the same routine is called at multiplayer deathmatch spawn
	// so it can be called multiple times
	ST_Start();
}

// =========================================================================
//                         STATUS BAR OVERLAY
// =========================================================================

boolean st_overlay;

static int SCY(int y)
{
	//31/10/99: fixed by Hurdler so it _works_ also in hardware mode
	// do not scale to resolution for hardware accelerated
	// because these modes always scale by default
	y = (int)(y * vid.fdupy); // scale to resolution
	if (cv_splitscreen.value)
	{
		y >>= 1;
		if (stplyr != &players[displayplayer])
			y += vid.height / 2;
	}
	return y;
}

static int STRINGY(int y)
{
	//31/10/99: fixed by Hurdler so it _works_ also in hardware mode
	// do not scale to resolution for hardware accelerated
	// because these modes always scale by default
	if (cv_splitscreen.value)
	{
		y >>= 1;
		if (stplyr != &players[displayplayer])
			y += BASEVIDHEIGHT / 2;
	}
	return y;
}

static inline int SCX(int x)
{
	return (int)(x * vid.fdupx);
}

// Draw a number, scaled, over the view
// Always draw the number completely since it's overlay
//
static void ST_DrawOverlayNum(int x /* right border */, int y, int num,
	patch_t **numpat)
{
	int w = (numpat[0]->width);
	boolean neg;

	// special case for 0
	if (!num)
	{
		V_DrawScaledPatch(x - (w*vid.dupx), y, V_NOSCALESTART|V_TRANSLUCENT, numpat[0]);
		return;
	}

	neg = num < 0;

	if (neg)
		num = -num;

	// draw the number
	while (num)
	{
		x -= (w * vid.dupx);
		V_DrawScaledPatch(x, y, V_NOSCALESTART|V_TRANSLUCENT, numpat[num % 10]);
		num /= 10;
	}

	// draw a minus sign if necessary
	if (neg)
		V_DrawScaledPatch(x - (8*vid.dupx), y, V_NOSCALESTART|V_TRANSLUCENT,
			(patch_t *)W_CachePatchName("STTMINUS", PU_STATIC)); // Tails
}

// Draw a number, scaled, over the view
// Always draw the number completely since it's overlay
//
// Supports different colors! woo!
static void ST_DrawNightsOverlayNum(int x /* right border */, int y, int num,
	patch_t **numpat, int colornum)
{
	int w = (numpat[0]->width);
	byte *colormap;
	int flags = 0;

	if (colornum == 0)
		colormap = colormaps;
	else
	{
		// Uses the player colors.
		flags = (flags & ~MF_TRANSLATION) | (colornum<<MF_TRANSSHIFT);

		colormap = (byte *)defaulttranslationtables - 256
			+ ((flags & MF_TRANSLATION)>>(MF_TRANSSHIFT-8));
	}

	// special case for 0
	if (!num)
	{
		V_DrawMappedPatch(x - (w*vid.dupx), y, V_NOSCALESTART|V_TRANSLUCENT, numpat[0], colormap);
		return;
	}

	I_Assert(num >= 0); // this function does not draw negative numbers

	// draw the number
	while (num)
	{
		x -= (w * vid.dupx);
		V_DrawMappedPatch(x, y, V_NOSCALESTART|V_TRANSLUCENT, numpat[num % 10], colormap);
		num /= 10;
	}

	// Sorry chum, this function only draws UNSIGNED values!
}

char letters[] = {
	"ABCDEFGHIJKLMNOPQRSTU"
};

static void ST_drawDebugInfo(void)
{
	char d2uver[33];
	char smomx[33];
	char smomy[33];
	char smomz[33];
	char sspeed[33];
	char sfloorz[33];
	char spmomz[33];
	char scability[33];
	char scharsped[33];
	char scharflags[33];
	char sstrcolor[33];
	char sendcolor[33];
	char sdedtimer[33];
	char sjumpfact[33];
	char sx[33];
	char sy[33];
	char sz[33];
	char sangle[33];
	char sunderwater[33];
	char smfjumped[33];
	char smfspinning[33];
	char smfstartdash[33];
	char sjumping[33];
	char sscoreadd[33];
	char scomptime[33];
	char scompdate[33];
	char sbuildtype[33];

	char sgamemap[33];
	char sactnum[33];
	char szonename[33];

	char swindowsys[33];

	char ssingleplayer[33];
	char smatch[33];
	char sskynum[33];

	patch_t* emmypatch;

	char emmypatchname[9];

	int currentrow;

	sprintf(sgamemap, "Map: %d", gamemap);
	sprintf(sactnum, "Act: %d", mapheaderinfo[gamemap-1].actnum);
	sprintf(szonename, "Zone name: %s", mapheaderinfo[gamemap-1].lvlttl);
	sprintf(ssingleplayer, "Supports singleplayer: %d", mapheaderinfo[gamemap-1].typeoflevel & TOL_COOP);
	sprintf(smatch, "Supports match: %d", mapheaderinfo[gamemap-1].typeoflevel & TOL_MATCH);
	sprintf(sskynum, "Skynum: %d", mapheaderinfo[gamemap-1].skynum);

	sprintf(smomx, "MOMX: %d", stplyr->rmomx >> FRACBITS);
	sprintf(smomy, "MOMY: %d", stplyr->rmomy >> FRACBITS);
	sprintf(smomz, "MOMZ: %d", stplyr->mo->momz >> FRACBITS);
	/*sprintf(sspeed, "%d", stplyr->speed);
	sprintf(sfloorz, "%d", stplyr->mo->floorz >> FRACBITS);
	sprintf(spmomz, "%d", stplyr->mo->ceilingz >> FRACBITS);
	sprintf(scability, "%d", stplyr->charability);
	sprintf(scharsped, "%d", stplyr->normalspeed);
	sprintf(scharflags, "%d", stplyr->charflags);
	sprintf(sstrcolor, "%d", stplyr->starttranscolor);
	sprintf(sendcolor, "%d", stplyr->endtranscolor);
	sprintf(sdedtimer, "%d", stplyr->deadtimer);
	sprintf(sjumpfact, "%d", stplyr->jumpfactor);*/
	sprintf(sx, "X: %d", stplyr->mo->x >> FRACBITS);
	sprintf(sy, "Y: %d", stplyr->mo->y >> FRACBITS);
	sprintf(sz, "Z: %d", stplyr->mo->z >> FRACBITS);

	sprintf(d2uver, "DEMO 2 ULTIMATE v%d.%d.%d", MAJORVER, MINORVER, TINYVER);
	sprintf(scompdate, "Compiled on %s", __DATE__);
	sprintf(scomptime, "Compiled at %s", __TIME__);

	#if BUILD_TYPE == DEMO
	sprintf(sbuildtype, "Build type: Demo");
	#elif BUILD_TYPE == PLAYTEST_BUILD
	sprintf(sbuildtype, "Build type: Playtester build");
	#elif BUILD_TYPE == DEV_RELEASE
	sprintf(sbuildtype, "Build type: Dev build");
	#elif BUILD_TYPE == FULL_RELEASE
	sprintf(sbuildtype, "Build type: Full release");
	#endif

	/*#ifdef SDLIO
		sprintf(swindowsys, "Window system: SDL1");
	#else
		sprintf(swindowsys, "Window system: Win32");
	#endif*/

	sprintf(sangle, "Angle: %d", stplyr->mo->angle >> FRACBITS);
	//sprintf(sunderwater, "%d", stplyr->powers[pw_underwater]);
	sprintf(smfjumped, "MF_JUMPED: %d", stplyr->mfjumped);
	sprintf(smfspinning, "MF_SPINNING: %d", stplyr->mfspinning);
	sprintf(smfstartdash, "MF_STARDASH: %d", stplyr->mfstartdash);
	sprintf(sjumping, "Jumping: %d", stplyr->jumping);
	sprintf(sscoreadd, "Scoreadd: %d", stplyr->scoreadd);
	
	// Remnant of old string-based emerald drawing code
	/*sprintf(emmys, "%d", emeralds);
	sprintf(emmy1, "%d", emeralds & EMERALD1);
	sprintf(emmy2, "%d", emeralds & EMERALD2);
	sprintf(emmy3, "%d", emeralds & EMERALD3);
	sprintf(emmy4, "%d", emeralds & EMERALD4);
	sprintf(emmy5, "%d", emeralds & EMERALD5);
	sprintf(emmy6, "%d", emeralds & EMERALD6);
	sprintf(emmy7, "%d", emeralds & EMERALD7);*/
	

	/*V_DrawString(0, 240, 24, 0, "SPEED =");
	V_DrawString(0, 296, 24, 0, sspeed);*/

	currentrow = 0;

	// ALWAYS draw version number. No matter what.
	V_DrawString(0, 320 - (strlen(d2uver) << 3), currentrow, 0, d2uver);
	currentrow += 8;

	if (devmodedraw[dm_playermovebasic]) {
		// Player movement values (basic)
		V_DrawString(V_REDMAP, 320 - (strlen(sx) << 3), currentrow, 0, sx);
		currentrow += 8;
		V_DrawString(V_GREENMAP, 320 - (strlen(sy) << 3), currentrow, 0, sy);
		currentrow += 8;
		V_DrawString(V_BLUEMAP, 320 - (strlen(sz) << 3), currentrow, 0, sz);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(sangle) << 3), currentrow, V_YELLOWMAP, sangle);
		currentrow += 8;
	}

	if (devmodedraw[dm_playermoveadv]) {
		// Player movement values (advanced)
		V_DrawString(0, 320 - (strlen(smomx) << 3), currentrow, 0, smomx);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(smomy) << 3), currentrow, 0, smomy);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(smomz) << 3), currentrow, 0, smomz);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(smfjumped) << 3), currentrow, 0, smfjumped);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(smfspinning) << 3), currentrow, 0, smfspinning);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(smfstartdash) << 3), currentrow, 0, smfstartdash);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(sjumping) << 3), currentrow, 0, sjumping);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(sscoreadd) << 3), currentrow, 0, sscoreadd);
		currentrow += 8;
	}


	if (devmodedraw[dm_emmys]) {
		// Emerald display
		// Was GONNA make it animate but that didn't work :(
		printf(emmypatchname, "EMMYA0", leveltime % 21);

		emmypatch = W_CachePatchName(emmypatchname, PU_CACHE);
		
		V_DrawFlatFill(320-144, currentrow, 144, 32, W_GetNumForName("FLOOR0_3"));
		
		if (emeralds & EMERALD1)
			V_DrawSmallScaledPatch(320, currentrow+56, 0, emmypatch);
		if (emeralds & EMERALD2)
			V_DrawSmallScaledPatch(320-24, currentrow+56, 0, emmypatch);
		if (emeralds & EMERALD3)
			V_DrawSmallScaledPatch(320-48, currentrow+56, 0, emmypatch);
		if (emeralds & EMERALD4)
			V_DrawSmallScaledPatch(320-72, currentrow+56, 0, emmypatch);
		if (emeralds & EMERALD5)
			V_DrawSmallScaledPatch(320-96, currentrow+56, 0, emmypatch);
		if (emeralds & EMERALD6)
			V_DrawSmallScaledPatch(320-120, currentrow+56, 0, emmypatch);
		if (emeralds & EMERALD7)
			V_DrawSmallScaledPatch(320-144, currentrow+56, 0, emmypatch);	
			
		currentrow += 32;
	}

	if (devmodedraw[dm_version]) {
		// Detailed version info, like the version command
		V_DrawString(0, 320 - (strlen(scomptime) << 3), currentrow, 0, scomptime);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(scompdate) << 3), currentrow, 0, scompdate);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(sbuildtype) << 3), currentrow, 0, sbuildtype);
		currentrow += 8;
		/*V_DrawString(0, 320 - (strlen(swindowsys) << 3), currentrow, 0, swindowsys);
		currentrow += 8;*/
	}

	if (devmodedraw[dm_map]) {
		// Info about the map we're on
		V_DrawString(0, 320 - (strlen(sgamemap) << 3), currentrow, 0, sgamemap);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(szonename) << 3), currentrow, 0, szonename);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(sactnum) << 3), currentrow, 0, sactnum);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(ssingleplayer) << 3), currentrow, 0, ssingleplayer);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(smatch) << 3), currentrow, 0, smatch);
		currentrow += 8;
		V_DrawString(0, 320 - (strlen(sskynum) << 3), currentrow, 0, sskynum);
		currentrow += 8;
	}

	if (devmodedraw[dm_minimap]) {
		// Minimap
	}
	
	/*V_DrawString(0, 192, 152, 0, "Underwater =");
	V_DrawString(0, 288, 152, 0, sunderwater);*/
}

static void ST_drawEggsNeeded(void)
{
	int i;
	int numeggs = 0;

	if (timeinmap > 110)
		return;

	for (i = 0; i < NUMEGGS; i++)
	{
		if (egglocations[i].level == gamemap && !(foundeggs & 1<<i))
			numeggs++;
	}

	V_DrawCenteredString(BASEVIDWIDTH/2, BASEVIDHEIGHT/4*3, 0, va("Number of eggs left in this level: %d", numeggs));
}

static void ST_drawLevelTitle(void)
{
	int realypos;
	int temp, temp2;
	if (timeinmap == 0) {
		ST_InitTitleCard();
	}

	#ifdef NEWTITLECARDS
	if (gamemap == 1) {
		if (timeinmap < 105) {
			if (lvlttlscroll > 0)
				lvlttlscroll -= (lvlttlscroll >> 3);
		} else {
			if (lvlttlypos < 200)
				lvlttlypos += 12;
			else if (lvlttlypos > 200)
				lvlttlypos = 200;

			if (zoneypos > 0) 
				zoneypos -= 24;
			else if (zoneypos < 0)
				zoneypos = 0;

			if (ttlnumypos > 0)
				ttlnumypos -= 12;
			else if (ttlnumypos < 0)
				ttlnumypos = 0;
		}
	} else {
	#endif
		if (timeinmap < 105) {
			if (lvlttlscroll > 0)
				lvlttlscroll -= (lvlttlscroll >> 3);

			if (zonescroll > 0)
				zonescroll -= (zonescroll >> 3);

			if (lvlttlscroll > 0)
				lvlttlscroll -= (lvlttlscroll >> 3);
		} else {
			if (lvlttlypos < 250)
				lvlttlypos += 12;
			else if (lvlttlypos > 250)
				lvlttlypos = 250;

			if (zoneypos > 0) 
				zoneypos -= 24;
			else if (zoneypos < 0)
				zoneypos = 0;

			if (ttlnumypos > 0)
				ttlnumypos -= 12;
			else if (ttlnumypos < 0)
				ttlnumypos = 0;
		}
	#ifdef NEWTITLECARDS
	}
	#endif

	// fucking ass oml
	#ifdef NEWTITLECARDS
	if (gamemap == 1) {
		// Draw GFZROCK background
		/*for (temp = 0; temp < (vid.width >> 5); temp++) {
			for (temp2 = 0; temp2 < (vid.height >> 5); temp++) {
				V_DrawClippedScaledPatch (SCX(temp << 5),SCY(temp2 << 5), V_NOSCALESTART, W_CachePatchName("GFZROCKS", PU_CACHE));
			}
		}*/

		realypos = lvlttlypos - lvlttlscroll;
		V_DrawClippedScaledPatch (SCX(lvlttlxpos),SCY(realypos), V_NOSCALESTART, lvlttl);
		
		temp = realypos + (lvlttl->height >> 1);
		if (actdraw) 
			V_DrawClippedScaledPatch (SCX(ttlnumxpos + (ttlnumxpos >> 3)),SCY(temp), V_NOSCALESTART,ttlnum);
	} else {
	#endif
		realypos = lvlttlypos - lvlttlscroll;
		if (!strcmp("", mapheaderinfo[gamemap - 1].lvlttl)) {
			V_DrawClippedScaledPatch (SCX(lvlttlxpos),SCY(realypos), V_NOSCALESTART,lvlttl);
		} else {
			V_DrawLevelTitle(lvlttlxpos, realypos, 0, mapheaderinfo[gamemap - 1].lvlttl);
		}

		CONS_Printf("realypos: %d\n", realypos);


		realypos = zoneypos - zonescroll;
		V_DrawClippedScaledPatch (SCX(zonexpos),SCY(realypos), V_NOSCALESTART,ttlzone);
		if (actdraw) 
			V_DrawClippedScaledPatch (SCX(ttlnumxpos),SCY(realypos), V_NOSCALESTART,ttlnum);
		realypos = lvlttlypos - lvlttlscroll;
		if (!strcmp("", mapheaderinfo[gamemap - 1].lvlttl)) {
			V_DrawClippedScaledPatch (SCX(lvlttlxpos),SCY(realypos), V_NOSCALESTART,lvlttl);
		} else {
			V_DrawLevelTitle(lvlttlxpos, realypos, 0, mapheaderinfo[gamemap - 1].lvlttl);
		}


		realypos = zoneypos - zonescroll;	
		V_DrawClippedScaledPatch (SCX(zonexpos),SCY(realypos), V_NOSCALESTART,ttlzone);

		if (actdraw) 
			V_DrawClippedScaledPatch (SCX(ttlnumxpos),SCY(realypos), V_NOSCALESTART,ttlnum);

	#ifdef NEWTITLECARDS
	}
	#endif
}

static void ST_drawFirstPersonHUD(void)
{
	player_t *player = stplyr;
	fixed_t yheight;
	patch_t *p = NULL;

	yheight = SCY(120);
	/// \todo you wanna do something about those countdown drown numbers?

	// Graue 06-18-2004: no V_NOSCALESTART, no SCX, no SCY, snap to right
	if (player->powers[pw_jumpshield])
		V_DrawScaledPatch(304, STRINGY(32), V_SNAPTORIGHT|V_TRANSLUCENT, jumpshield);
	else if (player->powers[pw_fireshield])
		V_DrawScaledPatch(304, STRINGY(32), V_SNAPTORIGHT|V_TRANSLUCENT, fireshield);
	else if (player->powers[pw_watershield])
		V_DrawScaledPatch(304, STRINGY(32), V_SNAPTORIGHT|V_TRANSLUCENT, watershield);
	else if (player->powers[pw_bombshield])
		V_DrawScaledPatch(304, STRINGY(32), V_SNAPTORIGHT|V_TRANSLUCENT, bombshield);
	else if (player->powers[pw_ringshield])
		V_DrawScaledPatch(304, STRINGY(32), V_SNAPTORIGHT|V_TRANSLUCENT, ringshield);
		else if (player->powers[pw_greenshield])
		V_DrawScaledPatch(304, STRINGY(32), V_SNAPTORIGHT|V_TRANSLUCENT, greenshield);

	if ((player->powers[pw_invulnerability] > 3*TICRATE || (player->powers[pw_invulnerability]
		&& leveltime & 1)) || ((player->powers[pw_flashing] && leveltime & 1)))
		V_DrawScaledPatch(304, STRINGY(56), V_SNAPTORIGHT|V_TRANSLUCENT, invincibility);

	if (player->powers[pw_sneakers] > 3*TICRATE || (player->powers[pw_sneakers]
		&& leveltime & 1))
		V_DrawScaledPatch(304, STRINGY(80), V_SNAPTORIGHT|V_TRANSLUCENT, sneakers);

	// Display the countdown drown numbers!
	if ((player->powers[pw_underwater] <= 11*TICRATE + 1
		&& player->powers[pw_underwater] >= 10*TICRATE + 1)
		|| (player->powers[pw_spacetime] <= 11*TICRATE + 1
		&& player->powers[pw_spacetime] >= 10*TICRATE + 1))
	{
		p = count5;
	}
	else if ((player->powers[pw_underwater] <= 9*TICRATE + 1
		&& player->powers[pw_underwater] >= 8*TICRATE + 1)
		|| (player->powers[pw_spacetime] <= 9*TICRATE + 1
		&& player->powers[pw_spacetime] >= 8*TICRATE + 1))
	{
		p = count4;
	}
	else if ((player->powers[pw_underwater] <= 7*TICRATE + 1
		&& player->powers[pw_underwater] >= 6*TICRATE + 1)
		|| (player->powers[pw_spacetime] <= 7*TICRATE + 1
		&& player->powers[pw_spacetime] >= 6*TICRATE + 1))
	{
		p = count3;
	}
	else if ((player->powers[pw_underwater] <= 5*TICRATE + 1
		&& player->powers[pw_underwater] >= 4*TICRATE + 1)
		|| (player->powers[pw_spacetime] <= 5*TICRATE + 1
		&& player->powers[pw_spacetime] >= 4*TICRATE + 1))
	{
		p = count2;
	}
	else if ((player->powers[pw_underwater] <= 3*TICRATE + 1
		&& player->powers[pw_underwater] >= 2*TICRATE + 1)
		|| (player->powers[pw_spacetime] <= 3*TICRATE + 1
		&& player->powers[pw_spacetime] >= 2*TICRATE + 1))
	{
		p = count1;
	}
	else if ((player->powers[pw_underwater] <= 1*TICRATE + 1
		&& player->powers[pw_underwater] > 1)
		|| (player->powers[pw_spacetime] <= 1*TICRATE + 1
		&& player->powers[pw_spacetime] > 1))
	{
		p = count0;
	}

	if (p)
		V_DrawTranslucentPatch(SCX((BASEVIDWIDTH/2) - (p->width/2)), yheight,
			V_NOSCALESTART, p);
}

static void ST_drawNiGHTSHUD(void)
{
	if (stplyr->nightsmode)
	{
		int locx, locy;

		if (cv_splitscreen.value)
		{
			locx = 110;
			locy = 188;
		}
		else
		{
			locx = 16;
			locy = 144;
		}

		if (!(stplyr->drillmeter & 1))
		{
			V_DrawFill(locx-2, STRINGY(locy-2), 100, 8, 64);
			V_DrawFill(locx, STRINGY(locy), 96, 4, 0);
			V_DrawFill(locx, STRINGY(locy), stplyr->drillmeter/20, 4, 112);
		}
		else
		{
			V_DrawFill(locx-2, STRINGY(locy-2), 100, 8, 144);
			V_DrawFill(locx, STRINGY(locy), 96, 4, 88);
			V_DrawFill(locx, STRINGY(locy), stplyr->drillmeter/20, 4, 116);
		}
	}

	if (gametype == GT_RACE)
	{
		// draw score (same in splitscreen as normal, too!)
		ST_DrawOverlayNum(SCX(hudinfo[HUD_SCORENUM].x), SCY(hudinfo[HUD_SCORENUM].y), stplyr->score, tallnum);
		V_DrawScaledPatch(SCX(hudinfo[HUD_SCORE].x), SCY(hudinfo[HUD_SCORE].y), V_NOSCALESTART|V_TRANSLUCENT, sboscore);

		// Draw Time
		if (cv_splitscreen.value)
		{
			int seconds = stplyr->realtime/TICRATE % 60;

			if (seconds < 10)
				ST_DrawOverlayNum(SCX(hudinfo[HUD_LOWSECONDSSPLIT].x), SCY(hudinfo[HUD_LOWSECONDSSPLIT].y), 0, tallnum);

			// seconds time
			ST_DrawOverlayNum(SCX(hudinfo[HUD_SECONDSSPLIT].x), SCY(hudinfo[HUD_SECONDSSPLIT].y), stplyr->realtime/TICRATE % 60, tallnum);

			// minutes time
			ST_DrawOverlayNum(SCX(hudinfo[HUD_MINUTESSPLIT].x), SCY(hudinfo[HUD_MINUTESSPLIT].y), stplyr->realtime/(60*TICRATE), tallnum);

			// colon location
			V_DrawScaledPatch(SCX(hudinfo[HUD_TIMECOLONSPLIT].x), SCY(hudinfo[HUD_TIMECOLONSPLIT].y), V_NOSCALESTART|V_TRANSLUCENT, sbocolon);

			// TIME location
			V_DrawScaledPatch(SCX(hudinfo[HUD_TIMESPLIT].x), SCY(hudinfo[HUD_TIMESPLIT].y), V_NOSCALESTART|V_TRANSLUCENT, sbotime);
		}
		else
		{
			if (cv_timetic.value) // show tics instead of MM : SS
				ST_DrawOverlayNum(SCX(hudinfo[HUD_SECONDS].x), SCY(hudinfo[HUD_SECONDS].y), stplyr->realtime, tallnum);
			else
			{
				int seconds = stplyr->realtime/TICRATE % 60;

				if (seconds < 10)
					ST_DrawOverlayNum(SCX(hudinfo[HUD_LOWSECONDS].x), SCY(hudinfo[HUD_LOWSECONDS].y), 0, tallnum);

				// seconds time
				ST_DrawOverlayNum(SCX(hudinfo[HUD_SECONDS].x), SCY(hudinfo[HUD_SECONDS].y), stplyr->realtime/TICRATE % 60, tallnum);

				// minutes time
				ST_DrawOverlayNum(SCX(hudinfo[HUD_MINUTES].x), SCY(hudinfo[HUD_MINUTES].y), stplyr->realtime/(60*TICRATE), tallnum);

				// colon location
				V_DrawScaledPatch(SCX(hudinfo[HUD_TIMECOLON].x), SCY(hudinfo[HUD_TIMECOLON].y), V_NOSCALESTART|V_TRANSLUCENT, sbocolon);
			}

			// TIME location
			V_DrawScaledPatch(SCX(hudinfo[HUD_TIME].x), SCY(hudinfo[HUD_TIME].y), V_NOSCALESTART|V_TRANSLUCENT, sbotime);
		}

		return;
	}

	if (stplyr->bonustime > 1)
		V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(100), 0, "BONUS TIME START!");

	V_DrawScaledPatch(SCX(16), SCY(8), V_NOSCALESTART|V_TRANSLUCENT, nbracket);
	V_DrawScaledPatch(SCX(24), (int)(SCY(8) + 8*vid.fdupy), V_NOSCALESTART|V_TRANSLUCENT, nhud[(leveltime/2)%12]);

	if (stplyr->capsule)
	{
		int amount;
		int origamount;
		const int length = 88;

		V_DrawScaledPatch(SCX(72), SCY(8), V_NOSCALESTART|V_TRANSLUCENT, nbracket);
		V_DrawScaledPatch(SCX(74), (int)(SCY(8) + 4*vid.fdupy), V_NOSCALESTART|V_TRANSLUCENT,
			minicaps);

		if (stplyr->capsule->reactiontime != 0)
		{
			int r;
			const int orblength = 20;

			for (r = 0; r < 5; r++)
			{
				V_DrawScaledPatch(SCX(230 - (7*r)), SCY(144), V_NOSCALESTART|V_TRANSLUCENT,
					redstat);
				V_DrawScaledPatch(SCX(188 - (7*r)), SCY(144), V_NOSCALESTART|V_TRANSLUCENT,
					orngstat);
				V_DrawScaledPatch(SCX(146 - (7*r)), SCY(144), V_NOSCALESTART|V_TRANSLUCENT,
					yelstat);
				V_DrawScaledPatch(SCX(104 - (7*r)), SCY(144), V_NOSCALESTART|V_TRANSLUCENT,
					byelstat);
			}
			origamount = stplyr->capsule->spawnpoint->angle & 1023;

			amount = (origamount - stplyr->capsule->health);

			amount = (amount * orblength)/origamount;

			if (amount > 0)
			{
				int t;

				// Fill up the bar with blue orbs... in reverse! (yuck)
				for (r = amount; r >= 0; r--)
				{
					t = r;

					if (r > 14)
						t += 1;
					if (r > 9)
						t += 1;
					if (r > 4)
						t += 1;

					V_DrawScaledPatch(SCX(76 + (7*t)), SCY(144), V_NOSCALESTART|V_TRANSLUCENT,
						bluestat);
				}
			}
		}
		else
		{
			// Lil' white box!
			V_DrawFill(15, STRINGY(8) + 34, length + 2, 5, 4);
			V_DrawFill(16, STRINGY(8)+35, length/4, 3, 231);
			V_DrawFill(16 + length/4, STRINGY(8) + 35, length/4, 3, 248);
			V_DrawFill(16 + (length/4)*2, STRINGY(8) + 35, length/4, 3, 215);
			V_DrawFill(16 + (length/4)*3, STRINGY(8) + 35, length/4, 3, 179);
			origamount = stplyr->capsule->spawnpoint->angle & 1023;

			if (origamount <= 0)
				I_Error("Give the egg capsule on mare %d a ring requirement.\n", stplyr->capsule->threshold);

			amount = (origamount - stplyr->capsule->health);
			amount = (amount * length)/origamount;

			if (amount > 0)
				V_DrawFill(16, STRINGY(8) + 35, amount, 3, 197);
		}
		V_DrawScaledPatch(SCX(40), (int)(SCY(8) + 5*vid.fdupy), V_NOSCALESTART|V_TRANSLUCENT, narrow[(leveltime/2)%8]);
	}
	else
		V_DrawScaledPatch(SCX(40), (int)(SCY(8) + 5*vid.fdupy), V_NOSCALESTART|V_TRANSLUCENT, narrow[8]);

	ST_DrawOverlayNum(SCX(68), (int)(SCY(8) + 11*vid.fdupy), stplyr->health > 0 ? stplyr->health - 1 : 0, tallnum);
	ST_DrawNightsOverlayNum(SCX(288), SCY(12), stplyr->score, nightsnum, 7); // Blue

	if (stplyr->nightstime > 0)
	{
		int numbersize;

		if (stplyr->nightstime < 10)
			numbersize = SCX(16)/2;
		else if (stplyr->nightstime < 100)
			numbersize = SCX(32)/2;
		else
			numbersize = SCX(48)/2;

		if (stplyr->nightstime < 10)
			ST_DrawNightsOverlayNum(SCX(160) + numbersize, SCY(32), stplyr->nightstime,
				nightsnum, 6); // Red
		else
			ST_DrawNightsOverlayNum(SCX(160) + numbersize, SCY(32), stplyr->nightstime,
				nightsnum, MAXSKINCOLORS-1); // Yellow
	}
}

static void ST_drawTagHUD(void)
{
	if (stplyr->tagit)
	{
		char stagit[33];
		sprintf(stagit, "%d", stplyr->tagit/TICRATE);

		if (cv_splitscreen.value)
			V_DrawString(0, 120, STRINGY(168), 0, "YOU'RE IT!");
		else
			V_DrawString(0, 120, STRINGY(176), 0, "YOU'RE IT!");

		V_DrawString(0, 158 - (V_StringWidth(stagit))/2, STRINGY(184), 0, stagit);
	}

	if (stplyr->tagzone)
	{
		char stagzone[33];
		sprintf(stagzone, "%d", stplyr->tagzone/TICRATE);
		if (cv_splitscreen.value)
		{
			V_DrawString(0, 201, STRINGY(168), 0, "IN NO-TAG ZONE");
			V_DrawString(0, 254 - (V_StringWidth(stagzone))/2, STRINGY(184), 0, stagzone);
		}
		else
		{
			V_DrawString(0, 104, STRINGY(160), 0, "IN NO-TAG ZONE");
			V_DrawString(0, 158 - (V_StringWidth(stagzone))/2, STRINGY(168), 0, stagzone);
		}
	}
	else if (stplyr->taglag)
	{
		char staglag[33];
		sprintf(staglag, "%d", stplyr->taglag/TICRATE);

		if (cv_splitscreen.value)
		{
			V_DrawString(0, 30, STRINGY(168), 0, "NO-TAG LAG");
			V_DrawString(0, 62 - (V_StringWidth(staglag))/2, STRINGY(184), 0, staglag);
		}
		else
		{
			V_DrawString(0, 120, STRINGY(160), 0, "NO-TAG LAG");
			V_DrawString(0, 158 - (V_StringWidth(staglag))/2, STRINGY(168), 0, staglag);
		}
	}
}

static void ST_drawCTFHUD(void)
{
	int i, team;
	unsigned short whichflag;
	team = whichflag = 0;

	for (i = 0; i < MAXPLAYERS; i++)
	{
		if (players[i].gotflag & MF_REDFLAG)
		{
			team = players[i].ctfteam;
			whichflag = players[i].gotflag;
			break; // break, don't continue.
		}
	}

	// Draw the flags
	if (cv_splitscreen.value)
	{
		V_DrawSmallScaledPatch(256, STRINGY(160), 0, rflagico);
		V_DrawSmallScaledPatch(288, STRINGY(160), 0, bflagico);
	}
	else
	{
		V_DrawSmallScaledPatch(256, STRINGY(176), 0, rflagico);
		V_DrawSmallScaledPatch(288, STRINGY(176), 0, bflagico);
	}

	if (stplyr->ctfteam != team && team > 0 && ((stplyr->ctfteam == 1 && whichflag & MF_REDFLAG)
		|| (stplyr->ctfteam == 2 && whichflag & MF_BLUEFLAG)))
	{
		int x;

		if (whichflag & MF_REDFLAG)
			x = 256;
		else
			x = 288;

		// OTHER TEAM HAS YOUR FLAG!
		if (cv_splitscreen.value)
			V_DrawScaledPatch(x, STRINGY(156), 0, nonicon);
		else
			V_DrawScaledPatch(x, STRINGY(156+16), 0, nonicon);
	}
	else if (stplyr->ctfteam == team && team > 0)
	{
		int x;

		if (whichflag & MF_REDFLAG)
			x = 256;
		else
			x = 288;

		// YOUR TEAM HAS ENEMY FLAG!
		if (cv_splitscreen.value)
			V_DrawScaledPatch(x, STRINGY(156), 0, nonicon);
		else
			V_DrawScaledPatch(x, STRINGY(156+16), 0, nonicon);
	}

	team = whichflag = 0;

	for (i = 0; i < MAXPLAYERS; i++)
	{
		if (players[i].gotflag & MF_BLUEFLAG)
		{
			team = players[i].ctfteam;
			whichflag = players[i].gotflag;
			break; // break, don't continue.
		}
	}
	if (stplyr->ctfteam != team && team > 0 && ((stplyr->ctfteam == 1 && whichflag & MF_REDFLAG)
		|| (stplyr->ctfteam == 2 && whichflag & MF_BLUEFLAG)))
	{
		int x;

		if (whichflag & MF_REDFLAG)
			x = 256;
		else
			x = 288;

		// OTHER TEAM HAS YOUR FLAG!
		if (cv_splitscreen.value)
			V_DrawScaledPatch(x, STRINGY(156), 0, nonicon);
		else
			V_DrawScaledPatch(x, STRINGY(156+16), 0, nonicon);
	}
	else if (stplyr->ctfteam == team && team > 0)
	{
		int x;

		if (whichflag & MF_REDFLAG)
			x = 256;
		else
			x = 288;

		// YOUR TEAM HAS ENEMY FLAG!
		if (cv_splitscreen.value)
			V_DrawScaledPatch(x, STRINGY(156), 0, nonicon);
		else
			V_DrawScaledPatch(x, STRINGY(156+16), 0, nonicon);
	}

	if (stplyr->gotflag & MF_REDFLAG)
	{
		// YOU HAVE THE RED FLAG
		if (cv_splitscreen.value)
			V_DrawScaledPatch(224, STRINGY(160), 0, gotrflag);
		else
			V_DrawScaledPatch(224, STRINGY(176), 0, gotrflag);
	}
	else if (stplyr->gotflag & MF_BLUEFLAG)
	{
		// YOU HAVE THE BLUE FLAG
		if (cv_splitscreen.value)
			V_DrawScaledPatch(224, STRINGY(160), 0, gotbflag);
		else
			V_DrawScaledPatch(224, STRINGY(176), 0, gotbflag);
	}
	if (stplyr->ctfteam == 1)
	{
		if (cv_splitscreen.value)
			V_DrawString(0, 256, STRINGY(184), V_TRANSLUCENT, "RED TEAM");
		else
			V_DrawString(0, 256, STRINGY(192), V_TRANSLUCENT, "RED TEAM");
	}
	else if (stplyr->ctfteam == 2)
	{
		if (cv_splitscreen.value)
			V_DrawString(0, 248, STRINGY(184), V_TRANSLUCENT, "BLUE TEAM");
		else
			V_DrawString(0, 248, STRINGY(192), V_TRANSLUCENT, "BLUE TEAM");
	}
	else
	{
		V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(84), V_TRANSLUCENT, "You are a spectator.");
		V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(100), V_TRANSLUCENT, "Jump on a team base to choose.");
		V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(116), V_TRANSLUCENT, "(Press Fire to Respawn)");
	}
}

#ifdef CHAOSISNOTDEADYET
static void ST_drawChaosHUD(void)
{
	char chains[33];
	sprintf(chains, "CHAINS: %d", stplyr->scoreadd);
	V_DrawString(0, 8, STRINGY(184), V_TRANSLUCENT, chains);
}
#endif

static void ST_drawSpecialStageHUD(void)
{
	if (totalrings > 0)
	{
		if (cv_splitscreen.value)
			ST_DrawOverlayNum(SCX(hudinfo[HUD_SS_TOTALRINGS_SPLIT].x), SCY(hudinfo[HUD_SS_TOTALRINGS_SPLIT].y), totalrings, tallnum);
		else
			ST_DrawOverlayNum(SCX(hudinfo[HUD_SS_TOTALRINGS].x), SCY(hudinfo[HUD_SS_TOTALRINGS].y), totalrings, tallnum);
	}

	if (leveltime < 5*TICRATE && totalrings > 0)
	{
		V_DrawScaledPatch(hudinfo[HUD_GETRINGS].x, (int)(SCY(hudinfo[HUD_GETRINGS].y)/vid.fdupy), V_TRANSLUCENT, getall);
		ST_DrawOverlayNum(SCX(hudinfo[HUD_GETRINGSNUM].x), SCY(hudinfo[HUD_GETRINGSNUM].y), totalrings, tallnum);
	}

	if (sstimer)
	{
		V_DrawString(0, hudinfo[HUD_TIMELEFT].x, STRINGY(hudinfo[HUD_TIMELEFT].y), 0, "TIME LEFT");
		ST_DrawNightsOverlayNum(SCX(hudinfo[HUD_TIMELEFTNUM].x), SCY(hudinfo[HUD_TIMELEFTNUM].y), sstimer/TICRATE, tallnum, 13);
	}
	else
		V_DrawScaledPatch(SCX(hudinfo[HUD_TIMEUP].x), SCY(hudinfo[HUD_TIMEUP].y), V_NOSCALESTART|V_TRANSLUCENT, timeup);
}

static void ST_drawEmeraldHuntIcon(mobj_t *hunt, int graphic)
{
	patch_t *p;
	int interval;
	fixed_t dist = P_AproxDistance(P_AproxDistance(stplyr->mo->x - hunt->x, stplyr->mo->y - hunt->y),
		stplyr->mo->z - hunt->z);

	if (dist < 128<<FRACBITS)
	{
		p = homing6;
		interval = 5;
	}
	else if (dist < 512<<FRACBITS)
	{
		p = homing5;
		interval = 10;
	}
	else if (dist < 1024<<FRACBITS)
	{
		p = homing4;
		interval = 20;
	}
	else if (dist < 2048<<FRACBITS)
	{
		p = homing3;
		interval = 30;
	}
	else if (dist < 3072<<FRACBITS)
	{
		p = homing2;
		interval = 35;
	}
	else
	{
		p = homing1;
		interval = 0;
	}

	V_DrawScaledPatch(hudinfo[graphic].x, STRINGY(hudinfo[graphic].y), V_TRANSLUCENT, p);
	if (interval > 0 && leveltime % interval == 0)
		S_StartSound(0, sfx_shotgn);
}

// Draw the status bar overlay, customisable: the user chooses which
// kind of information to overlay
//
/// \todo: Split up this 1400 line function into multiple functions!
//
static void ST_overlayDrawer(void)
{
	int splity;
	double temp, temp2;
	char* creditstxt;

	if (cv_splitscreen.value)
		splity = 24;
	else
		splity = 0;

	if (stplyr->linkcount > 1)
	{
		int colornum;

		colornum = ((stplyr->linkcount-1) / 5)%14;

		if (cv_splitscreen.value)
		{
			ST_DrawNightsOverlayNum(SCX(256), SCY(160), (stplyr->linkcount-1), nightsnum, colornum);
			V_DrawMappedPatch(SCX(264), SCY(160), V_NOSCALESTART, nightslink,
				colornum == 0 ? colormaps : (byte *)defaulttranslationtables - 256 + ((((0 & ~MF_TRANSLATION)
				| (colornum<<MF_TRANSSHIFT)) & MF_TRANSLATION) >> (MF_TRANSSHIFT-8)));
		}
		else
		{
			ST_DrawNightsOverlayNum(SCX(160), SCY(176), (stplyr->linkcount-1), nightsnum, colornum);
			V_DrawMappedPatch(SCX(168), SCY(176), V_NOSCALESTART, nightslink,
				colornum == 0 ? colormaps : (byte *)defaulttranslationtables - 256 + ((((0 & ~MF_TRANSLATION)
				| (colornum<<MF_TRANSSHIFT)) & MF_TRANSLATION) >> (MF_TRANSSHIFT-8)));
		}
	}

	if (gamemap == 39) {
		// Credits text, used for Credits Zone
		creditstxt = "Insert credits here!";


        temp = (double)(leveltime);

		// No bit shifting with floats :(
		temp2 = sin(temp / 8) * 32;

		V_DrawString(0, 160 - (strlen(creditstxt) << 2), 100 - ((int)temp2), V_YELLOWMAP, creditstxt);
		// Y = 100 - (sin(leveltime) * 32)
	} else {
		// lives status
		if ((gametype == GT_COOP || gametype == GT_RACE))
		{
			V_DrawScaledPatch(SCX(hudinfo[HUD_LIVESNAME].x), SCY(hudinfo[HUD_LIVESNAME].y) - (faceprefix[stplyr->skin]->height*vid.dupy),
				V_NOSCALESTART|V_TRANSLUCENT, facenameprefix[stplyr->skin]);

			if ((stplyr->powers[pw_super] && stplyr->skin == 0) || stplyr->nightsmode) // SONIC ONLY
				V_DrawScaledPatch(SCX(hudinfo[HUD_LIVESPIC].x), SCY(hudinfo[HUD_LIVESPIC].y) - (faceprefix[stplyr->skin]->height*vid.dupy),
				V_NOSCALESTART|V_TRANSLUCENT, supersonic);
			else
			{
				if (!stplyr->skincolor) // 'default' color
				{
					V_DrawScaledPatch(SCX(hudinfo[HUD_LIVESPIC].x), SCY(hudinfo[HUD_LIVESPIC].y) - (faceprefix[stplyr->skin]->height*vid.dupy),
						V_NOSCALESTART|V_TRANSLUCENT,faceprefix[stplyr->skin]);
				}
				else
				{
					byte *colormap = (byte *)translationtables[stplyr->skin] - 256 + (((stplyr->powers[pw_super] && stplyr->supercolor > 0) ? stplyr->supercolor : stplyr->skincolor)<<8);
					V_DrawMappedPatch(SCX(hudinfo[HUD_LIVESPIC].x), SCY(hudinfo[HUD_LIVESPIC].y) - (faceprefix[stplyr->skin]->height*vid.dupy),
						V_NOSCALESTART|V_TRANSLUCENT,faceprefix[stplyr->skin], colormap);
				}
			}

			// draw the number of lives
			ST_DrawOverlayNum(SCX(hudinfo[HUD_LIVESNUM].x), SCY(hudinfo[HUD_LIVESNUM].y) - (11*vid.dupy), stplyr->lives, tallnum);

			// now draw the "x"
			if (cv_splitscreen.value)
				V_DrawScaledPatch(SCX(hudinfo[HUD_LIVESX].x), SCY(hudinfo[HUD_LIVESX].y-16), V_NOSCALESTART|V_TRANSLUCENT, stlivex);
			else
				V_DrawScaledPatch(SCX(hudinfo[HUD_LIVESX].x), SCY(hudinfo[HUD_LIVESX].y) - (stlivex->height*vid.dupy),
					V_NOSCALESTART|V_TRANSLUCENT, stlivex);
		}

		if (maptol & TOL_NIGHTS)
		{
			ST_drawNiGHTSHUD();
		}
		else
		{
			if (cv_splitscreen.value)
			{
				// rings counter
				ST_DrawOverlayNum(SCX(hudinfo[HUD_RINGSNUMSPLIT].x), SCY(hudinfo[HUD_RINGSNUMSPLIT].y), stplyr->health > 0 ? stplyr->health - 1 : 0,
					tallnum);

				if (stplyr->health <= 1 && leveltime/5 & 1)
					V_DrawScaledPatch(SCX(hudinfo[HUD_RINGSSPLIT].x), SCY(hudinfo[HUD_RINGSSPLIT].y), V_NOSCALESTART|V_TRANSLUCENT, rrings);
				else
					V_DrawScaledPatch(SCX(hudinfo[HUD_RINGSSPLIT].x), SCY(hudinfo[HUD_RINGSSPLIT].y), V_NOSCALESTART|V_TRANSLUCENT, sborings);
			}
			else
			{
				if (gamemap >= sstage_start && gamemap <= sstage_end)
				{
					int ringscollected = 0; // Total # everyone has collected
					int i;

					for (i = 0; i < MAXPLAYERS; i++)
						if (playeringame[i] && players[i].mo && players[i].mo->health > 1)
							ringscollected += players[i].mo->health - 1;

					ST_DrawOverlayNum(SCX(hudinfo[HUD_RINGSNUM].x), SCY(hudinfo[HUD_RINGSNUM].y), ringscollected, tallnum);
				}
				else
				{
					ST_DrawOverlayNum(SCX(hudinfo[HUD_RINGSNUM].x), SCY(hudinfo[HUD_RINGSNUM].y), stplyr->health > 0 ? stplyr->health-1 : 0,
						tallnum);
				}

				if (stplyr->health <= 1 && leveltime/5 & 1)
					V_DrawScaledPatch(SCX(hudinfo[HUD_RINGS].x), SCY(hudinfo[HUD_RINGS].y), V_NOSCALESTART|V_TRANSLUCENT, rrings);
				else
					V_DrawScaledPatch(SCX(hudinfo[HUD_RINGS].x), SCY(hudinfo[HUD_RINGS].y), V_NOSCALESTART|V_TRANSLUCENT, sborings);
			}

			// draw score (same in splitscreen as normal, too!)
			ST_DrawOverlayNum(SCX(hudinfo[HUD_SCORENUM].x), SCY(hudinfo[HUD_SCORENUM].y), stplyr->score, tallnum);
			V_DrawScaledPatch(SCX(hudinfo[HUD_SCORE].x), SCY(hudinfo[HUD_SCORE].y), V_NOSCALESTART|V_TRANSLUCENT, sboscore);

			if (cv_splitscreen.value)
			{
				int seconds = stplyr->realtime/TICRATE % 60;

				if (seconds < 10)
					ST_DrawOverlayNum(SCX(hudinfo[HUD_LOWSECONDSSPLIT].x), SCY(hudinfo[HUD_LOWSECONDSSPLIT].y), 0, tallnum);

				// seconds time
				ST_DrawOverlayNum(SCX(hudinfo[HUD_SECONDSSPLIT].x), SCY(hudinfo[HUD_SECONDSSPLIT].y), stplyr->realtime/TICRATE % 60, tallnum);

				// minutes time
				ST_DrawOverlayNum(SCX(hudinfo[HUD_MINUTESSPLIT].x), SCY(hudinfo[HUD_MINUTESSPLIT].y), stplyr->realtime/(60*TICRATE), tallnum);

				// colon location
				V_DrawScaledPatch(SCX(hudinfo[HUD_TIMECOLONSPLIT].x), SCY(hudinfo[HUD_TIMECOLONSPLIT].y), V_NOSCALESTART|V_TRANSLUCENT, sbocolon);

				// TIME location
				V_DrawScaledPatch(SCX(hudinfo[HUD_TIMESPLIT].x), SCY(hudinfo[HUD_TIMESPLIT].y), V_NOSCALESTART|V_TRANSLUCENT, sbotime);
			}
			else
			{
				if (cv_timetic.value) // show tics instead of MM : SS
					ST_DrawOverlayNum(SCX(hudinfo[HUD_SECONDS].x), SCY(hudinfo[HUD_SECONDS].y), stplyr->realtime, tallnum);
				else
				{
					int seconds = stplyr->realtime/TICRATE % 60;

					if (seconds < 10)
						ST_DrawOverlayNum(SCX(hudinfo[HUD_LOWSECONDS].x), SCY(hudinfo[HUD_LOWSECONDS].y), 0, tallnum);

					// seconds time
					ST_DrawOverlayNum(SCX(hudinfo[HUD_SECONDS].x), SCY(hudinfo[HUD_SECONDS].y), stplyr->realtime/TICRATE % 60, tallnum);

					// minutes time
					ST_DrawOverlayNum(SCX(hudinfo[HUD_MINUTES].x), SCY(hudinfo[HUD_MINUTES].y), stplyr->realtime/(60*TICRATE), tallnum);

					// colon location
					V_DrawScaledPatch(SCX(hudinfo[HUD_TIMECOLON].x), SCY(hudinfo[HUD_TIMECOLON].y), V_NOSCALESTART|V_TRANSLUCENT, sbocolon);
				}

				// TIME location
				V_DrawScaledPatch(SCX(hudinfo[HUD_TIME].x), SCY(hudinfo[HUD_TIME].y), V_NOSCALESTART|V_TRANSLUCENT, sbotime);
			}
		}

		// GAME OVER pic
		if ((gametype == GT_COOP || gametype == GT_RACE) && stplyr->lives <= 0)
		{
			patch_t *p;

			if (countdown == 1)
				p = timeover;
			else
				p = sboover;

			V_DrawScaledPatch((BASEVIDWIDTH - p->width)/2, STRINGY(BASEVIDHEIGHT/2 - (p->height/2)), 0, p);
		}

		if (cv_objectplace.value && stplyr->mo && stplyr->mo->target)
		{
			char x[8], y[8], z[8];
			char doomednum[8], thingflags[8];
			sprintf(x, "%d", stplyr->mo->x >> FRACBITS);
			sprintf(y, "%d", stplyr->mo->y >> FRACBITS);
			sprintf(z, "%d", stplyr->mo->z >> FRACBITS);
			sprintf(doomednum, "%d", stplyr->mo->target->info->doomednum);
			sprintf(thingflags, "%d", cv_objflags.value);
			V_DrawString(0, 16, 98, 0, "X =");
			V_DrawString(0, 48, 98, 0, x);
			V_DrawString(0, 16, 108, 0, "Y =");
			V_DrawString(0, 48, 108, 0, y);
			V_DrawString(0, 16, 118, 0, "Z =");
			V_DrawString(0, 48, 118, 0, z);
			V_DrawString(0, 16, 128, 0, "thing # =");
			V_DrawString(0, 16+84, 128, 0, doomednum);
			V_DrawString(0, 16, 138, 0, "flags =");
			V_DrawString(0, 16+56, 138, 0, thingflags);
			V_DrawString(0, 16, 148, 0, "snap =");
			V_DrawString(0, 16+48, 148, 0, cv_snapto.string);
		}

		// Countdown timer for Race Mode
		if (countdown)
		{
			char scountdown[33];
			sprintf(scountdown, "%d", countdown/TICRATE);
			V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(176), 0, scountdown);
		}

		// This is where we draw all the fun cheese if you have the chasecam off!
		if ((stplyr == &players[consoleplayer] && !cv_chasecam.value)
			|| ((cv_splitscreen.value && stplyr == &players[secondarydisplayplayer])
			&& !cv_chasecam2.value))
		{
			ST_drawFirstPersonHUD();
		}

		// draw level title Tails
		ST_drawLevelTitle();

		if (netgame && gametype == GT_RACE && stplyr->lives <= 0 && displayplayer == consoleplayer && countdown != 1)
			V_DrawCenteredString(BASEVIDWIDTH/2, BASEVIDHEIGHT/3, 0, "Press F12 to watch another player.");

		if (stplyr->deadtimer > 0 && (stplyr->deadtimer < gameovertics) && stplyr->lives <= 0)
		{
			if (!netgame && !multiplayer)
			{
				// No continues here. Save 17-05-2025
				Command_ExitGame_f();
			}
		}
	}

	if (cv_debug)
		ST_drawDebugInfo();
}
