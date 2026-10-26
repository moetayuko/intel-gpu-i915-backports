/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_EVENTFD_H
#define _BACKPORT_DSM_EVENTFD_H

#include_next <linux/eventfd.h>

#define __bp_eventfd_signal_1(ctx) eventfd_signal(ctx, 1)
#define __bp_eventfd_signal_2(ctx, n) eventfd_signal(ctx, n)
#define __bp_eventfd_pick(_1, _2, func, ...) func
#define eventfd_signal(...) \
	__bp_eventfd_pick(__VA_ARGS__, __bp_eventfd_signal_2, __bp_eventfd_signal_1)(__VA_ARGS__)

#endif
