#include <SDL.h>

#include "../doomdef.h"
#include "../i_system.h"

byte graphics_started = 0;

byte keyboard_started = 0;

ULONG I_GetFreeMem(ULONG *total)
{
	total = NULL;
	return 0;
}

tic_t I_GetTime(void)
{
	return 0;
}

void I_Sleep(void){}

void I_GetEvent(void){}

void I_OsPolling(void){}

ticcmd_t *I_BaseTiccmd(void)
{
	return NULL;
}

ticcmd_t *I_BaseTiccmd2(void)
{
	return NULL;
}

void I_Quit(void)
{
	exit(0);
}

void I_Error(char *error, ...)
{
	int len;
	char* buffer;

	va_list args;
	va_start(args, error);

	len = vsnprintf(NULL, 0, error, args);
	va_end(args);

	buffer = (char*)malloc(len + 1);

	va_start(args, error);
	vsnprintf(buffer, len + 1, error, args);
	va_end(args);

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SRB2 Error", buffer, NULL);

	M_SaveConfig(NULL);
	D_QuitNetGame();
	I_ShutdownGraphics();
	I_ShutdownSound();
	I_ShutdownMusic();
	I_ShutdownSystem();
	exit(-1);
}

byte *I_AllocLow(int length)
{
	length = 0;
	return NULL;
}

void I_Tactile(FFType Type, const JoyFF_t *Effect)
{
	Type = 0;
	Effect = NULL;
}

void I_Tactile2(FFType Type, const JoyFF_t *Effect)
{
	Type = 0;
	Effect = NULL;
}

void I_JoyScale(void){}

void I_JoyScale2(void){}

void I_InitJoystick(void){}

void I_InitJoystick2(void){}

int I_NumJoys(void)
{
	return 0;
}

const char *I_GetJoyName(int joyindex)
{
	joyindex = 0;
	return NULL;
}

void I_OutputMsg(char *error, ...)
{
	int len;
	char* buffer;

	va_list args;
	va_start(args, error);

	len = vsnprintf(NULL, 0, error, args);
	va_end(args);

	buffer = (char*)malloc(len + 1);

	va_start(args, error);
	vsnprintf(buffer, len + 1, error, args);
	va_end(args);

	SDL_Log(buffer);
}
void I_StartupMouse(void){}

void I_StartupMouse2(void){}

void I_StartupKeyboard(void){}

int I_GetKey(void)
{
	return 0;
}

void I_LoadingScreen(const char* msg)
{
	SDL_Log(msg);
}


void I_StartupTimer(void){}

void I_AddExitFunc(void (*func)())
{
	func = NULL;
}

void I_RemoveExitFunc(void (*func)())
{
	func = NULL;
}

int I_StartupSystem(void)
{
	return -1;
}

void I_ShutdownSystem(void){}

void I_GetDiskFreeSpace(INT64* freespace)
{
	freespace = NULL;
}

char *I_GetUserName(void)
{
	return NULL;
}

int I_mkdir(const char *dirname, int unixright)
{
	dirname = NULL;
	unixright = 0;
	return -1;
}

UINT64 I_FileSize(const char *filename)
{
	filename = NULL;
	return (UINT64)-1;
}

const CPUInfoFlags *I_CPUInfo(void)
{
	return NULL;
}

const char *I_LocateWad(void)
{
	return NULL;
}

void I_GetJoystickEvents(void){}

void I_GetJoystick2Events(void){}

void I_GetMouseEvents(void){}

char *I_GetEnv(const char *name)
{
	name = NULL;
	return NULL;
}

int I_PutEnv(char *variable)
{
	variable = NULL;
	return -1;
}