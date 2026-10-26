/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_SLAB_H
#define _BACKPORT_DSM_SLAB_H

#include_next <linux/slab.h>

static inline void *kmemdup_array(const void *src, size_t n, size_t size, gfp_t gfp)
{
	size_t bytes;

	if (check_mul_overflow(n, size, &bytes))
		return NULL;
	return kmemdup(src, bytes, gfp);
}

#endif
