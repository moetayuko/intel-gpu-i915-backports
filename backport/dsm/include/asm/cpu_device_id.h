/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_CPU_DEVICE_ID_H
#define _BACKPORT_DSM_CPU_DEVICE_ID_H

#include_next <asm/cpu_device_id.h>

#define VFM_MODEL(vfm) ((vfm) & 0xff)
#define VFM_FAMILY(vfm) (((vfm) >> 8) & 0xff)
#define VFM_VENDOR(vfm) (((vfm) >> 16) & 0xff)
#define VFM_MAKE(vendor, family, model) \
	(((vendor) << 16) | ((family) << 8) | (model))
#define X86_MATCH_VFM(vfm, data) { \
	.vendor = VFM_VENDOR(vfm), .family = VFM_FAMILY(vfm), \
	.model = VFM_MODEL(vfm), .steppings = X86_STEPPING_ANY, \
	.feature = X86_FEATURE_ANY, .driver_data = (unsigned long)(data), \
}

#endif
