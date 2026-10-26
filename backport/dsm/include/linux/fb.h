/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_FB_H
#define _BACKPORT_DSM_FB_H

#include_next <linux/fb.h>

#define fb_io_read LINUX_BACKPORT(fb_io_read)
#define fb_io_write LINUX_BACKPORT(fb_io_write)
#define fb_io_mmap LINUX_BACKPORT(fb_io_mmap)

ssize_t fb_io_read(struct fb_info *, char __user *, size_t, loff_t *);
ssize_t fb_io_write(struct fb_info *, const char __user *, size_t, loff_t *);
int fb_io_mmap(struct fb_info *, struct vm_area_struct *);

#define fb_memcpy_fromio fb_memcpy_fromfb
#define fb_memcpy_toio fb_memcpy_tofb
#define fb_warn_once(info, fmt, ...) dev_warn_once((info)->device, fmt, ##__VA_ARGS__)

#define __FB_GEN_DEFAULT_DEFERRED_OPS_RDWR(__prefix, __damage_range, __mode) \
	static ssize_t __prefix ## _defio_read(struct fb_info *info, char __user *buf, \
					       size_t count, loff_t *ppos) \
	{ \
		return fb_ ## __mode ## _read(info, buf, count, ppos); \
	} \
	static ssize_t __prefix ## _defio_write(struct fb_info *info, const char __user *buf, \
						size_t count, loff_t *ppos) \
	{ \
		unsigned long offset = *ppos; \
		ssize_t ret = fb_ ## __mode ## _write(info, buf, count, ppos); \
		if (ret > 0) \
			__damage_range(info, offset, ret); \
		return ret; \
	}

#define __FB_GEN_DEFAULT_DEFERRED_OPS_DRAW(__prefix, __damage_area, __mode) \
	static void __prefix ## _defio_fillrect(struct fb_info *info, \
						const struct fb_fillrect *rect) \
	{ \
		__mode ## _fillrect(info, rect); \
		__damage_area(info, rect->dx, rect->dy, rect->width, rect->height); \
	} \
	static void __prefix ## _defio_copyarea(struct fb_info *info, \
						const struct fb_copyarea *area) \
	{ \
		__mode ## _copyarea(info, area); \
		__damage_area(info, area->dx, area->dy, area->width, area->height); \
	} \
	static void __prefix ## _defio_imageblit(struct fb_info *info, \
						 const struct fb_image *image) \
	{ \
		__mode ## _imageblit(info, image); \
		__damage_area(info, image->dx, image->dy, image->width, image->height); \
	}

#define FB_GEN_DEFAULT_DEFERRED_IOMEM_OPS(__prefix, __damage_range, __damage_area) \
	__FB_GEN_DEFAULT_DEFERRED_OPS_RDWR(__prefix, __damage_range, io) \
	__FB_GEN_DEFAULT_DEFERRED_OPS_DRAW(__prefix, __damage_area, cfb)

#define FB_GEN_DEFAULT_DEFERRED_SYSMEM_OPS(__prefix, __damage_range, __damage_area) \
	__FB_GEN_DEFAULT_DEFERRED_OPS_RDWR(__prefix, __damage_range, sys) \
	__FB_GEN_DEFAULT_DEFERRED_OPS_DRAW(__prefix, __damage_area, sys)

#define FB_GEN_DEFAULT_DEFERRED_DMAMEM_OPS(__prefix, __damage_range, __damage_area) \
	__FB_GEN_DEFAULT_DEFERRED_OPS_RDWR(__prefix, __damage_range, sys) \
	__FB_GEN_DEFAULT_DEFERRED_OPS_DRAW(__prefix, __damage_area, sys)

/*
 * Initializes struct fb_ops for deferred I/O.
 */

#define __FB_DEFAULT_DEFERRED_OPS_RDWR(__prefix) \
	.fb_read	= __prefix ## _defio_read, \
	.fb_write	= __prefix ## _defio_write

#define __FB_DEFAULT_DEFERRED_OPS_DRAW(__prefix) \
	.fb_fillrect	= __prefix ## _defio_fillrect, \
	.fb_copyarea	= __prefix ## _defio_copyarea, \
	.fb_imageblit	= __prefix ## _defio_imageblit

#define __FB_DEFAULT_DEFERRED_OPS_MMAP(__prefix) \
	.fb_mmap	= fb_deferred_io_mmap

#define FB_DEFAULT_DEFERRED_OPS(__prefix) \
	__FB_DEFAULT_DEFERRED_OPS_RDWR(__prefix), \
	__FB_DEFAULT_DEFERRED_OPS_DRAW(__prefix), \
	__FB_DEFAULT_DEFERRED_OPS_MMAP(__prefix)

#endif
