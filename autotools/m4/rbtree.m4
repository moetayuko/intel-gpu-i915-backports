AC_DEFUN([AC_RB_ADD_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <linux/rbtree.h>
		], [
			rb_add(NULL, NULL, NULL);
		], [], [
			AC_DEFINE(BPM_RB_ADD_NOT_PRESENT, 1,
				  [rb_add() is not available in the kernel])
		])
	])
])

AC_DEFUN([AC_RB_FIND_ADD_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <linux/rbtree.h>
		], [
			rb_find_add(NULL, NULL, NULL);
		], [], [
			AC_DEFINE(BPM_RB_FIND_ADD_NOT_PRESENT, 1,
				  [rb_find_add() is not available in the kernel])
		])
	])
])
