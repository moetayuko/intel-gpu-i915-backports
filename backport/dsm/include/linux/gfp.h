/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_GFP_H
#define _BACKPORT_DSM_GFP_H

#include_next <linux/gfp.h>

#define might_alloc(gfp) might_sleep_if(gfpflags_allow_blocking(gfp))

#endif
