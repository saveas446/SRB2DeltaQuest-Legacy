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
/// \brief Implements special effects:
///	 - Texture animation, height or lighting changes
///	 according to adjacent sectors, respective
///	 utility functions, etc.

#ifndef __P_SPEC__
#define __P_SPEC__

// at game start
void P_InitPicAnims(void);

// at map load (sectors)
void P_SetupLevelFlatAnims(void);

// at map load
void P_SpawnSpecials(void);

// every tic
void P_UpdateSpecials(void);
void P_PlayerInSpecialSector(player_t *player);

int twoSided(int sector, int line);
sector_t *getSector(int currentSector, int line, int side);
side_t *getSide(int currentSector, int line, int side);

fixed_t P_FindLowestFloorSurrounding(sector_t *sec);
fixed_t P_FindHighestFloorSurrounding(sector_t *sec);

fixed_t P_FindNextHighestFloor(sector_t *sec, int currentheight);
fixed_t P_FindNextLowestFloor(sector_t *sec, int currentheight);

fixed_t P_FindLowestCeilingSurrounding(sector_t *sec);
fixed_t P_FindHighestCeilingSurrounding(sector_t *sec);

int P_FindSectorFromLineTag(line_t *line, int start);
int P_FindSectorFromTag(int tag, int start);
int P_FindLineFromLineTag(const line_t *line, int start);
int P_FindSpecialLineFromTag(short special, short tag, int start);
int P_FindLineFromTag(int tag, int start);
int P_FindMinSurroundingLight(sector_t *sector, int max);

void P_SetupSignExit(player_t *player);

sector_t *getNextSector(line_t *line, sector_t *sec);

fixed_t P_FindNextHighestCeiling(sector_t *sec, int currentheight);
fixed_t P_FindNextLowestCeiling(sector_t *sec, int currentheight);
fixed_t P_FindShortestUpperAround(int secnum);
fixed_t P_FindShortestTextureAround(int secnum);
sector_t *P_FindModelFloorSector(fixed_t floordestheight, int secnum);
sector_t *P_FindModelCeilingSector(fixed_t ceildestheight, int secnum);

void P_SwitchWeather(int weathernum);

void P_LinedefExecute(int tag, mobj_t *actor, sector_t *caller);
void P_ProcessLineSpecial(line_t *line, mobj_t *mo);
void P_ChangeSectorTag(int sector, int newtag);

//
// P_LIGHTS
//
/** Fire flicker action structure.
  */
typedef struct
{
	thinker_t thinker; ///< The thinker in use for the effect.
	sector_t *sector;  ///< The sector where action is taking place.
	int count;
	int resetcount;
	int maxlight;      ///< The brightest light level to use.
	int minlight;      ///< The darkest light level to use.
} fireflicker_t;

typedef struct
{
	thinker_t thinker;
	sector_t *sector;
	int maxlight;
	int minlight;
} lightflash_t;

/** Laser block thinker.
  */
typedef struct
{
	thinker_t thinker; ///< Thinker structure for laser.
	ffloor_t *ffloor;  ///< 3Dfloor that is a laser.
	sector_t *sector;  ///< Sector in which the effect takes place.
} laserthink_t;

/** Strobe light action structure..
  */
typedef struct
{
	thinker_t thinker; ///< The thinker in use for the effect.
	sector_t *sector;  ///< The sector where the action is taking place.
	int count;
	int minlight;      ///< The minimum light level to use.
	int maxlight;      ///< The maximum light level to use.
	int darktime;      ///< How long to use minlight.
	int brighttime;    ///< How long to use maxlight.
} strobe_t;

typedef struct
{
	thinker_t thinker;
	sector_t *sector;
	int minlight;
	int maxlight;
	int direction;
	int speed;
} glow_t;

/** Thinker struct for fading lights.
  */
typedef struct
{
	thinker_t thinker; ///< Thinker in use for the effect.
	sector_t *sector;  ///< Sector where action is taking place.
	int destlevel;     ///< Light level we're fading to.
	int speed;         ///< Speed at which to change light level.
} lightlevel_t;

#define GLOWSPEED 8
#define STROBEBRIGHT 5
#define FASTDARK 15
#define SLOWDARK 35

void T_FireFlicker(fireflicker_t *flick);
void P_SpawnFireFlicker(sector_t *sector);
fireflicker_t *P_SpawnAdjustableFireFlicker(sector_t *minsector, sector_t *maxsector, int length);
void T_LightningFlash(lightflash_t *flash);
void T_StrobeFlash(strobe_t *flash);

void P_SpawnLightningFlash(sector_t *sector);
void P_SpawnStrobeFlash(sector_t *sector, int fastOrSlow, int inSync);

void T_Glow(glow_t *g);
void P_SpawnGlowingLight(sector_t *sector);
glow_t *P_SpawnAdjustableGlowingLight(sector_t *minsector, sector_t *maxsector, int length);

void P_FadeLight(int tag, int destvalue, int speed);
void T_LightFade(lightlevel_t *ll);

typedef enum
{
	floor_special,
	ceiling_special,
	lighting_special,
} special_e;

//
// P_CEILNG
//
typedef enum
{
	raiseToHighest,
	lowerToLowest,
	raiseToLowest,
	lowerToLowestFast,

	instantRaise, // instant-move for ceilings

	lowerAndCrush,
	crushAndRaise,
	fastCrushAndRaise,

	moveCeilingByFrontSector,
	instantMoveCeilingByFrontSector,

	lowerCeilingByLine,
	raiseCeilingByLine,

	bounceCeiling,
	bounceCeilingCrush,
} ceiling_e;

/** Ceiling movement structure.
  */
typedef struct
{
	thinker_t thinker;    ///< Thinker for the type of movement.
	ceiling_e type;       ///< Type of movement.
	sector_t *sector;     ///< Sector where the action is taking place.
	fixed_t bottomheight; ///< The lowest height to move to.
	fixed_t topheight;    ///< The highest height to move to.
	fixed_t speed;        ///< Ceiling speed.
	fixed_t oldspeed;
	boolean crush;        ///< Whether to crush things or not.

	fixed_t texture;      ///< The number of a flat to use when done.
	int direction;        ///< 1 = up, 0 = waiting, -1 = down.

	// ID
	int tag;
	int olddirection;

	struct ceilinglist *list;

	fixed_t origspeed;    ///< The original, "real" speed.
} ceiling_t;

#define CEILSPEED (FRACUNIT/NEWTICRATERATIO)

int EV_DoCeiling(line_t *line, ceiling_e type);

int EV_DoCrush(line_t *line, ceiling_e type);
void T_CrushCeiling(ceiling_t *ceiling);

void T_MoveCeiling(ceiling_t *ceiling);

//
// P_FLOOR
//
typedef enum
{
	// lower floor to lowest surrounding floor
	lowerFloorToLowest,

	// raise floor to next highest surrounding floor
	raiseFloorToNearestFast,

	// move the floor down instantly
	instantLower,

	moveFloorByFrontSector,
	instantMoveFloorByFrontSector,

	lowerFloorByLine,
	raiseFloorByLine,

	bounceFloor,
	bounceFloorCrush,
} floor_e;

typedef enum
{
	elevateUp,
	elevateDown,
	elevateCurrent,
	elevateContinuous,
	elevateBounce,
	elevateHighest,
	bridgeFall,
} elevator_e;

typedef struct
{
	thinker_t thinker;
	floor_e type;
	boolean crush;
	sector_t *sector;
	int direction;
	fixed_t texture;
	fixed_t floordestheight;
	fixed_t speed;
	fixed_t origspeed;
} floormove_t;

typedef struct
{
	thinker_t thinker;
	elevator_e type;
	sector_t *sector;
	sector_t *actionsector; // The sector the rover action is taking place in.
	int direction;
	fixed_t floordestheight;
	fixed_t ceilingdestheight;
	fixed_t speed;
	fixed_t origspeed;
	fixed_t low;
	fixed_t high;
	fixed_t distance;
	fixed_t floorwasheight; // Height the floor WAS at
	fixed_t ceilingwasheight; // Height the ceiling WAS at
	player_t *player; // Player who initiated the thinker (used for airbob)
	line_t *sourceline;
} elevator_t;

#define ELEVATORSPEED (FRACUNIT*4/NEWTICRATERATIO)
#define FLOORSPEED (FRACUNIT/NEWTICRATERATIO)

typedef enum
{
	ok,
	crushed,
	pastdest
} result_e;


#define DAMAGE_MASK     0x60
#define DAMAGE_SHIFT    5
#define SECRET_MASK     0x80
#define SECRET_SHIFT    7
#define FRICTION_MASK   0x100
#define FRICTION_SHIFT  8
#define PUSH_MASK       0x200
#define PUSH_SHIFT      9

//jff 02/04/98 Define masks, shifts, for fields in 
// generalized linedef types

//
// P_DOORS
//

//SoM: 3/6/2000
int P_SectorActive
( special_e t,
  sector_t* s );
  
typedef enum
{
    door_normal,
    close30ThenOpen,
    doorclose,
    dooropen,
    raiseIn5Mins,
    blazeRaise,
    blazeOpen,
    blazeClose,

    //SoM: 3/4/2000: General door types...
    genRaise,
    genBlazeRaise,
    genOpen,
    genBlazeOpen,
    genClose,
    genBlazeClose,
    genCdO,
    genBlazeCdO,
} vldoor_e;



typedef struct
{
    thinker_t   thinker;
    vldoor_e    type;
    sector_t*   sector;
    fixed_t     topheight;
    fixed_t     speed;

    // 1 = up, 0 = waiting at top, -1 = down
    int             direction;

    // tics to wait at the top
    int             topwait;
    // (keep in case a door going down is reset)
    // when it reaches 0, start going down
    int             topcountdown;

    //SoM: 3/6/2000: the line that triggered the door.
    line_t *line;
} vldoor_t;



#define VDOORSPEED              (FRACUNIT*2)
#define VDOORWAIT               150

int //SoM: 3/6/2000: boom support
EV_VerticalDoor
( line_t*       line,
  mobj_t*       thing );

int
EV_DoDoor
( line_t*       line,
  vldoor_e      type );

int
EV_DoLockedDoor
( line_t*       line,
  vldoor_e      type,
  mobj_t*       thing );

void    T_VerticalDoor (vldoor_t* door);
void    P_SpawnDoorCloseIn30 (sector_t* sec);

void
P_SpawnDoorRaiseIn5Mins
( sector_t*     sec,
  int           secnum );



#if 0 // UNUSED
//
//      Sliding doors...
//
typedef enum
{
    sd_opening,
    sd_waiting,
    sd_closing

} sd_e;



typedef enum
{
    sdt_openOnly,
    sdt_closeOnly,
    sdt_openAndClose

} sdt_e;




typedef struct
{
    thinker_t   thinker;
    sdt_e       type;
    line_t*     line;
    int         frame;
    int         whichDoorIndex;
    int         timer;
    sector_t*   frontsector;
    sector_t*   backsector;
    sd_e         status;

} slidedoor_t;



typedef struct
{
    char        frontFrame1[9];
    char        frontFrame2[9];
    char        frontFrame3[9];
    char        frontFrame4[9];
    char        backFrame1[9];
    char        backFrame2[9];
    char        backFrame3[9];
    char        backFrame4[9];

} slidename_t;



typedef struct
{
    int             frontFrames[4];
    int             backFrames[4];

} slideframe_t;



// how many frames of animation
#define SNUMFRAMES              4

#define SDOORWAIT               (35*3)
#define SWAITTICS               4

// how many diff. types of anims
#define MAXSLIDEDOORS   5

void P_InitSlidingDoorFrames(void);

void
EV_SlidingDoor
( line_t*       line,
  mobj_t*       thing );
#endif


#define GenFloorBase          0x6000
#define GenCeilingBase        0x4000
#define GenDoorBase           0x3c00
#define GenLockedBase         0x3800
#define GenLiftBase           0x3400
#define GenStairsBase         0x3000
#define GenCrusherBase        0x2F80

#define TriggerType           0x0007
#define TriggerTypeShift      0

// define masks and shifts for the floor type fields

#define FloorCrush            0x1000
#define FloorChange           0x0c00
#define FloorTarget           0x0380
#define FloorDirection        0x0040
#define FloorModel            0x0020
#define FloorSpeed            0x0018

#define FloorCrushShift           12
#define FloorChangeShift          10
#define FloorTargetShift           7
#define FloorDirectionShift        6
#define FloorModelShift            5
#define FloorSpeedShift            3
                               
// define masks and shifts for the ceiling type fields

#define CeilingCrush          0x1000
#define CeilingChange         0x0c00
#define CeilingTarget         0x0380
#define CeilingDirection      0x0040
#define CeilingModel          0x0020
#define CeilingSpeed          0x0018

#define CeilingCrushShift         12
#define CeilingChangeShift        10
#define CeilingTargetShift         7
#define CeilingDirectionShift      6
#define CeilingModelShift          5
#define CeilingSpeedShift          3

// define masks and shifts for the lift type fields

#define LiftTarget            0x0300
#define LiftDelay             0x00c0
#define LiftMonster           0x0020
#define LiftSpeed             0x0018

#define LiftTargetShift            8
#define LiftDelayShift             6
#define LiftMonsterShift           5
#define LiftSpeedShift             3

// define masks and shifts for the stairs type fields

#define StairIgnore           0x0200
#define StairDirection        0x0100
#define StairStep             0x00c0
#define StairMonster          0x0020
#define StairSpeed            0x0018

#define StairIgnoreShift           9
#define StairDirectionShift        8
#define StairStepShift             6
#define StairMonsterShift          5
#define StairSpeedShift            3

// define masks and shifts for the crusher type fields

#define CrusherSilent         0x0040
#define CrusherMonster        0x0020
#define CrusherSpeed          0x0018

#define CrusherSilentShift         6
#define CrusherMonsterShift        5
#define CrusherSpeedShift          3

// define masks and shifts for the door type fields

#define DoorDelay             0x0300
#define DoorMonster           0x0080
#define DoorKind              0x0060
#define DoorSpeed             0x0018

#define DoorDelayShift             8
#define DoorMonsterShift           7
#define DoorKindShift              5
#define DoorSpeedShift             3

// define masks and shifts for the locked door type fields

#define LockedNKeys           0x0200
#define LockedKey             0x01c0
#define LockedKind            0x0020
#define LockedSpeed           0x0018

#define LockedNKeysShift           9
#define LockedKeyShift             6
#define LockedKindShift            5
#define LockedSpeedShift           3

result_e T_MovePlane(sector_t *sector, fixed_t speed, fixed_t dest, boolean crush,
	int floorOrCeiling, int direction);
int EV_DoFloor(line_t *line, floor_e floortype);
int EV_DoElevator(line_t *line, elevator_e elevtype, boolean customspeed);
void EV_CrumbleChain(sector_t *sec, ffloor_t *rover);
int EV_BounceSector(sector_t *sector, fixed_t momz, sector_t *blocksector,
	boolean player);

// Some other special 3dfloor types
int EV_StartCrumble(sector_t *sector, ffloor_t *rover,
	boolean floating, player_t *player, fixed_t origalpha, boolean crumblereturn);

int EV_DoContinuousFall(sector_t *sec, sector_t *backsector, fixed_t speed, boolean backwards);

int EV_MarioBlock(sector_t *sector, sector_t *roversector, fixed_t topheight,
	struct line_s *masterline, mobj_t *puncher);

void T_MoveFloor(floormove_t *movefloor);

void T_MoveElevator(elevator_t *elevator);
void T_ContinuousFalling(elevator_t *elevator);
void T_BounceCheese(elevator_t *elevator);
void T_StartCrumble(elevator_t *elevator);
void T_MarioBlock(elevator_t *elevator);
void T_SpikeSector(elevator_t *elevator);
void T_FloatSector(elevator_t *elevator);
void T_MarioBlockChecker(elevator_t *elevator);
void T_ThwompSector(elevator_t *elevator);
void T_NoEnemiesSector(elevator_t *elevator);
void T_CameraScanner(elevator_t *elevator);
void T_RaiseSector(elevator_t *elevator);

typedef struct
{
	thinker_t thinker;	// Thinker for linedef executor delay
	line_t *line;		// Pointer to line that is waiting.
	mobj_t *caller;		// Pointer to calling mobj
	int timer;			// Delay timer
} executor_t;

void T_ExecutorDelay(executor_t *e);

// bits and shifts for generalized sector types
#define DAMAGE_MASK    0x60
#define DAMAGE_SHIFT   5
#define FRICTION_MASK  0x100
#define FRICTION_SHIFT 8
#define PUSH_MASK      0x200
#define PUSH_SHIFT     9

/** Generalized scroller.
  */
typedef struct
{
	thinker_t thinker;   ///< Thinker structure for scrolling.
	fixed_t dx, dy;      ///< (dx,dy) scroll speeds.
	int affectee;        ///< Number of affected sidedef or sector.
	int control;         ///< Control sector (-1 if none) used to control scrolling.
	fixed_t last_height; ///< Last known height of control sector.
	fixed_t vdx, vdy;    ///< Accumulated velocity if accelerative.
	int accel;           ///< Whether it's accelerative.
	/** Types of generalized scrollers.
	*/
	enum
	{
		sc_side,          ///< Scroll wall texture on a sidedef.
		sc_floor,         ///< Scroll floor.
		sc_ceiling,       ///< Scroll ceiling.
		sc_carry,         ///< Carry objects on floor.
		sc_carry_ceiling, ///< Carry objects on ceiling (for 3Dfloor conveyors).
	} type;
} scroll_t;

void T_Scroll(scroll_t *s);
void T_LaserFlash(laserthink_t *elevator);

/** Friction for ice/sludge effects.
  */
typedef struct
{
	thinker_t thinker;     ///< Thinker structure for friction.
	int friction;          ///< Friction value, 0xe800 = normal.
	int movefactor;        ///< Inertia factor when adding to momentum.
	int affectee;          ///< Number of affected sector.
	boolean roverfriction; ///< flag for whether friction originated from a FOF or not
} friction_t;

// Friction defines.
#define MORE_FRICTION_MOMENTUM 15000  ///< Mud factor based on momentum.
#define ORIG_FRICTION          0xE800 ///< Original value.
#define ORIG_FRICTION_FACTOR   2048   ///< Original value.

void T_Friction(friction_t *f);

typedef enum
{
	p_push,        ///< Point pusher or puller.
	p_wind,        ///< Wind.
	p_current,     ///< Current.
	p_upcurrent,   ///< Upwards current.
	p_downcurrent, ///< Downwards current.
	p_upwind,      ///< Upwards wind.
	p_downwind     ///< Downwards wind.
} pushertype_e;

// Model for pushers for push/pull effects
typedef struct
{
	thinker_t thinker;   ///< Thinker structure for push/pull effect.
	/** Types of push/pull effects.
	*/
	pushertype_e type;   ///< Type of push/pull effect.
	mobj_t *source;      ///< Point source if point pusher/puller.
	int x_mag;           ///< X strength.
	int y_mag;           ///< Y strength.
	int magnitude;       ///< Vector strength for point pusher/puller.
	int radius;          ///< Effective radius for point pusher/puller.
	int x, y, z;         ///< Point source if point pusher/puller.
	int affectee;     ///< Number of affected sector.
	boolean roverpusher; ///< flag for whether pusher originated from a FOF or not
	int referrer;     /// If roverpusher == true, then this will contain the sector # of the control sector where the effect was applied.
} pusher_t;

// Prototype functions for pushers
boolean PIT_PushThing(mobj_t *thing);
void T_Pusher(pusher_t *p);
mobj_t *P_GetPushThing(size_t s);

mobj_t *P_GetObjectTypeInSectorNum(mobjtype_t type, size_t s);

void P_CalcHeight(player_t *player);

sector_t *P_ThingOnSpecial3DFloor(mobj_t *mo);

#endif
