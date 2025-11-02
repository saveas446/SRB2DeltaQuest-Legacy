#include <SDL.h>
#include <SDL_keycode.h>

#include "../doomdef.h"
#include "../keys.h"
#include "../i_system.h"

byte graphics_started = 0;

byte keyboard_started = 0;

ULONG I_GetFreeMem(ULONG *total)
{
	total = NULL;
	return 0;
}

#ifdef _WIN32
static long    hacktics = 0;       //faB: used locally for keyboard repeat keys
static DWORD starttickcount = 0; // hack for win2k time bug
ULONG I_GetTime(void)
{
	int newtics = 0;

	if (!starttickcount) // high precision timer
	{
		LARGE_INTEGER currtime; // use only LowPart if high resolution counter is not available
		static LARGE_INTEGER basetime = { {0, 0} };

		// use this if High Resolution timer is found
		static LARGE_INTEGER frequency;

		if (!basetime.LowPart)
		{
			if (!QueryPerformanceFrequency(&frequency))
				frequency.QuadPart = 0;
			else
				QueryPerformanceCounter(&basetime);
		}

		if (frequency.LowPart && QueryPerformanceCounter(&currtime))
		{
			newtics = (INT32)((currtime.QuadPart - basetime.QuadPart) * TICRATE
				/ frequency.QuadPart);
		}
	}
	else
		newtics = (GetTickCount() - starttickcount) / (1000 / TICRATE);

	return newtics;
}
#else
ULONG I_GetTime (void)
{
	ULONG ticks = SDL_GetTicks();

	ticks = (ticks*TICRATE);

	ticks = (ticks/1000);

	return ticks;
}
#endif

void I_Sleep(void){}

void I_GetEvent(void){
	event_t e_w;
	SDL_Event e_s;
	// This exists so we can reuse most of the keydown event code for the keyup event
	boolean up = false;
	boolean ignoremodifiers = false;
		memset(&e_w,0x00,sizeof(e_w));
		while (SDL_PollEvent(&e_s)) {
			switch (e_s.type) {
				case SDL_QUIT:
					I_Quit();
					break;
				case SDL_KEYUP:
					e_w.type = ev_keyup;
					up = true;
				case SDL_KEYDOWN:
					if (!up)
						e_w.type = ev_keydown;

					switch (e_s.key.keysym.sym) {
						case SDLK_UP:
							e_w.data1 = KEY_UPARROW;
							break;
						case SDLK_DOWN:
							e_w.data1 = KEY_DOWNARROW;
							break;
						case SDLK_RIGHT:
							e_w.data1 = KEY_RIGHTARROW;
							break;
						case SDLK_LEFT:
							e_w.data1 = KEY_LEFTARROW;
							break;
						default:
							e_w.data1 = e_s.key.keysym.sym;
							break;
					}

					if (!ignoremodifiers) {
						// Intercept modifier keys
						if (SDL_GetModState() & KMOD_SHIFT) {
							e_w.data1 = KEY_SHIFT;
							D_PostEvent(&e_w);
							return;
						}
						if (SDL_GetModState() & KMOD_CTRL) {
							e_w.data1 = KEY_CTRL;
							D_PostEvent(&e_w);
							return;
						}
						if (SDL_GetModState() & KMOD_ALT) {
							e_w.data1 = KEY_ALT;
							D_PostEvent(&e_w);
							return;
						}
					}
						
					// For when I inevitably come back to this
					//CONS_Printf("Virtual key code: 0x%02X (%c)\n", e_s.key.keysym.sym, e_s.key.keysym.sym);
					//CONS_Printf("Physical key code: 0x%02X (%c)\n", e_s.key.keysym.scancode, e_s.key.keysym.scancode);
					D_PostEvent(&e_w);
					break;
				case SDL_MOUSEMOTION:
					e_w.type = ev_mouse;
					e_w.data1 = 0;
					SDL_GetMouseState(&e_w.data2, &e_w.data3);

					// Adjust mouse X
					e_w.data2 -= 250;
					e_w.data2 >>= 2;

					D_PostEvent(&e_w);
					break;
				default:
					break;
			}
		}
}

void I_OsPolling(void){
	I_GetEvent();
}

static ticcmd_t emptycmd;
ticcmd_t *I_BaseTiccmd(void)
{
	return &emptycmd;
}

static ticcmd_t emptycmd2;
ticcmd_t *I_BaseTiccmd2(void)
{
	return &emptycmd2;
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