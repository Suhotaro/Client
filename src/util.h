#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdlib.h>

/* Error handling macroses
 *
 * TODO: add simple examples
 */
#define RETURNIFFALSE(cond, f, arg...) \
	if (!cond) { \
		printf("[%s][%s][%d]: ERROR: " f"\n", __FILE__, __func__, __LINE__, ##arg); \
		return; \
	}

#define RETURNVALIFFALSE(cond, val, f, arg...) \
	if (!cond) { \
		printf("[%s][%s][%d]: ERROR: " f"\n",__FILE__,  __func__, __LINE__, ##arg); \
		return val; \
	}

#define RETURNIFTRUE(cond, f, arg...) \
	if (cond) { \
		printf("[%s][%s][%d]: ERROR: " f"\n",__FILE__,  __func__, __LINE__, ##arg); \
		return; \
	}

#define RETURNVALIFTRUE(cond, val, f, arg...) \
	if (cond) { \
		printf("[%s][%s][%d]: ERROR: " f"\n",__FILE__,  __func__, __LINE__, ##arg); \
		return val; \
	}

#define EXITIFTRUE(cond, f, arg...) \
	if (cond) { \
		printf("[%s][%s][%d]: ERROR: " f"\n",__FILE__,  __func__, __LINE__, ##arg); \
		exit(1); \
	}

#endif //__UTIL_H__
