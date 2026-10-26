/* SPDX-License-Identifier: MIT */
#ifndef _BACKPORT_LINUX_GPU_BUDDY_H
#define _BACKPORT_LINUX_GPU_BUDDY_H

#ifdef HAVE_LINUX_GPU_BUDDY_H
#include_next <linux/gpu_buddy.h>
#else
#include <drm/drm_buddy.h>

/* i915 uses the same interfaces under the newer GPU buddy names. */
#define gpu_buddy drm_buddy
#define gpu_buddy_block drm_buddy_block
#define gpu_buddy_init drm_buddy_init
#define gpu_buddy_fini drm_buddy_fini
#define gpu_buddy_alloc_blocks drm_buddy_alloc_blocks
#define gpu_buddy_free_list drm_buddy_free_list
#define gpu_buddy_block_offset drm_buddy_block_offset
#define gpu_buddy_block_size drm_buddy_block_size

#define GPU_BUDDY_RANGE_ALLOCATION DRM_BUDDY_RANGE_ALLOCATION
#define GPU_BUDDY_TOPDOWN_ALLOCATION DRM_BUDDY_TOPDOWN_ALLOCATION
#define GPU_BUDDY_CONTIGUOUS_ALLOCATION DRM_BUDDY_CONTIGUOUS_ALLOCATION
#endif

#endif /* _BACKPORT_LINUX_GPU_BUDDY_H */
