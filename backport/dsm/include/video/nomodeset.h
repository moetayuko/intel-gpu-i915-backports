/* SPDX-License-Identifier: MIT */

#ifndef _BACKPORT_DSM_NOMODESET_H
#define _BACKPORT_DSM_NOMODESET_H

#include <linux/console.h>

/* Before video/nomodeset.h, DRM used the console's nomodeset flag. */

static inline bool video_firmware_drivers_only(void)
{
	return vgacon_text_force();
}

#endif
