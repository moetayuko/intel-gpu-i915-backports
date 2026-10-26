/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_MUTEX_H
#define _BACKPORT_DSM_MUTEX_H

#include_next <linux/mutex.h>
#include <linux/cleanup.h>

DEFINE_GUARD(mutex, struct mutex *, mutex_lock(_T), mutex_unlock(_T))

#endif
