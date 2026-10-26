/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_KTHREAD_H
#define _BACKPORT_DSM_KTHREAD_H

#include_next <linux/kthread.h>

/* In 5.10, create_worker already wakes the new worker. */
#define kthread_run_worker kthread_create_worker

#endif
