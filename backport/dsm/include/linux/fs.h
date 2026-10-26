/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_FS_H
#define _BACKPORT_DSM_FS_H

#include_next <linux/fs.h>

static inline int vfs_mmap(struct file *file, struct vm_area_struct *vma)
{
	return file->f_op->mmap(file, vma);
}

#define get_file_active LINUX_BACKPORT(get_file_active)
struct file *get_file_active(struct file **ptr);

#endif
