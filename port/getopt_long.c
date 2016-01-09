

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

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif

#include "libport.h"

static int optpos, optreset;

static void getopt_msg(const char *a, const char *b, const char *c, size_t l)
{ 
	FILE *f = stderr;
	flockfile(f);
	fwrite(a, strlen(a), 1, f)
	&& fwrite(b, strlen(b), 1, f)
	&& fwrite(c, l, 1, f)
	&& putc('\n', f);
	funlockfile(f);
}

static int musl_getopt(int argc, char * const argv[], const char *optstring)
{
	int i;
	wchar_t c, d;
	int k, l;
	char *optchar;

	if (!optind || optreset) {
		optreset = 0;
		optpos = 0;
		optind = 1;
	}

	if (optind >= argc || !argv[optind])
		return -1;

	if (argv[optind][0] != '-') {
		if (optstring[0] == '-') {
			optarg = argv[optind++];
			return 1;
		}
		return -1;
	}

	if (!argv[optind][1])
		return -1;

	if (argv[optind][1] == '-' && !argv[optind][2])
		return optind++, -1;

	if (!optpos) optpos++;
	if ((k = mbtowc(&c, argv[optind]+optpos, MB_LEN_MAX)) < 0) {
		k = 1;
		c = 0xfffd; /* replacement char */
	}
	optchar = argv[optind]+optpos;
	optopt = c;
	optpos += k;

	if (!argv[optind][optpos]) {
		optind++;
		optpos = 0;
	}

	if (optstring[0] == '-' || optstring[0] == '+')
		optstring++;

	i = 0;
	d = 0;
	do {
		l = mbtowc(&d, optstring+i, MB_LEN_MAX);
		if (l>0) i+=l; else i++;
	} while (l && d != c);

	if (d != c) {
		if (optstring[0] != ':' && opterr)
			getopt_msg(argv[0], ": unrecognized option: ", optchar, k);
		return '?';
	}
	if (optstring[i] == ':') {
		if (optstring[i+1] == ':') optarg = 0;
		else if (optind >= argc) {
			if (optstring[0] == ':') return ':';
			if (opterr) getopt_msg(argv[0],
				": option requires an argument: ",
				optchar, k);
			return '?';
		}
		if (optstring[i+1] != ':' || optpos) {
			optarg = argv[optind++] + optpos;
			optpos = 0;
		}
	}
	return c;
}

static void permute(char *const *argv, int dest, int src)
{
	char **av = (char **)argv;
	char *tmp = av[src];
	int i;
	for (i=src; i>dest; i--)
		av[i] = av[i-1];
	av[dest] = tmp;
}
static int getopt_long_core(int argc, char *const *argv, const char *optstring, const struct option *longopts, int *idx, int longonly);

static int musl_getopt_long(int argc, char *const *argv, const char *optstring, const struct option *longopts, int *idx, int longonly)
{
	int ret, skipped, resumed;
	const char *optstring2 = optstring + 1;
	if (!optind || optreset) {
		optreset = 0;
		optpos = 0;
		optind = 1;
	}
	if (optind >= argc || !argv[optind]) return -1;
	skipped = optind;
	if (optstring[0] != '+' && optstring[0] != '-') {
		int i;
		for (i=optind; ; i++) {
			if (i >= argc || !argv[i]) return -1;
			if (argv[i][0] == '-' && argv[i][1]) break;
		}
		optind = i;
		optstring2 = optstring;
	}
	resumed = optind;
	ret = getopt_long_core(argc, argv, optstring2, longopts, idx, longonly);
	if (resumed > skipped) {
		int i, cnt = optind-resumed;
		for (i=0; i<cnt; i++)
			permute(argv, skipped, optind-1);
		optind = skipped + cnt;
	}
	return ret;
}

static int getopt_long_core(int argc, char *const *argv, const char *optstring, const struct option *longopts, int *idx, int longonly)
{

	if (longopts && argv[optind][0] == '-' &&
		((longonly && argv[optind][1]) ||
		 (argv[optind][1] == '-' && argv[optind][2])))
	{
		int i, cnt, match;
		char *opt;
		for (cnt=i=0; longopts[i].name; i++) {
			const char *name = longopts[i].name;
			opt = argv[optind]+1;
			if (*opt == '-') opt++;
			for (; *name && *name == *opt; name++, opt++);
			if (*opt && *opt != '=') continue;
			match = i;
			if (!*name) {
				cnt = 1;
				break;
			}
			cnt++;
		}
		if (cnt==1) {
			i = match;
			optind++;
			optopt = longopts[i].val;
			if (*opt == '=') {
				if (!longopts[i].has_arg) {
					if (optstring[0] == ':' || !opterr)
						return '?';
					getopt_msg(argv[0],
						": option does not take an argument: ",
						longopts[i].name,
						strlen(longopts[i].name));
					return '?';
				}
				optarg = opt+1;
			} else {
				if (longopts[i].has_arg == required_argument) {
					if (!(optarg = argv[optind])) {
						if (optstring[0] == ':' || !opterr)
							return ':';
						getopt_msg(argv[0],
							": option requires an argument: ",
							longopts[i].name,
							strlen(longopts[i].name));
						return '?';
					}
					optind++;
				} else optarg = NULL;
			}
			if (idx) *idx = i;
			if (longopts[i].flag) {
				*longopts[i].flag = longopts[i].val;
				return 0;
			}
			return longopts[i].val;
		}
		if (argv[optind][1] == '-') {
			if (optstring[0] != ':' && opterr)
				getopt_msg(argv[0], cnt ?
					": option is ambiguous: " :
					": unrecognized option: ",
					argv[optind]+2,
					strlen(argv[optind]+2));
			optind++;
			return '?';
		}
	}
	return musl_getopt(argc, argv, optstring);
}

int getopt_long(int argc, char *const *argv, const char *optstring, const struct option *longopts, int *idx)
{
	return musl_getopt_long(argc, argv, optstring, longopts, idx, 0);
}

int getopt_long_only(int argc, char *const *argv, const char *optstring, const struct option *longopts, int *idx)
{
	return musl_getopt_long(argc, argv, optstring, longopts, idx, 1);
}