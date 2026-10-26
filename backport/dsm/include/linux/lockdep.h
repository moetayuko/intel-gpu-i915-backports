/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_LOCKDEP_H
#define _BACKPORT_DSM_LOCKDEP_H

#include_next <linux/lockdep.h>

#ifdef CONFIG_LOCKDEP
#define lockdep_assert(cond) do { WARN_ON(debug_locks && !(cond)); } while (0)
#define lockdep_assert_once(cond) do { WARN_ON_ONCE(debug_locks && !(cond)); } while (0)
#else
#define lockdep_assert(cond) do { } while (0)
#define lockdep_assert_once(cond) do { } while (0)
#endif

#ifdef CONFIG_LOCKDEP
#define lockdep_assert_none_held_once() lockdep_assert_once(!current->lockdep_depth)
#else
#define lockdep_assert_none_held_once() do { } while (0)
#endif

#endif
