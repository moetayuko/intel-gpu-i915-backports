/* SPDX-License-Identifier: MIT */
#ifndef _BACKPORT_DRM_MM_H
#define _BACKPORT_DRM_MM_H

#include_next <drm/drm_mm.h>

/* Intel LTS iterator used while relocating VF GGTT allocations. */
#ifndef drm_mm_for_each_node_in_range_safe
#define drm_mm_for_each_node_in_range_safe(node__, next__, mm__, start__, end__)	\
	for (node__ = __drm_mm_interval_first((mm__), (start__), (end__)-1), \
		next__ = list_next_entry(node__, node_list); \
	     node__->start < (end__);					\
	     node__ = next__, next__ = list_next_entry(next__, node_list))
#endif
#endif
