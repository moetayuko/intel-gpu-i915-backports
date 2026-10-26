dnl #
dnl # v6.4-1aaba11da9aa
dnl # driver core: class: remove module * from class_create()
dnl #
AC_DEFUN([AC_CLASS_CREATE_NO_OWNER_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <linux/device.h>
		],[
			class_create("test_class");
		],[
		],[
			AC_DEFINE(BPM_CLASS_CREATE_NO_OWNER_NOT_PRESENT, 1,
				[class_create() still requires owner parameter])
		])
	])
])
