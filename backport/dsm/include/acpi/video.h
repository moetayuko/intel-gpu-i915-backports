/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_ACPI_VIDEO_H
#define _BACKPORT_DSM_ACPI_VIDEO_H

#include_next <acpi/video.h>

/* In 5.10 video registration also registers backlight devices. */
#define acpi_video_register_backlight acpi_video_register

static inline bool acpi_video_backlight_use_native(void)
{
 return acpi_video_get_backlight_type() == acpi_backlight_native;
}

#endif
