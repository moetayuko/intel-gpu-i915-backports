/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_TIMER_H
#define _BACKPORT_DSM_TIMER_H

#include_next <linux/timer.h>

#define timer_container_of(var, callback_timer, timer_fieldname) \
	container_of(callback_timer, typeof(*var), timer_fieldname)
#define timer_delete_sync del_timer_sync

#endif
