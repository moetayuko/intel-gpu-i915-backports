/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_STRING_HELPERS_H
#define _BACKPORT_DSM_STRING_HELPERS_H

#include_next <linux/string_helpers.h>

static inline const char *str_true_false(bool v)
{
 return v ? "true" : "false";
}

#endif
