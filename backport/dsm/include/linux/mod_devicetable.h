/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_MOD_DEVICETABLE_H
#define _BACKPORT_DSM_MOD_DEVICETABLE_H

#include_next <linux/mod_devicetable.h>

#define AUXILIARY_NAME_SIZE 32
#define AUXILIARY_MODULE_PREFIX "auxiliary:"

struct auxiliary_device_id {
	char name[AUXILIARY_NAME_SIZE];
	kernel_ulong_t driver_data;
};

#endif
