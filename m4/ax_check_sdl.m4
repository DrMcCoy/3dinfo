#serial 1000
dnl @synopsis AX_CHECK_SDL(version, action-if, action-if-not)
dnl
dnl @summary check for SDL of sufficient version (2.1.3 -> 2103)
dnl
dnl Defines SDL_LIBS, SDL_CFLAGS. See sdl-config(1) man
dnl page.
dnl
dnl @category InstalledPackages
dnl @author Sven Hesse <drmccoy@drmccoy.de>
dnl @version 2013-12-14
dnl @license Creative Commons CC0 1.0 Universal Public Domain Dedication

AC_DEFUN([AX_CHECK_SDL], [
	AC_ARG_VAR(SDL_CONFIG, [sdl-config utility])
	AC_ARG_VAR(SDL_CFLAGS, [C compiler flags for SDL])
	AC_ARG_VAR(SDL_LIBS, [libraries to pass to the linker for SDL])

	AC_CHECK_PROG([SDL_CONFIG], [sdl-config], [sdl-config])

	if test -n "$SDL_CONFIG"; then
		if test -z "$SDL_CFLAGS"; then
			SDL_CFLAGS=`$SDL_CONFIG --cflags`
		fi
		if test -z "$SDL_LIBS"; then
			SDL_LIBS=`$SDL_CONFIG --libs`
		fi
	fi

	save_CFLAGS="$CFLAGS"
	save_CXXFLAGS="$CXXFLAGS"
	save_LIBS="$LIBS"
	CFLAGS="$CFLAGS $SDL_CFLAGS"
	CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
	LIBS="$LIBS $SDL_LIBS"

	AC_LINK_IFELSE([AC_LANG_SOURCE([[
#include <SDL/SDL.h>

int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Quit();

#if SDL_COMPILEDVERSION < $1
	#error SDL version too low
#endif

	return 0;
}
]])], nosdl=0, nosdl=1)

	CFLAGS="$save_CFLAGS"
	CXXFLAGS="$save_CXXFLAGS"
	LIBS="$save_LIBS"

	if test "$nosdl" = "1"; then
		ifelse([$3], , :, [$3])
	else
		ifelse([$2], , :, [$2])
	fi

	AC_SUBST(SDL_CFLAGS)
	AC_SUBST(SDL_LIBS)
])
