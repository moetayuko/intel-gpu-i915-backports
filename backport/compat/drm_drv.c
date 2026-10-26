/*
 * Created: Fri Jan 19 10:48:35 2001 by faith@acm.org
 *
 * Copyright 2001 VA Linux Systems, Inc., Sunnyvale, California.
 * All Rights Reserved.
 *
 * Author Rickard E. (Rik) Faith <faith@valinux.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */


#include <linux/bitops.h>
#include <linux/debugfs.h>
#include <linux/export.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/mount.h>
#include <linux/pseudo_fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/sprintf.h>
#include <linux/srcu.h>
#include <linux/xarray.h>

#include <drm/drm_accel.h>
#include <drm/drm_bridge.h>
#include <drm/drm_cache.h>
#include <drm/drm_color_mgmt.h>
#include <drm/drm_drv.h>
#include <drm/drm_file.h>
#include <drm/drm_managed.h>
#include <drm/drm_mode_object.h>
#include <drm/drm_print.h>
#include <drm/drm_privacy_screen_machine.h>

#ifdef BPM_DRM_WEDGE_RECOVERY_COLD_RESET_NOT_PRESENT

#define WEDGE_STR_LEN	32
#define PID_STR_LEN	15
#define COMM_STR_LEN	(TASK_COMM_LEN + 5)

/*
 * Available recovery methods for wedged device. To be sent along with device
 * wedged uevent.
 */
static const char *drm_get_wedge_recovery(unsigned int opt)
{
	switch (BIT(opt)) {
	case DRM_WEDGE_RECOVERY_NONE:
		return "none";
	case DRM_WEDGE_RECOVERY_REBIND:
		return "rebind";
	case DRM_WEDGE_RECOVERY_BUS_RESET:
		return "bus-reset";
	case DRM_WEDGE_RECOVERY_VENDOR:
		return "vendor-specific";
	case DRM_WEDGE_RECOVERY_COLD_RESET:
		return "cold-reset";
	default:
		return NULL;
	}
}

/**
 * drm_dev_wedged_event - generate a device wedged uevent
 * @dev: DRM device
 * @method: method(s) to be used for recovery
 * @info: optional information about the guilty task
 *
 * This generates a device wedged uevent for the DRM device specified by @dev.
 * Recovery @method\(s) of choice will be sent in the uevent environment as
 * ``WEDGED=<method1>[,..,<methodN>]`` in order of less to more side-effects.
 * If caller is unsure about recovery or @method is unknown (0),
 * ``WEDGED=unknown`` will be sent instead.
 *
 * Refer to "Device Wedging" chapter in Documentation/gpu/drm-uapi.rst for more
 * details.
 *
 * Returns: 0 on success, negative error code otherwise.
 */
int backport_drm_dev_wedged_event(struct drm_device *dev, unsigned long method,
			 struct drm_wedge_task_info *info)
{
	char event_string[WEDGE_STR_LEN], pid_string[PID_STR_LEN], comm_string[COMM_STR_LEN];
	char *envp[] = { event_string, NULL, NULL, NULL };
	const char *recovery = NULL;
	unsigned int len, opt;

	len = scnprintf(event_string, sizeof(event_string), "%s", "WEDGED=");

	for_each_set_bit(opt, &method, BITS_PER_TYPE(method)) {
		recovery = drm_get_wedge_recovery(opt);
		if (drm_WARN_ONCE(dev, !recovery, "invalid recovery method %u\n", opt))
			break;

		len += scnprintf(event_string + len, sizeof(event_string) - len, "%s,", recovery);
	}

	if (recovery)
		/* Get rid of trailing comma */
		event_string[len - 1] = '\0';
	else
		/* Caller is unsure about recovery, do the best we can at this point. */
		snprintf(event_string, sizeof(event_string), "%s", "WEDGED=unknown");

	drm_info(dev, "device wedged, %s\n", method == DRM_WEDGE_RECOVERY_NONE ?
		 "but recovered through reset" : "needs recovery");

	if (info && (info->comm[0] != '\0') && (info->pid >= 0)) {
		snprintf(pid_string, sizeof(pid_string), "PID=%u", info->pid);
		snprintf(comm_string, sizeof(comm_string), "TASK=%s", info->comm);
		envp[1] = pid_string;
		envp[2] = comm_string;
	}

	return kobject_uevent_env(&dev->primary->kdev->kobj, KOBJ_CHANGE, envp);
}
#define drm_dev_wedged_event backport_drm_dev_wedged_event
EXPORT_SYMBOL(drm_dev_wedged_event);
#endif
