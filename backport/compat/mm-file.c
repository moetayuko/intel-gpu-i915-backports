// SPDX-License-Identifier: GPL-2.0
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/export.h>

void vma_set_file(struct vm_area_struct *vma, struct file *file)
{
	get_file(file);
	swap(vma->vm_file, file);
	fput(file);
}
EXPORT_SYMBOL_GPL(vma_set_file);

struct file *get_file_active(struct file **ptr)
{
	struct file *file;

	/* 5.10 frees the file allocation after an RCU grace period. */
	rcu_read_lock();
	file = READ_ONCE(*ptr);
	if (file && !get_file_rcu(file))
		file = NULL;
	rcu_read_unlock();
	return file;
}
EXPORT_SYMBOL_GPL(get_file_active);
