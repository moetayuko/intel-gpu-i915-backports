/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_SEQLOCK_H
#define _BACKPORT_DSM_SEQLOCK_H

#include_next <linux/seqlock.h>

#define seqprop_sequence(s) __seqcount_sequence(s)

#endif
