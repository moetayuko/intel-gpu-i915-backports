dnl # v5.13: saturating size arithmetic for allocation helpers.
AC_DEFUN([AC_SIZE_MUL_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <linux/overflow.h>
		], [
			size_t size = size_mul(2, 3);
			(void)size;
		], [], [
			AC_DEFINE(BPM_SIZE_MUL_NOT_PRESENT, 1,
				[size_mul() is not available])
		])
	])
])
