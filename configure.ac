#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([<autotoolscpplibtemplate>], [1.0.0], [<owner-email>])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
LT_INIT
AC_ENABLE_SHARED
AC_DISABLE_STATIC
AC_PROG_LIBTOOL(libtool)

# Checks for header files.
AC_CHECK_HEADERS([stdarg.h stdio.h stdlib.h unistd.h sys/types.h sys/wait.h])
AC_CHECK_HEADERS([exception errno.h])
AC_CHECK_HEADERS([string map vector queue])
AC_CHECK_HEADERS([limits])
AC_CHECK_HEADERS([cstring fstream sstream])
AC_CHECK_HEADERS([signal.h])
AC_CHECK_HEADERS([sys/time.h time.h])
AC_CHECK_HEADERS(pthread.h,, [AC_MSG_ERROR([pthread.h required])])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.

# Output Makefile files.
AC_CONFIG_FILES([Makefile src/Makefile man/Makefile])
AC_OUTPUT
