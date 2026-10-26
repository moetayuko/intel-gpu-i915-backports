#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([intel-i915-drm], [1.0], [linux-graphics@intel.com])

AC_SUBST([DOLLAR_SIGN],[$])

AC_CONFIG_AUX_DIR([src])
AC_CONFIG_HEADERS([src/backport-include/backport/backport_auto.h])

AM_INIT_AUTOMAKE

AC_PROG_CC

AC_CONFIG_MACRO_DIR([m4])

AC_I915_CONFIG

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
