// SPDX-License-Identifier: GPL-2.0
/* Adapted from linux-intel-lts fs/debugfs/file.c. */
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/rcupdate.h>
static ssize_t debugfs_read_file_str(struct file *file, char __user *user_buf,
			      size_t count, loff_t *ppos)
{
	struct dentry *dentry = file->f_path.dentry;
	char *str, *copy = NULL;
	int copy_len, len;
	ssize_t ret;

	ret = debugfs_file_get(dentry);
	if (unlikely(ret))
		return ret;

	str = *(char **)file->private_data;
	len = strlen(str) + 1;
	copy = kmalloc(len, GFP_KERNEL);
	if (!copy) {
		debugfs_file_put(dentry);
		return -ENOMEM;
	}

	copy_len = strscpy(copy, str, len);
	debugfs_file_put(dentry);
	if (copy_len < 0) {
		kfree(copy);
		return copy_len;
	}

	copy[copy_len] = '\n';

	ret = simple_read_from_buffer(user_buf, count, ppos, copy, len);
	kfree(copy);

	return ret;
}

static ssize_t debugfs_write_file_str(struct file *file, const char __user *user_buf,
				      size_t count, loff_t *ppos)
{
	struct dentry *dentry = file->f_path.dentry;
	char *old, *new = NULL;
	int pos = *ppos;
	int r;

	r = debugfs_file_get(dentry);
	if (unlikely(r))
		return r;

	old = *(char **)file->private_data;

	/* only allow strict concatenation */
	r = -EINVAL;
	if (pos && pos != strlen(old))
		goto error;

	r = -E2BIG;
	if (pos + count + 1 > PAGE_SIZE)
		goto error;

	r = -ENOMEM;
	new = kmalloc(pos + count + 1, GFP_KERNEL);
	if (!new)
		goto error;

	if (pos)
		memcpy(new, old, pos);

	r = -EFAULT;
	if (copy_from_user(new + pos, user_buf, count))
		goto error;

	new[pos + count] = '\0';
	strim(new);

	rcu_assign_pointer(*(char __rcu **)file->private_data, new);
	synchronize_rcu();
	kfree(old);

	debugfs_file_put(dentry);
	return count;

error:
	kfree(new);
	debugfs_file_put(dentry);
	return r;
}

static const struct file_operations fops_str = {
	.read =		debugfs_read_file_str,
	.write =	debugfs_write_file_str,
	.open =		simple_open,
	.llseek =	default_llseek,
};

static const struct file_operations fops_str_ro = {
	.read =		debugfs_read_file_str,
	.open =		simple_open,
	.llseek =	default_llseek,
};

static const struct file_operations fops_str_wo = {
	.write =	debugfs_write_file_str,
	.open =		simple_open,
	.llseek =	default_llseek,
};

/**
 * debugfs_create_str - create a debugfs file that is used to read and write a string value
 * @name: a pointer to a string containing the name of the file to create.
 * @mode: the permission that the file should have
 * @parent: a pointer to the parent dentry for this file.  This should be a
 *          directory dentry if set.  If this parameter is %NULL, then the
 *          file will be created in the root of the debugfs filesystem.
 * @value: a pointer to the variable that the file should read to and write
 *         from. This pointer and the string it points to must not be %NULL.
 *
 * This function creates a file in debugfs with the given name that
 * contains the value of the variable @value.  If the @mode variable is so
 * set, it can be read from, and written to.
 */
void debugfs_create_str(const char *name, umode_t mode,
			struct dentry *parent, char **value)
{
	if (WARN_ON(!value || !*value))
		return;

	const struct file_operations *ops = &fops_str;

 if (!(mode & 0222))
  ops = &fops_str_ro;
 else if (!(mode & 0444))
  ops = &fops_str_wo;
 debugfs_create_file_unsafe(name, mode, parent, value, ops);
}
EXPORT_SYMBOL_GPL(debugfs_create_str);
