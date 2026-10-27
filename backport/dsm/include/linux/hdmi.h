/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_HDMI_H
#define _BACKPORT_DSM_HDMI_H

#if IS_ENABLED(CPTCFG_HDMI_BACKPORT)
#include <linux/hdmi-backport.h>
#else
#include_next <linux/hdmi.h>

#define HDMI_MAX_INFOFRAME_SIZE 27
#endif

#endif
