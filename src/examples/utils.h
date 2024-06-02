#pragma once

/***************************************************************************
 *   Copyright (C) 2024 by Kyle Hayes                                      *
 *   Author Kyle Hayes  kyle.hayes@gmail.com                               *
 *                                                                         *
 * This software is available under either the Mozilla Public License      *
 * version 2.0 or the GNU LGPL version 2 (or later) license, whichever     *
 * you choose.                                                             *
 *                                                                         *
 * MPL 2.0:                                                                *
 *                                                                         *
 *   This Source Code Form is subject to the terms of the Mozilla Public   *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this   *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.              *
 *                                                                         *
 *                                                                         *
 * LGPL 2:                                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifdef _WIN32
    #include <windows.h>
    #include <stdint.h>
    #include <string.h>
    #include <stdlib.h>
    #include <time.h>
    #include <stdio.h>
    #include <stdlib.h>

    #define strcasecmp _stricmp
    #define strdup _strdup
	#define snprintf_platform sprintf_s
	#define sscanf_platform sscanf_s

    #ifndef WINDOWS
        #define WINDOWS 1
    #endif

/**
 * @brief sleep for the specified number of milliseconds.
 * 
 * @param ms - number of milliseconds to sleep.
 * @return int - alwasy returns true.
 */
static int util_sleep_ms(int ms)
{
    Sleep(ms);
    return 1;
}


/**
 * @brief get the current system time in milliseconds.
 * 
 * @return int64_t - current system time in milliseconds.
 */
int64_t util_time_ms(void)
{
    FILETIME ft;
    int64_t res;

    GetSystemTimeAsFileTime(&ft);

    /* calculate time as 100ns increments since Jan 1, 1601. */
    res = (int64_t)(ft.dwLowDateTime) + ((int64_t)(ft.dwHighDateTime) << 32);

    /* get time in ms */
    res = res / 10000;

    return  res;
}



#else

/* POSIX*/

    #include <unistd.h>
    #include <strings.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <time.h>
    #include <stdarg.h>
    #include <string.h>
    #include <sys/time.h>

	#define snprintf_platform snprintf
	#define sscanf_platform sscanf

    #ifndef POSIX
        #define POSIX 1
    #endif



/**
 * @brief sleep for the specified number of milliseconds.
 * 
 * @param ms - number of milliseconds to sleep.
 * @return int - alwasy returns true.
 */
static int util_sleep_ms(int ms)
{
    struct timeval tv;

    tv.tv_sec = ms/1000;
    tv.tv_usec = (ms % 1000)*1000;

    return select(0,NULL,NULL,NULL, &tv);
}


/**
 * @brief get the current system time in milliseconds.
 * 
 * @return int64_t - current system time in milliseconds.
 */
static int64_t util_time_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv,NULL);

    return  ((int64_t)tv.tv_sec*1000)+ ((int64_t)tv.tv_usec/1000);
}


#endif

