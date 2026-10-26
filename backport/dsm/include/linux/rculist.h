/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_RCULIST_H
#define _BACKPORT_DSM_RCULIST_H

#include_next <linux/rculist.h>

#define list_for_each_rcu(pos, head) \
	for (pos = rcu_dereference_raw(list_next_rcu(head)); \
	     pos != (head); pos = rcu_dereference_raw(list_next_rcu(pos)))

#endif
