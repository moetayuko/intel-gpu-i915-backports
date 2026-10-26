/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_INTERRUPT_H
#define _BACKPORT_DSM_INTERRUPT_H

#include_next <linux/interrupt.h>

#define tasklet_unlock_spin_wait tasklet_unlock_wait

#endif
