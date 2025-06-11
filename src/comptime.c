/*
 * Compile date and time.
 */

#include "comptime.h"
#include "d2udef.h"
#include "doomstat.h"
#include "doomtype.h"
#include <time.h>
#include <string.h>

//! IMPORTANT: UPDATE THIS EVERY TIME YOU INCREMENT THE MINOR VERSION YOU OAF
#define MINOR_VER2 1

const char *compdate = __DATE__;
const char *comptime = __TIME__;

// Increments every day after the last time we incremented MINORVER
USHORT TINYVER;


int month_to_num(const char *mon) {
    int i;
    static const char *months[] = {
        "Jan","Feb","Mar","Apr","May","Jun",
        "Jul","Aug","Sep","Oct","Nov","Dec"
    };
    for (i = 0; i < 12; i++) {
        if (strcmp(mon, months[i]) == 0)
            return i;
    }
    return -1;
}

int calc_TINYVER(void) {
    char month_str[4];
    int day, year;

    struct tm build_tm;
    struct tm base_tm;

    time_t build_time;
    time_t base_time;

    // Parse compdate
    if (sscanf(compdate, "%3s %d %d", month_str, &day, &year) != 3)
        return -1;

    build_tm.tm_mday = day;
    build_tm.tm_mon = month_to_num(month_str) + 1;
    build_tm.tm_year = year - 1900;
    build_tm.tm_isdst = -1;

    // If I forget to update this date, this error is my reminder
    if (MINOR_VER2 != MINORVER) {
        //"version number incorrect"
        I_Error("calc_TINYVER(): Program metadata mismatch. Contact D2U devs.");
    }
    
    // this is the date to update
    base_tm.tm_mday = 12;
    base_tm.tm_mon = 6;
    base_tm.tm_year = 2025 - 1900;
    base_tm.tm_isdst = -1;

    build_time = mktime(&build_tm);
    base_time = mktime(&base_tm);

    if (build_time == -1 || base_time == -1)
        return -1;

    return (int)((build_time - base_time) / 86400); // seconds in a day
}