AC_DEFUN([AC_INTEL_GMCH_GTT_GET_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <drm/intel-gtt.h>
		], [
			(void)intel_gmch_gtt_get;
		], [
		], [
			AC_DEFINE([BPM_INTEL_GMCH_GTT_GET_NOT_PRESENT], 1,
				[intel_gmch_gtt_get() is not available])
		])
	])
])

AC_DEFUN([AC_INTEL_GMCH_GTT_READ_ENTRY_NOT_PRESENT], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TRY_COMPILE([
			#include <drm/intel-gtt.h>
		], [
			(void)intel_gmch_gtt_read_entry;
		], [
		], [
			AC_DEFINE([BPM_INTEL_GMCH_GTT_READ_ENTRY_NOT_PRESENT], 1,
				[intel_gmch_gtt_read_entry() is not available])
		])
	])
])
