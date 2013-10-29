#ifndef __UTIL_H
#define __UTIL_H

#include <stdio.h>

#ifdef DEBUG
#define debug_print(FMT, ...)  \
	if (DEBUG) \
		 do { fprintf(stderr, "%s() in %s, line %i: " FMT "\n", \
			__func__, __FILE__, __LINE__, ##__VA_ARGS__);   \
		 } while (0);
		
#else
#define debug_print(FMT, ...) 
#endif

#endif
