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
/// \brief Commonly used routines
/// 
///	Default Config File.
///	PCX Screenshots.
///	File i/o

#ifdef __GNUC__
#include <unistd.h>
#endif
// Extended map support.
#include <ctype.h>
#ifdef SDLIO
#include "SDL_rwops.h"
#else
#ifndef _WIN32_WCE
#include <fcntl.h>
#endif
#endif

#include "console.h"
#include "M_filewriter.h"

#include "zlib.h"

#define USE_PNG

#ifdef HAVE_MNG
#define MNG_NO_INCLUDE_JNG	
#define MNG_SUPPORT_WRITE
#define MNG_ACCESS_CHUNKS
#include <libmng.h>
#endif

#include "doomdef.h"
#include "g_game.h"
#include "m_misc.h"
#include "p_local.h"
#include "hu_stuff.h"
#include "v_video.h"
#include "z_zone.h"
#include "g_input.h"
#include "i_video.h"
#include "d_main.h"
#include "m_argv.h"
#include "i_system.h"

#include <windows.h>
#include <gl/GL.h>

#ifdef _WIN32_WCE
#include "sdl/SRB2CE/cehelp.h"
#endif

#ifdef _XBOX
#include "sdl/SRB2XBOX/xboxhelp.h"
#endif

#ifdef HWRENDER
#include "hardware/hw_main.h"
#endif

static CV_PossibleValue_t screenshot_cons_t[] = {{0, "Default"}, {1, "HOME"}, {2, "SRB2"}, {3, "CUSTOM"}, {0, NULL}};
consvar_t cv_screenshot_option = {"screenshot_option", "Default", CV_SAVE, screenshot_cons_t, NULL, 0, NULL, NULL, 0, 0, NULL};
consvar_t cv_screenshot_folder = {"screenshot_folder", "screenshots/", CV_SAVE, NULL, NULL, 0, NULL, NULL, 0, 0, NULL};
boolean moviemode = false; // In snapshots files or in 1 MNG file

/** Returns the map number for a map identified by the last two characters in
  * its name.
  *
  * \param first  The first character after MAP.
  * \param second The second character after MAP.
  * \return The map number, or 0 if no map corresponds to these characters.
  * \sa G_BuildMapName
  */
int M_MapNumber(char first, char second)
{
	if (isdigit(first))
	{
		if (isdigit(second))
			return ((int)first - '0') * 10 + ((int)second - '0');
		return 0;
	}

	if (!isalpha(first))
		return 0;
	if (!isalnum(second))
		return 0;

	return 100 + ((int)tolower(first) - 'a') * 36 + (isdigit(second) ? ((int)second - '0') :
		((int)tolower(second) - 'a') + 10);
}

// ==========================================================================
//                         FILE INPUT / OUTPUT
// ==========================================================================

//
// FIL_WriteFile
//
#ifndef O_BINARY
#define O_BINARY 0
#endif

/** Writes out a file.
  *
  * \param name   Name of the file to write.
  * \param source Memory location to write from.
  * \param length How many bytes to write.
  * \return True on success, false on failure.
  */
boolean FIL_WriteFile(char const *name, const void *source, size_t length)
{
#ifdef SDLIO
	SDL_RWops *handle;
#else
	int handle;
#endif
	size_t count;

#ifdef SDLIO
	handle = SDL_RWFromFile(name, "w+b");
#else
	handle = open(name, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0666);
#endif

#ifdef SDLIO
	if (!handle)
#else
	if (handle == -1)
#endif
		return false;

#ifdef SDLIO
	count = SDL_RWwrite(handle, source, 1, (int)length);
	SDL_RWclose(handle);
#else
	count = write(handle, source, (unsigned int)length);
	close(handle);
#endif

	if (count < length)
		return false;

	return true;
}

/** Reads in a file, appending a zero byte at the end.
  *
  * \param name   Filename to read.
  * \param buffer Pointer to a pointer, which will be set to the location of a
  *               newly allocated buffer holding the file's contents.
  * \return Number of bytes read, not counting the zero byte added to the end,
  *         or 0 on error.
  */
int FIL_ReadFile(char const *name, byte **buffer)
{
#ifdef SDLIO
	SDL_RWops *handle;
#else
	int handle;
#endif
	int count, length;
#if !defined (SDLIO) && !defined (_arch_dreamcast)
	struct stat fileinfo;
#endif
	byte *buf;

#ifdef SDLIO
	handle = SDL_RWFromFile(name, "rb");
	if (!handle)
#else
	handle = open(name, O_RDONLY | O_BINARY, 0666);
	if (handle == -1)
#endif
		return 0;

#if !defined (SDLIO) && !defined (_arch_dreamcast)
	if (fstat(handle, &fileinfo) == -1)
		return 0;
#endif

#ifdef SDLIO
	{
		int currpos = SDL_RWtell(handle);
		SDL_RWseek(handle,0,SEEK_END);
		length = SDL_RWtell(handle);
		SDL_RWseek(handle,currpos,SEEK_SET);
	}
#elif defined (_arch_dreamcast)
	length = fs_total(handle);
#else
	length = fileinfo.st_size;
#endif
	buf = Z_Malloc(length + 1, PU_STATIC, NULL);
#ifdef SDLIO
	count = SDL_RWread(handle, buf, 1, length);
	SDL_RWclose(handle);
#else
	count = read(handle, buf, length);
	close(handle);
#endif

	if (count < length)
	{
		Z_Free(buf);
		return 0;
	}

	// append 0 byte for script text files
	buf[length] = 0;

	*buffer = buf;
	return length;
}

/** Check if the filename exists
  *
  * \param name   Filename to check.
  * \return true if file exists, false if it doesn't.
  */
boolean FIL_FileExists(char const *name)
{
	return access(name,0)+1;
}


/** Check if the filename OK to write
  *
  * \param name   Filename to check.
  * \return true if file write-able, false if it doesn't.
  */
boolean FIL_WriteFileOK(char const *name)
{
	return access(name,2)+1;
}


/** Check if the filename OK to read
  *
  * \param name   Filename to check.
  * \return true if file read-able, false if it doesn't.
  */
boolean FIL_ReadFileOK(char const *name)
{
	return access(name,4)+1;
}

/** Check if the filename OK to read/write
  *
  * \param name   Filename to check.
  * \return true if file (read/write)-able, false if it doesn't.
  */
boolean FIL_FileOK(char const *name)
{
	return access(name,6)+1;
}


/** Checks if a pathname has a file extension and adds the extension provided
  * if not.
  *
  * \param path      Pathname to check.
  * \param extension Extension to add if no extension is there.
  */
void FIL_DefaultExtension(char *path, const char *extension)
{
	char *src;

	// search for '.' from end to begin, add .EXT only when not found
	src = path + strlen(path) - 1;

	while (*src != '/' && src != path)
	{
		if (*src == '.')
			return; // it has an extension
		src--;
	}

	strcat(path, extension);
}

void FIL_ForceExtension(char *path, const char *extension)
{
	char *src;

	// search for '.' from end to begin, add .EXT only when not found
	src = path + strlen(path) - 1;

	while (*src != '/' && src != path)
	{
		if (*src == '.')
		{
			*src = '\0';
			break; // it has an extension
		}
		src--;
	}

	strcat(path, extension);
}

/** Checks if a filename extension is found.
  * Lump names do not contain dots.
  *
  * \param in String to check.
  * \return True if an extension is found, otherwise false.
  */
boolean FIL_CheckExtension(const char *in)
{
	while (*in++)
		if (*in == '.')
			return true;

	return false;
}

// ==========================================================================
//                        CONFIGURATION FILE
// ==========================================================================

//
// DEFAULTS
//

char configfile[MAX_WADPATH];

// ==========================================================================
//                          CONFIGURATION
// ==========================================================================
static boolean gameconfig_loaded = false; // true once config.cfg loaded AND executed

/** Saves a player's config, possibly to a particular file.
  *
  * \sa Command_LoadConfig_f
  */
void Command_SaveConfig_f(void)
{
	char tmpstr[MAX_WADPATH];

	if (COM_Argc() < 2)
	{
		CONS_Printf("saveconfig <filename[.cfg]> [-silent] : save config to a file\n");
		return;
	}
	strcpy(tmpstr, COM_Argv(1));
	FIL_ForceExtension(tmpstr, ".cfg");

	M_SaveConfig(tmpstr);
	if (stricmp(COM_Argv(2), "-silent"))
		CONS_Printf("config saved as %s\n", configfile);
}

/** Loads a game config, possibly from a particular file.
  *
  * \sa Command_SaveConfig_f, Command_ChangeConfig_f
  */
void Command_LoadConfig_f(void)
{
	if (COM_Argc() != 2)
	{
		CONS_Printf("loadconfig <filename[.cfg]> : load config from a file\n");
		return;
	}

	strcpy(configfile, COM_Argv(1));
	FIL_ForceExtension(configfile, ".cfg");
	COM_BufInsertText(va("exec \"%s\"\n", configfile));
}

/** Saves the current configuration and loads another.
  *
  * \sa Command_LoadConfig_f, Command_SaveConfig_f
  */
void Command_ChangeConfig_f(void)
{
	if (COM_Argc() != 2)
	{
		CONS_Printf("changeconfig <filename[.cfg]> : save current config and load another\n");
		return;
	}

	COM_BufAddText(va("saveconfig \"%s\"\n", configfile));
	COM_BufAddText(va("loadconfig \"%s\"\n", COM_Argv(1)));
}

/// DEBUG CAMERA/SCREENSHOT COMMANDS
// These are all glorified macros

void Command_ModifyCamRotate_f(void) {
	int modify;

	if (COM_Argc() != 2)
	{
		CONS_Printf("Invalid arguments!");
		return;
	} 

	modify = atoi(COM_Argv(1)); // Modify by first command arg

	CV_SetValue(&cv_cam_rotate, cv_cam_rotate.value + modify);
}

void Command_ModifyCamDist_f(void) {
	int modify;

	if (COM_Argc() != 2)
	{
		CONS_Printf("Invalid arguments!");
		return;
	} 

	modify = atoi(COM_Argv(1)); // Modify by first command arg

	CV_SetValue(&cv_cam_dist, (cv_cam_dist.value >> FRACBITS) + modify);
}

void Command_ModifyCamHeight_f(void) {
	int modify;

	if (COM_Argc() != 2)
	{
		CONS_Printf("Invalid arguments!");
		return;
	} 

	modify = atoi(COM_Argv(1)); // Modify by first command arg

	CV_SetValue(&cv_cam_height, (cv_cam_height.value >> FRACBITS) + modify);
}

void Command_StopCamera_f(void) {
	CV_SetValue(&cv_cam_speed, 0);
}


/** Loads the default config file.
  *
  * \sa Command_LoadConfig_f
  */
void M_FirstLoadConfig(void)
{
	// configfile is initialised by d_main when searching for the wad?

	// check for a custom config file
	if (M_CheckParm("-config") && M_IsNextParm())
	{
		strcpy(configfile, M_GetNextParm());
		CONS_Printf("config file: %s\n",configfile);
	}

	// load default control
	G_Controldefault();

	// load config, make sure those commands doesnt require the screen..
	CONS_Printf("\n");
	COM_BufInsertText(va("exec \"%s\"\n", configfile));
	// no COM_BufExecute() needed; that does it right away

	// make sure I_Quit() will write back the correct config
	// (do not write back the config if it crash before)
	gameconfig_loaded = true;
}

/** Saves the game configuration.
  *
  * \sa Command_SaveConfig_f
  */
void M_SaveConfig(char *filename)
{
	FILE *f;

	// make sure not to write back the config until it's been correctly loaded
	if (!gameconfig_loaded)
		return;

	// can change the file name
	if (filename)
	{
		f = fopen(filename, "w");
		// change it only if valid
		if (f)
			strcpy(configfile, filename);
		else
		{
			CONS_Printf("Couldn't save game config file %s\n", filename);
			return;
		}
	}
	else
	{
		f = fopen(configfile, "w");
		if (!f)
		{
			CONS_Printf("Couldn't save game config file %s\n", configfile);
			return;
		}
	}

	// header message
	fprintf(f, "// SRB2 configuration file.\n");

	// FIXME: save key aliases if ever implemented..

	CV_SaveVariables(f);
	if (!dedicated) G_SaveKeySetting(f);

	fclose(f);
}


#if !defined (DC) && !defined (_WIN32_WCE) && !defined (PSP)
static const char *Newsnapshotfile(const char *pathname, const char *ext)
{
	static char freename[13] = "srb2XXXX.ext";
	int i = 5000; // start in the middle: num screenshots divided by 2
	int add = i; // how much to add or subtract if wrong; gets divided by 2 each time
	int result; // -1 = guess too high, 0 = correct, 1 = guess too low

	// find a file name to save it to
	strcpy(freename+9,ext);

	for (;;)
	{
		freename[4] = (char)('0' + (char)(i/1000));
		freename[5] = (char)('0' + (char)((i/100)%10));
		freename[6] = (char)('0' + (char)((i/10)%10));
		freename[7] = (char)('0' + (char)(i%10));

		if (FIL_WriteFileOK(va(pandf,pathname,freename))) // access succeeds
			result = 1; // too low
		else // access fails: equal or too high
		{
			if (!i)
				break; // not too high, so it must be equal! YAY!

			freename[4] = (char)('0' + (char)((i-1)/1000));
			freename[5] = (char)('0' + (char)(((i-1)/100)%10));
			freename[6] = (char)('0' + (char)(((i-1)/10)%10));
			freename[7] = (char)('0' + (char)((i-1)%10));
			if (!FIL_WriteFileOK(va(pandf,pathname,freename))) // access fails
				result = -1; // too high
			else
				break; // not too high, so equal, YAY!
		}

		add /= 2;

		if (!add) // don't get stuck at 5 due to truncation!
			add = 1;

		i += add * result;

		if (add < 0 || add > 9999)
			return NULL;
	}

	freename[4] = (char)('0' + (char)(i/1000));
	freename[5] = (char)('0' + (char)((i/100)%10));
	freename[6] = (char)('0' + (char)((i/10)%10));
	freename[7] = (char)('0' + (char)(i%10));

	return freename;
}
#endif

#ifdef HAVE_MNG // SRB2 MNG Moives

static mng_handle MNG_ptr;

typedef struct MNG_user_struct
{
		FILE *fh;                 /* file handle */
		mng_palette8ep pal;        /* 256RGB pal*/
} MNG_userdata;
typedef MNG_userdata *MNG_userdatap;

static mng_ptr MNG_DECL MNG_memalloc(mng_size_t iLen)
{return (mng_ptr)calloc (1,iLen);}

static void MNG_DECL MNG_free(mng_ptr iPtr, mng_size_t iLen)
{ iLen = 0; free (iPtr);}

static mng_bool MNG_DECL MNG_open (mng_handle hHandle)
{ hHandle = NULL; return MNG_TRUE;}

static mng_bool MNG_DECL MNG_close (mng_handle hHandle)
{
	MNG_userdatap privdata = (MNG_userdatap)mng_get_userdata(hHandle);
	if (privdata->fh)
		fclose(privdata->fh);
	privdata->fh = 0;
	return MNG_TRUE;
}

static mng_bool MNG_DECL MNG_write (mng_handle  hHandle,
                                    mng_ptr     pBuf,
                                    mng_uint32  iBuflen,
                                    mng_uint32p pWritten)
{                                      
  MNG_userdatap privdata = (MNG_userdatap)mng_get_userdata(hHandle);
  *pWritten = fwrite(pBuf, 1, iBuflen, privdata->fh);
  return MNG_TRUE;
}

static boolean M_SetupMNG(const mng_pchar filename, byte *pal)
{
	MNG_userdatap MNG_shared;
	mng_retcode ret;

	MNG_shared = malloc(sizeof (MNG_userdata));
	if (!MNG_shared)
	{
		return false;
	}

	MNG_shared->fh = fopen(filename,"wb");
	if (!MNG_shared->fh)
	{
		free(MNG_shared);
		return false;
	}
	MNG_shared->pal = (mng_palette8ep)pal;

	MNG_ptr = mng_initialize ((mng_ptr)MNG_shared, MNG_memalloc, MNG_free, MNG_NULL);
	if (!MNG_ptr)
	{
		fclose(MNG_shared->fh);
		free(MNG_shared);
		return false;
	}

	ret = mng_setcb_openstream  (MNG_ptr, MNG_open);
	ret = mng_setcb_closestream (MNG_ptr, MNG_close);
	ret = mng_setcb_writedata   (MNG_ptr, MNG_write);
	ret = mng_create(MNG_ptr);
	ret = mng_putchunk_mhdr(MNG_ptr, vid.width, vid.height, TICRATE, 0, 0, 0,
	      MNG_SIMPLICITY_VALID | MNG_SIMPLICITY_SIMPLEFEATURES);
	ret = mng_putchunk_term(MNG_ptr, MNG_TERMACTION_REPEAT,
	      MNG_ITERACTION_FIRSTFRAME, TICRATE, 0x7FFFFFFF);

	// iTXT

	if (pal)
	{
		ret = mng_putchunk_plte(MNG_ptr, 256, MNG_shared->pal);
	}

	ret = mng_putchunk_back(MNG_ptr, 0,0,0, 0, 0, MNG_BACKGROUNDIMAGE_NOTILE);
	ret = mng_putchunk_fram(MNG_ptr, MNG_FALSE, MNG_FRAMINGMODE_1, 0,MNG_NULL,
			MNG_CHANGEDELAY_DEFAULT, MNG_CHANGETIMOUT_NO, MNG_CHANGECLIPPING_NO, MNG_CHANGESYNCID_NO,
			100/TICRATE, 0,0,0,0,0,0, MNG_NULL,0);

	ret = mng_putchunk_defi(MNG_ptr, 0, MNG_DONOTSHOW_NOTVISIBLE, MNG_CONCRETE, MNG_FALSE, 0,0, MNG_FALSE, 0,0,0,0);

	return true;
}

boolean M_OpenMNG(void)
{
	const char *freename = NULL, *pathname = ".";
	boolean ret = false;

	if (cv_screenshot_option.value == 0)
		pathname = usehome ? srb2home : srb2path;
	else if (cv_screenshot_option.value == 1)
		pathname = srb2home;
	else if (cv_screenshot_option.value == 2)
		pathname = srb2path;
	else if (cv_screenshot_option.value == 3 && *cv_screenshot_folder.string != '\0')
		pathname = cv_screenshot_folder.string;

	if (rendermode == render_none)
		I_Error("Can't take a MNG movie without a render system");
	else
		freename = Newsnapshotfile(pathname,"mng");

	if (!freename)
		goto failure;

	if (rendermode == render_soft)
		ret = M_SetupMNG(va(pandf,pathname,freename), W_CacheLumpName("PLAYPAL", PU_CACHE));
	else
		ret = M_SetupMNG(va(pandf,pathname,freename), NULL);

failure:
	if (!ret)
	{
		if (freename)
			CONS_Printf("Couldn't create MNG movie file %s in %s\n", freename, pathname);
		else
			CONS_Printf("Couldn't create MNG movie file (all 10000 slots used!) in %s\n", pathname);
	}
	return ret;
}

void M_SaveMNG(void)
{
	MNG_userdatap privdata;
	mng_retcode ret;
	mng_uint8 mColortype;
	if (!MNG_ptr) return;

	privdata = (MNG_userdatap)mng_get_userdata(MNG_ptr);

	if (privdata->pal)
	{
		mColortype = MNG_COLORTYPE_INDEXED;
	}
	else
	{
		mColortype = MNG_COLORTYPE_RGB;
	}

	ret = mng_putchunk_ihdr(MNG_ptr, vid.width, vid.height,
	      MNG_BITDEPTH_8, mColortype, MNG_COMPRESSION_DEFLATE,
		   MNG_FILTER_ADAPTIVE, MNG_INTERLACE_NONE);

	if (privdata->pal)
		ret = mng_putchunk_plte (MNG_ptr, 0, privdata->pal);

	ret = mng_putchunk_idat(MNG_ptr,0,NULL); //RAW?
	ret = mng_putchunk_iend(MNG_ptr);
}

boolean M_CloseMNG(void)
{
	mng_ptr privdata;
	if (!MNG_ptr) return false;

	privdata = mng_get_userdata(MNG_ptr);
	if (privdata)
		free(privdata);

	mng_putchunk_mend(MNG_ptr);
	mng_write(MNG_ptr);
	mng_cleanup(&MNG_ptr);

	CONS_Printf("MNG movie saved\n");
	return false;
}

#endif

/*
// Generate a GIF color palette from raw RGB byte array
ColorMapObject* M_CreateGIFColorMap(byte* data, int count) {

	int i;
    ColorMapObject* cmap;
	cmap = GifMakeMapObject(count, NULL);
    if (!cmap) return NULL;

    for (i = 0; i < count; ++i) {
        cmap->Colors[i].Red   = data[i * 3 + 0];
        cmap->Colors[i].Green = data[i * 3 + 1];
        cmap->Colors[i].Blue  = data[i * 3 + 2];
    }

    return cmap;
}

GifFileType* gif;

int M_SetupGIF(const char* filename, byte* pal) {
    int error;
    
	ColorMapObject* cmap;
	SavedImage image;
	byte *linear = NULL;

	int datasize;
	gif = EGifOpenFileName(filename, false, &error);

    if (!gif) return error;
	if (!filename) return GIF_ERROR;

	cmap = M_CreateGIFColorMap(pal, 256);

    EGifPutScreenDesc(gif, vid.width, vid.height, cmap->BitsPerPixel, 0, cmap);

    // allocate savedimage
    image.ImageDesc.Left = 0;
    image.ImageDesc.Top = 0;
    image.ImageDesc.Width = vid.width;
    image.ImageDesc.Height = vid.height;
    image.ImageDesc.Interlace = false;
    image.ImageDesc.ColorMap = NULL;

    datasize = vid.width * vid.height;

    image.RasterBits = (GifByteType*)malloc(datasize);

	if (rendermode == render_soft)
	{
		// munge planar buffer to linear
		linear = screens[2];
		I_ReadScreen(linear);
	}

    memcpy(image.RasterBits, linear, datasize);

    // write the frame
    EGifPutImageDesc(gif, image.ImageDesc.Left, image.ImageDesc.Top, vid.width, vid.height, false, NULL);
    EGifPutLine(gif, image.RasterBits, datasize) ;

    free(image.RasterBits);
    return error;
}

int gif_frames;

boolean M_OpenGIF(void)
{
	const char *freename = NULL, *pathname = ".";
	boolean ret = false;

	if (cv_screenshot_option.value == 0)
		pathname = usehome ? srb2home : srb2path;
	else if (cv_screenshot_option.value == 1)
		pathname = srb2home;
	else if (cv_screenshot_option.value == 2)
		pathname = srb2path;
	else if (cv_screenshot_option.value == 3 && *cv_screenshot_folder.string != '\0')
		pathname = cv_screenshot_folder.string;

	if (rendermode == render_none)
		I_Error("Can't take a GIF movie without a render system");
	else
		freename = Newsnapshotfile(pathname,"gif");

	if (!freename)
		goto failure;

	if (rendermode == render_soft)
		ret = M_SetupGIF(va(pandf,pathname,freename), W_CacheLumpName("PLAYPAL", PU_CACHE));
	else
		ret = M_SetupGIF(va(pandf,pathname,freename), NULL);

	gif_frames = 0;

failure:
	if (!ret)
	{
		if (freename)
			CONS_Printf("Couldn't create GIF movie file %s in %s\n", freename, pathname);
		else
			CONS_Printf("Couldn't create GIF movie file (all 10000 slots used!) in %s\n", pathname);
	}
	return ret;
}


// Append a single frame to an open GIF file
int M_SaveGIF(void) {
	byte* linear = NULL;
	int y;
	GraphicsControlBlock gcb;
    GifByteType extData[4];

	if (rendermode == render_soft)
	{
		// munge planar buffer to linear
		linear = screens[2];
		I_ReadScreen(linear);
	}

	// temporary solution until we can calculate delay in a better way
   
	if (vid.width == 640)
    	gcb.DelayTime = 3;  // delay in 1/100ths of a second

	if (vid.width == 1280)	
		gcb.DelayTime = 6;  // delay in 1/100ths of a second


    gcb.DisposalMode = DISPOSAL_UNSPECIFIED;

    if (EGifGCBToExtension(&gcb, extData) == GIF_ERROR) return gif->Error;
    if (EGifPutExtension(gif, GRAPHICS_EXT_FUNC_CODE, sizeof(extData), extData) == GIF_ERROR) return gif->Error;

    if (EGifPutImageDesc(gif, 0, 0, vid.width, vid.height, false, NULL) == GIF_ERROR)
        return gif->Error;

    if (EGifPutLine(gif, (GifByteType*)linear, vid.width * vid.height) == GIF_ERROR)
        return gif->Error;

	gif_frames++;

    return GIF_OK;
}

M_CloseGIF(void) {
	int error;
	if (EGifCloseFile(gif, &error) == GIF_ERROR) return error;
    return GIF_OK;
}*/


#ifdef USE_PNG
/** Writes a PNG file to disk.
  *
  * \param filename Filename to write to.
  * \param data     The image data.
  * \param width    Width of the picture.
  * \param height   Height of the picture.
  * \param palette  Palette of image data
  *  \note if palette is NULL, BGR888 format
  */
#define PNG_COLOR_TYPE_INDEXED 3
#define PNG_COLOR_TYPE_RGB 2
#define PNG_COLOR_TYPE_RGBA 6

#define PNG_FILTER_NONE 0
#define PNG_FILTER_SUB 1
#define PNG_FILTER_UP 2
#define PNG_FILTER_AVG 3
#define PNG_FILTER_PAETH 4

static UINT8 png_choose_filter(const UINT8* row, const UINT8* prew_row, int width, int channels) {
	int x;
	UINT8 filter;

	int width_bytes = width * channels;
	int best_sum = INT_MAX;
	UINT8 best_filter = PNG_FILTER_NONE;

	for(filter = PNG_FILTER_NONE; filter <= PNG_FILTER_UP; filter++) {
		int sum = 0;

		for(x = 0; x < width_bytes; x++) {
			int left = (x >= channels) ? row[x - channels] : 0;
			int up = prew_row ? prew_row[x] : 0;
			int val = 0;

			switch(filter) {
				case PNG_FILTER_NONE: val = row[x]; break;
				case PNG_FILTER_SUB: val = row[x] - left; break;
				case PNG_FILTER_UP: val = row[x] - up; break;
			}

			sum += abs(val);
		}

		if(sum < best_sum) {
			best_sum = sum;
			best_filter = filter;
		}
	}

	return best_filter;
}

static void png_apply_sub_filter(UINT8* row, int width, int channels) {
	int x;
	int width_bytes = width * channels;

	row[0] = PNG_FILTER_SUB;
	for(x = width_bytes - 1; x >= channels; x--) {
		int i = x + 1;
		row[i] = (row[i] - row[i - channels]) & 0xff;
	}
}

static void png_apply_up_filter(UINT8* row, UINT8* prew_row, int width, int channels) {
	int x;
	int width_bytes =width * channels;

	row[0] = PNG_FILTER_UP;
	if(!prew_row) return;

	for(x = 0; x < width_bytes; x++) {
		int i = x + 1;
		row[i] = (row[i] - prew_row[x]) & 0xff;
	}
}

const UINT8 png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
boolean M_SavePNG(const char* filename, const UINT8* data, int width, int height, int channels, const UINT8* palette) {
	// get file
	FileBuffer fb = {0};

	FILE* file = fopen(filename, "wb");
	if(!file) {
		CONS_Printf("M_SavePNG: Error on opening %s for write\n", filename);
		goto fail_return;
	}

	fw_write_bytes(&fb, png_signature, sizeof png_signature);

	{ // write ihdr chunk
		size_t start;
	
		// header
		fw_be_write_u32(&fb, 13); // length
		start = fb.pos;

		fw_write_string(&fb, "IHDR"); // identifier
		
		// chunk data
		fw_be_write_u32(&fb, width);
		fw_be_write_u32(&fb, height);
		fw_write_u8(&fb, 8); // bit depth

		if(channels == 1) fw_write_u8(&fb, PNG_COLOR_TYPE_INDEXED);
		else if(channels == 3) fw_write_u8(&fb, PNG_COLOR_TYPE_RGB);
		else if(channels == 4) fw_write_u8(&fb, PNG_COLOR_TYPE_RGBA);
		else {
			CONS_Printf("M_SavePNG: Unsupported amount of color channels (%u)\n", channels);
			goto fail_return;
		}

		fw_write_u8(&fb, 0); // compression method
		fw_write_u8(&fb, 0); // filter method
		fw_write_u8(&fb, 0); // interlace method
		
		// checksum
		fw_be_write_u32(&fb, fw_get_crc32(&fb, start, 17)); // 17 = chunk length + sizeof(UINT32)
	}

	if(channels == 1) { // write plte chunk
		size_t start;

		fw_be_write_u32(&fb, 256 * 3);
		start = fb.pos;

		fw_write_string(&fb, "PLTE");
		fw_write_bytes(&fb, palette, 256 * 3);

		fw_be_write_u32(&fb, fw_get_crc32(&fb, start, 256 * 3 + 4));
	}

	{ // write idat chunk
		uLongf idat_size;
		size_t start;
		UINT8* idat_comp_data;
		int y;
		
		size_t width_bytes =  width * channels;
		size_t row_bytes = width_bytes + 1; // filter byte :3
		size_t img_size = row_bytes * height;

		UINT8* png_rows = malloc(img_size);
		UINT8* prew_row = calloc(width_bytes, 1);
		if(!png_rows || !prew_row) {
			CONS_Printf("M_SavePNG: Out of memory\n");
			goto fail_return;
		}

		for(y = 0; y < height; y++) {
			UINT8 filter;

			UINT8* row_ptr = png_rows + y * row_bytes;
			const UINT8* data_ptr = data + y * width_bytes;
			memcpy(row_ptr + 1, data_ptr, width_bytes);

			filter = png_choose_filter(row_ptr, prew_row, width, channels);
			
			if(!filter) row_ptr[0] = PNG_FILTER_NONE;
			else if(filter == 1) png_apply_sub_filter(row_ptr, width, channels);
			else if(filter == 2) png_apply_up_filter(row_ptr, prew_row, width, channels);

			memcpy(prew_row, data_ptr, width_bytes);
		}

		free(prew_row);

		idat_size = compressBound(img_size);
		idat_comp_data = malloc((size_t)idat_size);
		if(!idat_comp_data) goto fail_return;

		if(compress2(idat_comp_data, &idat_size, png_rows, img_size, Z_BEST_COMPRESSION) != Z_OK) {
			free(png_rows);
			free(idat_comp_data);

			CONS_Printf("M_SavePNG: Error at compressing IDAT chunk on %s\n", filename);
			goto fail_return;
		}

		fw_be_write_u32(&fb, (size_t)idat_size);
		start = fb.pos;

		fw_write_string(&fb, "IDAT");
		fw_write_bytes(&fb, idat_comp_data, (size_t)idat_size);

		fw_be_write_u32(&fb, fw_get_crc32(&fb, start, (size_t)idat_size + 4));

		free(png_rows);
		free(idat_comp_data);
	}

	{ // write iend chunk
		size_t start;

		fw_be_write_u32(&fb, 0);
		start = fb.pos;

		fw_write_string(&fb, "IEND");

		fw_be_write_u32(&fb, fw_get_crc32(&fb, start, 4));
	}
	
	// finish writing
	if(fb.pos > 0) {
		size_t written = fwrite(fb.buffer, 1, fb.pos, file);
		if(written != fb.pos) {
			CONS_Printf("M_SavePNG: Error on writing full buffer for %s\n", filename);
			goto fail_return;
		}
	}

	fclose(file);
	if(fb.buffer) free(fb.buffer);

	return true;

fail_return:
	fclose(file);
	if(fb.buffer) free(fb.buffer);

	return false;
}

#else
/** PCX file structure.
  */
typedef struct
{
	char manufacturer;
	char version;
	char encoding;
	char bits_per_pixel;

	unsigned short xmin, ymin;
	unsigned short xmax, ymax;
	unsigned short hres, vres;
	unsigned char palette[48];

	char reserved;
	char color_planes;
	unsigned short bytes_per_line;
	unsigned short palette_type;

	char filler[58];
	unsigned char data; ///< Unbounded; used for all picture data.
} pcx_t;

/** Writes a PCX file to disk.
  *
  * \param filename Filename to write to.
  * \param data     The image data.
  * \param width    Width of the picture.
  * \param height   Height of the picture.
  * \param palette  Palette of image data
  */
#if !defined (DC) && !defined (_WIN32_WCE) && !defined (PSP)
static boolean WritePCXfile(const char* filename, const byte* data, int width, int height, const byte* palette)
{
	int i;
	size_t length;
	pcx_t* pcx;
	byte* pack;

	pcx = Z_Malloc(width * height * 2 + 1000, PU_STATIC, NULL);

	pcx->manufacturer = 0x0a; // PCX id
	pcx->version = 5; // 256 color
	pcx->encoding = 1; // uncompressed
	pcx->bits_per_pixel = 8; // 256 color
	pcx->xmin = pcx->ymin = 0;
	pcx->xmax = SHORT(width - 1);
	pcx->ymax = SHORT(height - 1);
	pcx->hres = SHORT(width);
	pcx->vres = SHORT(height);
	memset(pcx->palette, 0, sizeof(pcx->palette));
	pcx->color_planes = 1; // chunky image
	pcx->bytes_per_line = SHORT(width);
	pcx->palette_type = SHORT(1); // not a grey scale
	memset(pcx->filler, 0, sizeof(pcx->filler));

	// pack the image
	pack = &pcx->data;

	for (i = 0; i < width * height; i++)
	{
		if ((*data & 0xc0) != 0xc0)
			*pack++ = *data++;
		else
		{
			*pack++ = 0xc1;
			*pack++ = *data++;
		}
	}

	// write the palette
	*pack++ = 0x0c; // palette ID byte
	for (i = 0; i < 768; i++)
		*pack++ = *palette++;

	// write output file
	length = pack - (byte*)pcx;
	i = FIL_WriteFile(filename, pcx, length);

	Z_Free(pcx);
	return i;
}
#endif
#endif

/** Takes a screenshot.
  * The screenshot is saved as "srb2xxxx.pcx" (or "srb2xxxx.tga" in hardware
  * rendermode) where xxxx is the lowest four-digit number for which a file
  * does not already exist.
  *
  * \sa HWR_ScreenShot
  */
void M_ScreenShot(void)
{
#if !defined (DC) && !defined (_WIN32_WCE) && !defined (PSP)
	const char *freename = NULL, *pathname = ".";
	boolean ret = false;
	byte *linear = NULL;

	if (cv_screenshot_option.value == 0)
		pathname = usehome ? srb2home : srb2path;
	else if (cv_screenshot_option.value == 1)
		pathname = srb2home;
	else if (cv_screenshot_option.value == 2)
		pathname = srb2path;
	else if (cv_screenshot_option.value == 3 && *cv_screenshot_folder.string != '\0')
		pathname = cv_screenshot_folder.string;

	if (cv_screenshot_option.value != 3)
	{
		// Check that we're not creating a screenshots folder inside a screenshots folder
		// This has the added downside that if any of the folders D2U is stored in has "screenshots" in it, that it won't add "\screenshots\" to the end of the path
		// But whatever
		if (strstr(pathname, "screenshots") == NULL) {
			strcat(pathname, PATHSEP"screenshots"PATHSEP);
		}
		I_mkdir(pathname, 0755);
	}

#ifdef USE_PNG
	if (rendermode != render_none)
		freename = Newsnapshotfile(pathname,"png");
#else
	if (rendermode == render_soft)
		freename = Newsnapshotfile(pathname,"pcx");
	else if (rendermode != render_none)
		freename = Newsnapshotfile(pathname,"tga");
#endif
	else
		I_Error("Can't take a screenshot without a render system");

	if (rendermode == render_soft)
	{
		// munge planar buffer to linear
		linear = screens[2];
		I_ReadScreen(linear);
	}

	if (!freename)
		goto failure;

		// save the pcx file
#ifdef HWRENDER
	if (rendermode != render_soft) {
		UINT8* data = HWR_Screenshot();
		ret = M_SavePNG(va(pandf,pathname,freename), data, vid.width, vid.height, 3, NULL);
	} else
#endif
	if (rendermode != render_none)
	{
#ifdef USE_PNG
		ret = M_SavePNG(va(pandf,pathname,freename), linear, vid.width, vid.height, 1,
			W_CacheLumpName("PLAYPAL", PU_CACHE));
#else
		ret = WritePCXfile(va(pandf,pathname,freename), linear, vid.width, vid.height,
			W_CacheLumpName("PLAYPAL", PU_CACHE));
#endif
	}

failure:
	//if (ret)
	//{
		//if (!moviemode)
			//CONS_Printf("screen shot %s saved in %s\n", freename, pathname); Removed because it was annoying for taking screenshots
	//}
	if (!ret)
	{
		if (freename)
			CONS_Printf("Couldn't create screen shot %s in %s\n", freename, pathname);
		else
			CONS_Printf("Couldn't create screen shot (all 10000 slots used!) in %s\n", pathname);
	}
#endif
}

// ==========================================================================
//                        MISC STRING FUNCTIONS
// ==========================================================================

/** Returns a temporary string made out of varargs.
  * For use with CONS_Printf().
  *
  * \param format Format string.
  * \return Pointer to a static buffer of 1024 characters, containing the
  *         resulting string.
  */
char *va(const char *format, ...)
{
	va_list argptr;
	static char string[1024];

	va_start(argptr, format);
	vsprintf(string, format, argptr);
	va_end(argptr);

	return string;
}

/** Creates a string in the first argument that is the second argument followed
  * by the third argument followed by the first argument.
  * Useful for making filenames with full path. s1 = s2+s3+s1
  *
  * \param s1 First string, suffix, and destination.
  * \param s2 Second string. Ends up first in the result.
  * \param s3 Third string. Ends up second in the result.
  */
void strcatbf(char *s1, const char *s2, const char *s3)
{
	char tmp[1024];

	strcpy(tmp, s1);
	strcpy(s1, s2);
	strcat(s1, s3);
	strcat(s1, tmp);
}
