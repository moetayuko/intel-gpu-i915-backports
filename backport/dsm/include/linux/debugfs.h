/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_DEBUGFS_H
#define _BACKPORT_DSM_DEBUGFS_H

#include_next <linux/debugfs.h>

#define debugfs_create_str LINUX_BACKPORT(debugfs_create_str)
void debugfs_create_str(const char *, umode_t, struct dentry *, char **);

#endif
