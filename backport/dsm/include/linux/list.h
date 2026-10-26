/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_LIST_H
#define _BACKPORT_DSM_LIST_H

#include_next <linux/list.h>

#ifndef list_last_entry_or_null
#define list_last_entry_or_null(ptr, type, member) \
	(!list_empty(ptr) ? list_last_entry(ptr, type, member) : NULL)
#endif

#endif
