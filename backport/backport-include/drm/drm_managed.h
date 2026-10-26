/* SPDX-License-Identifier: MIT */
/*
 * Copyright _ 2021 Intel Corporation
 */

#ifndef __BACKPORT_DRM_MANAGED_H__
#define __BACKPORT_DRM_MANAGED_H__

#include_next <drm/drm_managed.h>
#include <linux/mutex.h>
#include <linux/find.h>

#ifdef BPM_DRMM_WORKQUEUE_RELEASE_NOT_PRESENT
void __drmm_workqueue_release(struct drm_device *device, void *wq);
#endif

#ifdef BPM_DRMM_ALLOC_ORDERED_WORKQUEUE_NOT_PRESENT
#define drmm_alloc_ordered_workqueue(dev, fmt, flags, args...)					\
	({											\
		struct workqueue_struct *wq = alloc_ordered_workqueue(fmt, flags, ##args);	\
		wq ? ({										\
			int ret = drmm_add_action_or_reset(dev, __drmm_workqueue_release, wq);	\
			ret ? ERR_PTR(ret) : wq;						\
		}) :										\
			wq;									\
	})
#endif

#ifdef BPM_DRMM_MUTEX_INIT_NOT_PRESENT
void __drmm_mutex_release(struct drm_device *dev, void *res);

#define drmm_mutex_init(dev, lock) ({                                        \
	static struct lock_class_key __key;                                  \
	__mutex_init(lock, #lock, &__key);                                   \
	drmm_add_action_or_reset(dev, __drmm_mutex_release, lock);           \
})
#endif

#endif /* __BACKPORT_DRM_MANAGED_H__ */
