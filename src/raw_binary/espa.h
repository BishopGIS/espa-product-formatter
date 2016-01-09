#ifndef ESPA_H
#define ESPA_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif //HAVE_CONFIG_H

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif //HAVE_UNISTD_H

#ifdef HAVE_MATH_H
#include <math.h>
#endif //HAVE_MATH_H

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif

#ifdef HAVE_TIME_H
#include <time.h>
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#ifdef HAVE_GETOPT_H
#   include <getopt.h>
#else
#   include "libport.h"
#endif //HAVE_GETOPT_H

#if !defined(MAXPATHLEN) && defined(FNMAX)
#define MAXPATHLEN FNMAX
#endif

#if !defined(MAXPATHLEN) && defined(MAXPATH)
#define MAXPATHLEN MAXPATH
#endif

#if !defined(MAXPATHLEN) && defined(_MAX_PATH)
#define MAXPATHLEN _MAX_PATH
#endif

#if !defined(MAXPATHLEN) && defined(PATH_MAX)
#define MAXPATHLEN PATH_MAX
#endif

#if !defined(MAXPATHLEN) && defined(FILENAME_MAX)
#define MAXPATHLEN FILENAME_MAX
#endif

#if !defined(MAXPATHLEN) && defined(NAME_MAX)
#define MAXPATHLEN NAME_MAX
#endif

/* Hewlett-Packard HP-UX 10.20 has FILENAME_MAX == NAME_MAX == 14, but
the file system actually supports names up to 255 characters in length,
so override ridiculously small values! */
#if !defined(MAXPATHLEN) || (MAXPATHLEN < 255)
#undef MAXPATHLEN
#define MAXPATHLEN 255
#endif

#ifdef _WIN32
#include <windows.h>
#define	ftello	_ftelli64
#define	fseeko	_fseeki64
#endif	//	#ifdef _WIN32

#endif //ESPA_H