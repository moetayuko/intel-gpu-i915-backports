AC_DEFUN([AC_I2C_ALGORITHM_XFER_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <linux/i2c.h>
		], [
			struct i2c_algorithm algo;
			algo.xfer = NULL;
		], [], [
			AC_DEFINE(BPM_I2C_ALGORITHM_XFER_NOT_PRESENT, 1,
				  [struct i2c_algorithm has no xfer member])
		])
	])
])
