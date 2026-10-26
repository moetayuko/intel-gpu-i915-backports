/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_UTIL_MACROS_H
#define _BACKPORT_DSM_UTIL_MACROS_H

#include_next <linux/util_macros.h>

#ifndef PTR_IF
#define PTR_IF(cond, ptr) ((cond) ? (ptr) : NULL)
#endif

#endif
