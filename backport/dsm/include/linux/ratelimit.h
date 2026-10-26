/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_RATELIMIT_H
#define _BACKPORT_DSM_RATELIMIT_H

#include_next <linux/ratelimit.h>

static inline int ratelimit_state_get_miss(struct ratelimit_state *rs)
{
 return READ_ONCE(rs->missed);
}

#endif
