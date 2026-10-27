/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_COMPILER_TYPES_H
#define _BACKPORT_DSM_COMPILER_TYPES_H

#include_next <linux/compiler_types.h>

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#endif
