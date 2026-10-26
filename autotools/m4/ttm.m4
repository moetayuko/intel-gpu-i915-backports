AC_DEFUN([AC_TTM_DEVICE_INIT_ALLOC_FLAGS_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <drm/ttm/ttm_device.h>
		], [
			(void)ttm_device_init(NULL, NULL, NULL, NULL, NULL, 0);
		], [
			AC_DEFINE([BPM_TTM_DEVICE_INIT_ALLOC_FLAGS_PRESENT], [1],
				[ttm_device_init() has an allocation flags argument])
		], [])
	])
])

AC_DEFUN([AC_TTM_BO_FINI_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <drm/ttm/ttm_bo.h>
		], [
			(void)ttm_bo_fini;
		], [
			AC_DEFINE([BPM_TTM_BO_FINI_PRESENT], [1],
				[ttm_bo_fini() is available])
		], [])
	])
])
