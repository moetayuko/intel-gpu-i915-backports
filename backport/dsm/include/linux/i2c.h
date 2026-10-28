/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BACKPORT_DSM_I2C_H
#define _BACKPORT_DSM_I2C_H

#include_next <linux/i2c.h>

#ifndef I2C_FUNC_SMBUS_EMUL_ALL
#define I2C_FUNC_SMBUS_EMUL_ALL (I2C_FUNC_SMBUS_EMUL | \
				 I2C_FUNC_SMBUS_READ_BLOCK_DATA | \
				 I2C_FUNC_SMBUS_BLOCK_PROC_CALL)
#endif

#endif
