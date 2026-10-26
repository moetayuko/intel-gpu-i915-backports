/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_MINMAX_H
#define _BACKPORT_DSM_MINMAX_H

#include_next <linux/minmax.h>

#define MAX_T(type, a, b) ((type)(a) > (type)(b) ? (type)(a) : (type)(b))
#define MIN_T(type, a, b) ((type)(a) < (type)(b) ? (type)(a) : (type)(b))

#define min_array(array, len) ({ \
 typeof(&(array)[0]) __array = (array); \
 typeof((array)[0]) __min = __array[0]; \
 size_t __i; \
 for (__i = 1; __i < (len); __i++) __min = min(__min, __array[__i]); \
 __min; \
})

#define max_array(array, len) ({ \
 typeof(&(array)[0]) __array = (array); \
 typeof((array)[0]) __max = __array[0]; \
 size_t __i; \
 for (__i = 1; __i < (len); __i++) __max = max(__max, __array[__i]); \
 __max; \
})

#endif
