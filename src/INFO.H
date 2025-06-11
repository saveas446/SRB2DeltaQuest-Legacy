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
/// \brief Thing frame/state LUT

#ifndef __INFO__
#define __INFO__

// Needed for action function pointer handling.
#include "d_think.h"
#include "sounds.h"
#include "d2udef.h"
#include "p_mobj.h"

// IMPORTANT NOTE: If you add/remove from this list of action
// functions, don't forget to update them in dehacked.c!
#ifndef _MSC_VER
void A_Explode(mobj_t* actor);
void A_Pain(mobj_t* actor);
void A_Fall(mobj_t* actor);
void A_MonitorPop(mobj_t* actor);
void A_Look(mobj_t* actor);
void A_Chase(mobj_t* actor);
void A_FaceTarget(mobj_t* actor);
void A_Scream(mobj_t* actor);
void A_BossDeath(mobj_t* actor);
void A_CustomPower(mobj_t* actor); // Use this for a custom power
void A_JumpShield(mobj_t* actor); // Obtained Jump Shield
void A_RingShield(mobj_t* actor); // Obtained Ring Shield
void A_RingBox(mobj_t* actor); // Obtained Ring Box Tails
void A_Invincibility(mobj_t* actor); // Obtained Invincibility Box
void A_SuperSneakers(mobj_t* actor); // Obtained Super Sneakers Box
void A_BunnyHop(mobj_t* actor); // have bunny hop tails
void A_BubbleSpawn(mobj_t* actor); // Randomly spawn bubbles
void A_BubbleRise(mobj_t* actor); // Bubbles float to surface
void A_BubbleCheck(mobj_t* actor); // Don't draw if not underwater
void A_ExtraLife(mobj_t* actor); // Extra Life
void A_BombShield(mobj_t* actor); // Obtained Bomb Shield
void A_WaterShield(mobj_t* actor); // Obtained Water Shield
void A_FireShield(mobj_t* actor);
void A_ScoreRise(mobj_t* actor); // Rise the score logo
void A_AttractChase(mobj_t* actor); // Ring Chase
void A_DropMine(mobj_t* actor); // Drop Mine from Skim or Jetty-Syn Bomber
void A_FishJump(mobj_t* actor); // Fish Jump
void A_SignPlayer(mobj_t* actor); // Level end sign function to determine which player to display
void A_ThrownRing(mobj_t* actor); // Sparkle trail for red ring
void A_SetSolidSteam(mobj_t* actor);
void A_UnsetSolidSteam(mobj_t* actor);
void A_JetChase(mobj_t* actor);
void A_JetbThink(mobj_t* actor); // Jetty-Syn Bomber Thinker
void A_JetgThink(mobj_t* actor); // Jetty-Syn Gunner Thinker
void A_JetgShoot(mobj_t* actor); // Jetty-Syn Shoot Function
void A_ShootBullet(mobj_t* actor); // JetgShoot without reactiontime setting
void A_MouseThink(mobj_t* actor); // Mouse Thinker
void A_DetonChase(mobj_t* actor); // Deton Chaser
void A_CapeChase(mobj_t* actor); // Fake little Super Sonic cape
void A_RotateSpikeBall(mobj_t* actor); // Spike ball rotation
void A_SnowBall(mobj_t* actor); // Snowball function for Snow Buster
void A_CrawlaCommanderThink(mobj_t* actor); // Crawla Commander
void A_SmokeTrailer(mobj_t* actor);
void A_RingExplode(mobj_t* actor);
void A_MixUp(mobj_t* actor);
void A_BossScream(mobj_t* actor);
void A_Invinciblerize(mobj_t* actor);
void A_DeInvinciblerize(mobj_t* actor);
void A_Boss2PogoSFX(mobj_t* actor);
void A_EggmanBox(mobj_t* actor);
void A_TurretFire(mobj_t* actor);
void A_SuperTurretFire(mobj_t* actor);
void A_TurretStop(mobj_t* actor);
void A_SkimChase(mobj_t* actor);
void A_SkullAttack(mobj_t* actor);
void A_FireShot(mobj_t* actor);
void A_SuperFireShot(mobj_t* actor);
void A_SparkFollow(mobj_t* actor);
void A_BuzzFly(mobj_t* actor);
void A_SetReactionTime(mobj_t* actor);
void A_LinedefExecute(mobj_t* actor);
void A_PlaySeeSound(mobj_t* actor);
void A_PlayAttackSound(mobj_t* actor);
void A_PlayActiveSound(mobj_t* actor);
void A_PumaJump(mobj_t* actor); // Mario mode function
void A_1upThinker(mobj_t* actor);
void A_BossZoom(mobj_t* actor); //Usused
void A_Boss1Chase(mobj_t* actor);
void A_Boss2Chase(mobj_t* actor);
void A_Boss2Pogo(mobj_t* actor);
void A_BossJetFume(mobj_t* actor);
void A_SpawnObjectAbsolute(mobj_t* actor);
void A_SpawnObjectRelative(mobj_t* actor);
void A_ChangeAngleRelative(mobj_t* actor);
void A_ChangeAngleAbsolute(mobj_t* actor);
void A_PlaySound(mobj_t* actor);
void A_FindTarget(mobj_t* actor);
void A_FindTracer(mobj_t* actor);
void A_SetTics(mobj_t* actor);
void A_ChangeColorRelative(mobj_t* actor);
void A_ChangeColorAbsolute(mobj_t* actor);
void A_MoveRelative(mobj_t* actor);
void A_MoveAbsolute(mobj_t* actor);
void A_SetTargetsTarget(mobj_t* actor);
void A_SetObjectFlags(mobj_t* actor);
void A_SetObjectFlags2(mobj_t* actor);
void A_RandomState(mobj_t* actor);
void A_RandomStateRange(mobj_t* actor);
void A_DualAction(mobj_t* actor);
#else
void A_Explode();
void A_Pain();
void A_Fall();
void A_MonitorPop();
void A_Look();
void A_Chase();
void A_FaceTarget();
void A_Scream();
void A_BossDeath();
void A_CustomPower(); // Use this for a custom power
void A_JumpShield(); // Obtained Jump Shield
void A_RingShield(); // Obtained Ring Shield
void A_RingBox(); // Obtained Ring Box Tails
void A_Invincibility(); // Obtained Invincibility Box
void A_SuperSneakers(); // Obtained Super Sneakers Box
void A_BunnyHop(); // have bunny hop tails
void A_BubbleSpawn(); // Randomly spawn bubbles
void A_BubbleRise(); // Bubbles float to surface
void A_BubbleCheck(); // Don't draw if not underwater
void A_ExtraLife(); // Extra Life
void A_BombShield(); // Obtained Bomb Shield
void A_WaterShield(); // Obtained Water Shield
void A_FireShield();
void A_ScoreRise(); // Rise the score logo
void A_AttractChase(); // Ring Chase
void A_DropMine(); // Drop Mine from Skim or Jetty-Syn Bomber
void A_FishJump(); // Fish Jump
void A_SignPlayer(); // Level end sign function to determine which player to display
void A_ThrownRing(); // Sparkle trail for red ring
void A_SetSolidSteam();
void A_UnsetSolidSteam();
void A_JetChase();
void A_JetbThink(); // Jetty-Syn Bomber Thinker
void A_JetgThink(); // Jetty-Syn Gunner Thinker
void A_JetgShoot(); // Jetty-Syn Shoot Function
void A_ShootBullet(); // JetgShoot without reactiontime setting
void A_MouseThink(); // Mouse Thinker
void A_DetonChase(); // Deton Chaser
void A_CapeChase(); // Fake little Super Sonic cape
void A_RotateSpikeBall(); // Spike ball rotation
void A_SnowBall(); // Snowball function for Snow Buster
void A_CrawlaCommanderThink(); // Crawla Commander
void A_SmokeTrailer();
void A_RingExplode();
void A_MixUp();
void A_BossScream();
void A_Invinciblerize();
void A_DeInvinciblerize();
void A_Boss2PogoSFX();
void A_EggmanBox();
void A_TurretFire();
void A_SuperTurretFire();
void A_TurretStop();
void A_SkimChase();
void A_SkullAttack();
void A_FireShot();
void A_SuperFireShot();
void A_SparkFollow();
void A_BuzzFly();
void A_SetReactionTime();
void A_LinedefExecute();
void A_PlaySeeSound();
void A_PlayAttackSound();
void A_PlayActiveSound();
void A_PumaJump(); // Mario mode function
void A_1upThinker();
void A_BossZoom(); //Usused
void A_Boss1Chase();
void A_Boss2Chase();
void A_Boss2Pogo();
void A_BossJetFume();
void A_SpawnObjectAbsolute();
void A_SpawnObjectRelative();
void A_ChangeAngleRelative();
void A_ChangeAngleAbsolute();
void A_PlaySound();
void A_FindTarget();
void A_FindTracer();
void A_SetTics();
void A_ChangeColorRelative();
void A_ChangeColorAbsolute();
void A_MoveRelative();
void A_MoveAbsolute();
void A_SetTargetsTarget();
void A_SetObjectFlags();
void A_SetObjectFlags2();
void A_RandomState();
void A_RandomStateRange();
void A_DualAction();
#endif

#include "spritenum.h"

extern mobjinfo_t mobjinfo[NUMMOBJTYPES];

void P_PatchInfoTables(void);

void P_BackupTables(void);

void P_ResetData(int flags);

#endif
