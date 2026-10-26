dnl #
dnl # v7.0-rc1 - e19e1b480ac7
dnl # add default_gfp() helper macro and use it in the new *alloc_obj() helpers
dnl #

AC_DEFUN([AC_KMALLOC_OBJ_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <linux/slab.h>
		],[
			void *p = NULL;
			kmalloc_obj(*p);
		],[
		],[
			AC_DEFINE(BPM_KMALLOC_OBJ_NOT_PRESENT, 1,
				[kmalloc_obj() is not available])
		])
	])
])

AC_DEFUN([AC_KZALLOC_OBJ_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <linux/slab.h>
		],[
			void *p = NULL;
			kzalloc_obj(*p);
		],[
		],[
			AC_DEFINE(BPM_KZALLOC_OBJ_NOT_PRESENT, 1,
				[kzalloc_obj() is not available])
		])
	])
])

AC_DEFUN([AC_KZALLOC_OBJS_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <linux/slab.h>
		],[
			void *p = NULL;
			kzalloc_objs(*p, 1);
		],[
		],[
			AC_DEFINE(BPM_KZALLOC_OBJS_NOT_PRESENT, 1,
				[kzalloc_objs() is not available])
		])
	])
])

AC_DEFUN([AC_KMALLOC_OBJS_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <linux/slab.h>
		],[
			void *p = NULL;
			kmalloc_objs(*p, 1);
		],[
		],[
			AC_DEFINE(BPM_KMALLOC_OBJS_NOT_PRESENT, 1,
				[kmalloc_objs() is not available])
		])
	])
])

AC_DEFUN([AC_KVZALLOC_OBJS_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <linux/slab.h>
		],[
			void *p = NULL;
			kvzalloc_objs(*p, 1);
		],[
		],[
			AC_DEFINE(BPM_KVZALLOC_OBJS_NOT_PRESENT, 1,
				[kvzalloc_objs() is not available])
		])
	])
])

AC_DEFUN([AC_KVMALLOC_OBJS_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <linux/slab.h>
		],[
			void *p = NULL;
			kvmalloc_objs(*p, 1);
		],[
		],[
			AC_DEFINE(BPM_KVMALLOC_OBJS_NOT_PRESENT, 1,
				[kvmalloc_objs() is not available])
		])
	])
])

AC_DEFUN([AC_KZALLOC_FLEX_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <linux/slab.h>
		],[
			struct test {
				int a;
				int b[];
			};
			struct test *p = NULL;
			p = kzalloc_flex(*p, b, 1);
		],[
		],[
			AC_DEFINE(BPM_KZALLOC_FLEX_NOT_PRESENT, 1,
				[kzalloc_flex() is not available])
		])
	])
])

AC_DEFUN([AC_KREALLOC_ARRAY_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <linux/slab.h>
		], [
			void *ptr = krealloc_array(NULL, 2, 4, GFP_KERNEL);
			kfree(ptr);
		], [], [
			AC_DEFINE(BPM_KREALLOC_ARRAY_NOT_PRESENT, 1,
				[krealloc_array() is not available])
		])
	])
])
