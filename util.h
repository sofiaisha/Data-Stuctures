#ifndef __UTIL_H
#define __UTIL_H

#include <stdio.h>

#ifdef DEBUG1
#define debug_print(FMT, ...)  \
	if (DEBUG1) \
		 do { fprintf(stderr, "%s() in %s, line %i: " FMT "\n", \
			__func__, __FILE__, __LINE__, ##__VA_ARGS__);   \
		 } while (0);
#else
#define debug_print(FMT, ...) 
#endif

#ifdef DEBUG2
#define debug_print_simple(FMT, ...)  \
	if (DEBUG2) \
		 do { fprintf(stderr, FMT " ", ##__VA_ARGS__); } while (0);
#else
#define debug_print_simple(FMT, ...) 
#endif

#endif
