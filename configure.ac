AC_PREREQ([2.65])

AC_INIT([3dinfo],[0.0.1])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([1.11 -Wall no-dist-gzip dist-xz subdir-objects])
# Support silent build rules. Disable
# by either passing --disable-silent-rules to configure or passing V=1
# to make
AM_SILENT_RULES([yes])

dnl SDL params to configure
AC_ARG_VAR(SDL_CONFIG, [sdl-config utility])
AC_ARG_VAR(SDL_CFLAGS, [C compiler flags for SDL])
AC_ARG_VAR(SDL_LIBS, [libraries to pass to the linker for SDL])

dnl If AM_PROG_AR exists, call it, to shut up a libtool warning (and make it more portable, I guess?)
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

dnl libtool
LT_PREREQ([2.2])
LT_INIT([disable-shared])

dnl We're C++
AC_PROG_CXX
AM_PROG_CC_C_O
dnl We want a make install
AC_PROG_INSTALL

dnl --with-werror
AC_ARG_WITH([werror], [AS_HELP_STRING([--with-werror], [Compile with -Werror @<:@default=no@:>@])], [], [with_werror=no])
if test "x$with_werror" = "xyes"; then
	WERROR="-Werror -Werror=unused-but-set-variable"
fi

dnl Standard C, C++
AC_C_CONST
AC_HEADER_STDC

dnl Endianness
AC_C_BIGENDIAN()

dnl Special variables of the size of pointers
AC_TYPE_INTPTR_T
AC_TYPE_UINTPTR_T

dnl math library and special float functions
AC_CHECK_LIB([m], [cos])
AC_CHECK_FUNCS([fabs])
AC_CHECK_FUNCS([fmin])
AC_CHECK_FUNCS([fmax])
AC_CHECK_FUNCS([fabsf])
AC_CHECK_FUNCS([fminf])
AC_CHECK_FUNCS([fmaxf])

dnl SDL
AC_CHECK_PROG([SDL_CONFIG], [sdl-config], [sdl-config])

if test -n "$SDL_CONFIG"; then
	if test -z "$SDL_CFLAGS"; then
		SDL_CFLAGS=`$SDL_CONFIG --cflags`
	fi
	if test -z "$SDL_LIBS"; then
		SDL_LIBS=`$SDL_CONFIG --libs`
	fi
fi

save_LIBS="$LIBS"
LIBS="$LIBS $SDL_LIBS"
AC_CHECK_FUNC(SDL_Init, , AC_CHECK_LIB(SDL, SDL_Init, SDL_LIBS="$SDL_LIBS -lSDL", nosdl=1))
LIBS="$save_LIBS"

if test -n "$nosdl"; then
	AC_MSG_ERROR([SDL is required and could not be found!])
fi

dnl OpenGL
case "$target" in
	*darwin*)
		dnl Do not run the OpenGL checks on OS X, they will incorrectly find the X11
		dnl OpenGL libraries which we do not want.
		dnl TODO: Eventually add a test for OS X for OpenGL

		GL_CFLAGS=""
		GL_LIBS="-framework OpenGL"
		;;
	*)
		AX_CHECK_GL

		if test -n "$no_gl"; then
			AC_MSG_ERROR([OpenGL is required and could not be found!])
		fi
		;;
esac;

dnl Extra flags
case "$target" in
	*darwin*)
		THREEDINFO_CFLAGS="-DUNIX -DMACOSX"
		THREEDINFO_LIBS=""
		;;
	*mingw*)
		THREEDINFO_CFLAGS=""
		THREEDINFO_LIBS="-static-libgcc -static-libstdc++"
		;;
	*)
		THREEDINFO_CFLAGS="-DUNIX"
		THREEDINFO_LIBS=""
		;;
esac;

AC_SUBST(SDL_LIBS)
AC_SUBST(SDL_CFLAGS)

AC_SUBST(GL_CFLAGS)
AC_SUBST(GL_LIBS)

AC_SUBST(THREEDINFO_CFLAGS)
AC_SUBST(THREEDINFO_LIBS)

AC_SUBST(WERROR)

AC_CONFIG_FILES([glew/Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
