/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_MATH_H
#define _BACKPORT_DSM_MATH_H

/* These helpers lived in kernel.h before the math.h split. */
#include <linux/kernel.h>

#define abs_diff(a, b) ({			\
	typeof(a) __a = (a);			\
	typeof(b) __b = (b);			\
	(void)(&__a == &__b);			\
	__a > __b ? (__a - __b) : (__b - __a);	\
})

#endif
