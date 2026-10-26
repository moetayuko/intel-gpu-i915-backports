/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_WW_MUTEX_H
#define _BACKPORT_DSM_WW_MUTEX_H

#include_next <linux/ww_mutex.h>

static inline int backport_ww_mutex_trylock(struct ww_mutex *lock,
					  struct ww_acquire_ctx *ctx)
{
	/* 5.10 cannot associate a context in trylock. Use the caller's slow
	 * acquisition path so ww_mutex_lock() performs the wound/wait protocol.
	 */
	if (ctx)
		return 0;
	return ww_mutex_trylock(lock);
}

#define __bp_ww_trylock_1(lock) backport_ww_mutex_trylock(lock, NULL)
#define __bp_ww_trylock_2(lock, ctx) backport_ww_mutex_trylock(lock, ctx)
#define __bp_ww_trylock_pick(_1, _2, func, ...) func
#define ww_mutex_trylock(...) \
	__bp_ww_trylock_pick(__VA_ARGS__, __bp_ww_trylock_2, __bp_ww_trylock_1)(__VA_ARGS__)

#endif
