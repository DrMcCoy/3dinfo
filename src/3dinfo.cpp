/* Akachi - A small self-contained 3D engine
 *
 * Akachi is the legal property of its developers, whose names can be
 * found in the AUTHORS file distributed with this source
 * distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/** @file akachitest.cpp
 *  Sample testing framework for the Akachi engine.
 */

#include <stdexcept>

#include <SDL.h>

#include "glew/glew.h"


#ifdef ARRAYSIZE
	#undef ARRAYSIZE
#endif

#define ARRAYSIZE(x) ((int)(sizeof(x) / sizeof(x[0])))


SDL_Surface *init(int width, int height, bool fullscreen);
SDL_Surface *initSize(int width, int height, bool fullscreen);
SDL_Surface *setupSDLGL(int width, int height, int bpp, unsigned int flags);
void deinit();

#define BOOL2STR(x) ((x) ? "yes" : "no")

#define CHECK_GL_VERSION(x, y) printf("OpenGL %s?\t%s\n", (x), (GLEW_VERSION_##y) ? "yes" : "no")
#define CHECK_GL_EXTENSION(x) printf(#x "? %s\n", BOOL2STR(x))
#define CHECK_GL_FUNCTION(x) CHECK_GL_EXTENSION(x)

int main(int argc, char **argv) {
	SDL_Surface *screen = 0;

	screen = init(800, 600, false);

	CHECK_GL_VERSION("1.1", 1_1);
	CHECK_GL_VERSION("1.2", 1_2);
	CHECK_GL_VERSION("1.2.1", 1_2_1);
	CHECK_GL_VERSION("1.3", 1_3);
	CHECK_GL_VERSION("1.4", 1_4);
	CHECK_GL_VERSION("1.5", 1_5);
	CHECK_GL_VERSION("2.0", 2_0);
	CHECK_GL_VERSION("2.1", 2_1);
	CHECK_GL_VERSION("3.0", 3_0);
	CHECK_GL_VERSION("3.1", 3_1);
	CHECK_GL_VERSION("3.2", 3_2);
	CHECK_GL_VERSION("3.3", 3_3);
	CHECK_GL_VERSION("4.0", 4_0);
	CHECK_GL_VERSION("4.1", 4_1);
	CHECK_GL_VERSION("4.2", 4_2);
	CHECK_GL_VERSION("4.3", 4_3);

	const char *glsl = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("\nGLSL: %s\n", glsl ? glsl : "no");

	printf("\n");
	CHECK_GL_EXTENSION(GL_NV_gpu_program4);
	CHECK_GL_EXTENSION(GL_NV_vertex_program3);
	CHECK_GL_EXTENSION(GL_ARB_fragment_program);
	printf("\n");
	CHECK_GL_EXTENSION(GL_EXT_vertex_shader);
	CHECK_GL_EXTENSION(GL_ARB_vertex_shader);
	CHECK_GL_EXTENSION(GL_EXT_geometry_shader4);
	CHECK_GL_EXTENSION(GL_ARB_geometry_shader4);
	CHECK_GL_EXTENSION(GL_EXT_Cg_shader);
	CHECK_GL_EXTENSION(GL_EXT_gpu_shader4);
	CHECK_GL_EXTENSION(GL_ARB_fragment_shader);
	printf("\n");
	CHECK_GL_FUNCTION(glCreateShader);
	CHECK_GL_FUNCTION(glCompileShader);
	CHECK_GL_FUNCTION(glUseProgram);
	CHECK_GL_FUNCTION(glUniformMatrix4fv);

	printf("\n");
	CHECK_GL_EXTENSION(GLEW_EXT_bgra);

	printf("\n");
	CHECK_GL_EXTENSION(GL_ARB_framebuffer_object);
	CHECK_GL_EXTENSION(GL_EXT_framebuffer_object);
	CHECK_GL_EXTENSION(GL_EXT_framebuffer_blit);
	CHECK_GL_EXTENSION(GL_EXT_framebuffer_multisample);
	CHECK_GL_EXTENSION(GL_EXT_packed_depth_stencil);

	deinit();
	return 0;
}

SDL_Surface *init(int width, int height, bool fullscreen) {
	if ((width <= 0) || (height <= 0))
		throw std::runtime_error("Invalid dimensions");

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::runtime_error(SDL_GetError());

	SDL_Surface *screen = initSize(width, height, fullscreen);
	if (!screen)
		throw std::runtime_error(std::string("Failed setting the video mode: ") + SDL_GetError());

	return screen;
}

SDL_Surface *initSize(int width, int height, bool fullscreen) {
	int bpp = SDL_GetVideoInfo()->vfmt->BitsPerPixel;
	if ((bpp != 16) && (bpp != 24) && (bpp != 32))
		throw std::runtime_error("Need 16, 24 or 32 bits per pixel");

	unsigned int flags = SDL_OPENGL;

	if (fullscreen)
		flags |= SDL_FULLSCREEN;

	// The way we try to find an optimal color mode is a bit complex:
	// We only want 16bpp as a fallback, but otherwise prefer the native value.
	// So, if we're currently in 24bpp or 32bpp, we try that one first, then the
	// other one and 16bpp only as a last resort.
	// If we're currently in 16bpp mode, we try the higher two first as well,
	// before being okay with native 16bpp mode.

	const int colorModes[] = { bpp == 16 ? 32 : bpp, bpp == 24 ? 32 : 24, 16 };

	SDL_Surface *screen = 0;
	for (int i = 0; i < ARRAYSIZE(colorModes); i++) {
		if ((screen = setupSDLGL(width, height, colorModes[i], flags)))
			break;
	}

	if (!screen)
		return 0;

	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
		return 0;

	return screen;
}

SDL_Surface *setupSDLGL(int width, int height, int bpp, unsigned int flags) {
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE    ,   8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE  ,   8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE   ,   8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE  ,   8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,   1);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);

	return SDL_SetVideoMode(width, height, bpp, flags);
}

void deinit() {
	SDL_Quit();
}
