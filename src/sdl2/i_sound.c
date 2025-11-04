#include "../doomdef.h"
#include "../doomstat.h"
#include "../w_wad.h"
#include "../z_zone.h"
#include "../i_sound.h"

#include <stdio.h>

#include <SDL.h>
#include <SDL_mixer.h>

byte sound_started = 0;

void *I_GetSfx(sfxinfo_t *sfx)
{
	sfx = NULL;
	return NULL;
}

void I_FreeSfx(sfxinfo_t *sfx)
{
	sfx = NULL;
}

void I_StartupSound(void){}

void I_ShutdownSound(void){}

//
//  SFX I/O
//

int I_StartSound(int id, int vol, int sep, int pitch, int priority)
{
	id = vol =sep = pitch = priority = 0;
	return -1;
}

void I_StopSound(int handle)
{
	handle = 0;
}

int I_SoundIsPlaying(int handle)
{
	handle = 0;
	return -1;
}

void I_UpdateSoundParams(int handle, int vol, int sep, int pitch)
{
	handle = vol = sep = pitch = 0;
}

void I_SetSfxVolume(int volume)
{
	volume = 0;
}

//
//  MUSIC I/O
//
byte music_started = 0;

void I_InitMusic(void){}

void I_ShutdownMusic(void){}

void I_PauseSong(int handle)
{
	handle = 0;
}

void I_ResumeSong(int handle)
{
	handle = 0;
}

//
//  MIDI I/O
//

byte midimusic_started = 0;

void I_InitMIDIMusic(void){}

void I_ShutdownMIDIMusic(void){}

void I_SetMIDIMusicVolume(int volume)
{
	volume = 0;
}

int I_RegisterSong(void *data, int len)
{
	data = NULL;
	len = 0;
	return -1;
}

boolean I_PlaySong(int handle, int looping)
{
	handle = 0;
	looping = 0;
	return false;
}

void I_StopSong(int handle)
{
	handle = 0;
}

void I_UnRegisterSong(int handle)
{
	handle = 0;
}

//
//  DIGMUSIC I/O
//


#ifdef HAVE_MIXER

void I_InitDigMusic(void){
	if (!nomusic) {
		SDL_Init(SDL_INIT_AUDIO);
		if (Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG) {
			CONS_Printf("OGG support enabled\n");
		} else {
			I_Error("I_InitDigMusic(): OGG support NOT enabled: %s\n", Mix_GetError());
		}
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	}
}

void I_ShutdownDigMusic(void){}


Mix_Music* currentsong;
void* data = NULL;
SDL_RWops* rw;

boolean I_StartDigSong(const char *musicname, int looping)
{
	char* songname;

	songname = strupr(va("o_%s", musicname));

	// Song does not exist
	if (W_CheckNumForName(songname) == -1) 
		return false;

	data = W_CacheLumpNum(W_GetNumForName(songname), PU_MUSIC);

	rw = SDL_RWFromConstMem(data, W_LumpLength(W_GetNumForName(songname)));

	if (rw == NULL) 
		I_Error("I_StartDigSong(): rw is NULL!\n Error: %s", Mix_GetError());

	currentsong = Mix_LoadMUSType_RW(rw, MUS_OGG, 1);

	if (currentsong == NULL) 
		I_Error("I_StartDigSong(): currentsong is NULL!\n Error: %s", Mix_GetError());

	if (looping)
		Mix_PlayMusic(currentsong, -1);
	else
		Mix_PlayMusic(currentsong, 1);

	return true;
}

void I_StopDigSong(void){}

void I_SetDigMusicVolume(int volume)
{
	volume = 0;
}
#else

void I_InitDigMusic(void){}

void I_ShutdownDigMusic(void){}

boolean I_StartDigSong(const char *musicname, int looping)
{
	musicname = NULL;
	looping = 0;
	return false;
}

void I_StopDigSong(void){}

void I_SetDigMusicVolume(int volume)
{
	volume = 0;
}
#endif


