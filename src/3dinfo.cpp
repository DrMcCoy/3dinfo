/* 3DInfo - Print information about your graphics hardware.
 *
 * 3DInfo is the legal property of its developers, whose labels can be
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

/** @file 3dinfo.cpp
 *  Print information about your graphics hardware.
 */

#include <cassert>
#include <stdexcept>
#include <list>

#include <SDL.h>

#include "glew/glew.h"

#include "capability.h"


#ifdef ARRAYSIZE
	#undef ARRAYSIZE
#endif

#define ARRAYSIZE(x) ((int)(sizeof(x) / sizeof(x[0])))

void dispHelp(const char *name);

SDL_Surface *init(int width, int height, bool fullscreen, bool experimental);
SDL_Surface *initSize(int width, int height, bool fullscreen, bool experimental);
SDL_Surface *setupSDLGL(int width, int height, int bpp, unsigned int flags);
void deinit();

void getCapabilities(std::list<Capability> &caps);
void displayCapabilities(std::list<Capability> &capsNormal, std::list<Capability> &capsExp);

int getGLParamInt(GLenum param, GLint def) {
	glGetIntegerv(param, &def);
	return def;
}

#define CHECK_GL_VERSION(x, y) (x), ((bool)GLEW_VERSION_##y)
#define CHECK_GL_EXTENSION(x) (#x "?"), ((bool)x)
#define CHECK_GL_FUNCTION(x) CHECK_GL_EXTENSION(x)
#define CHECK_GL_PARAM_INT(x, def) (#x "?"), getGLParamInt(x, def)

int main(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
			dispHelp(argv[0]);
			return 0;
		}

		// Unknown parameter
		dispHelp(argv[0]);
		return -1;
	}


	std::list<Capability> _capNormal;
	init(800, 600, false, false);
	getCapabilities(_capNormal);

	std::list<Capability> _capExperimental;
	init(800, 600, false, true);
	getCapabilities(_capExperimental);

	displayCapabilities(_capNormal, _capExperimental);

	deinit();
	return 0;
}

void getCapabilities(std::list<Capability> &caps) {
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 1.1?",   1_1)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 1.2?",   1_2)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 1.2.1?", 1_2_1)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 1.3?",   1_3)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 1.4?",   1_4)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 1.5?",   1_5)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 2.0?",   2_0)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 2.1?",   2_1)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 3.0?",   3_0)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 3.1?",   3_1)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 3.2?",   3_2)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 3.3?",   3_3)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 4.0?",   4_0)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 4.1?",   4_1)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 4.2?",   4_2)));
	caps.push_back(Capability(CHECK_GL_VERSION("OpenGL 4.3?",   4_3)));

	caps.push_back(Capability());

	caps.push_back(Capability("GLSL:", std::string((const char *)glGetString(GL_SHADING_LANGUAGE_VERSION))));

	caps.push_back(Capability());

	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_NV_gpu_program4)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_NV_vertex_program3)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_ARB_fragment_program)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_ARB_shading_language_100)));

	caps.push_back(Capability());

	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_EXT_vertex_shader)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_ARB_vertex_shader)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_EXT_geometry_shader4)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_ARB_geometry_shader4)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_EXT_Cg_shader)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_EXT_gpu_shader4)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_ARB_fragment_shader)));

	caps.push_back(Capability());

	caps.push_back(Capability(CHECK_GL_FUNCTION(glCreateShader)));
	caps.push_back(Capability(CHECK_GL_FUNCTION(glCompileShader)));
	caps.push_back(Capability(CHECK_GL_FUNCTION(glUseProgram)));
	caps.push_back(Capability(CHECK_GL_FUNCTION(glUniformMatrix4fv)));

	caps.push_back(Capability());

	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_EXT_bgra)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_ARB_texture_non_power_of_two)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_ARB_texture_rectangle)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_EXT_texture_rectangle)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_NV_texture_rectangle)));

	caps.push_back(Capability());

	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_ARB_framebuffer_object)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_EXT_framebuffer_object)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_EXT_framebuffer_blit)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_EXT_framebuffer_multisample)));
	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_EXT_packed_depth_stencil)));

	caps.push_back(Capability());

	caps.push_back(Capability(CHECK_GL_EXTENSION(GLEW_ARB_vertex_buffer_object)));

	caps.push_back(Capability());

	caps.push_back(Capability(CHECK_GL_PARAM_INT(GL_MAX_ELEMENTS_INDICES, -1)));
	caps.push_back(Capability(CHECK_GL_PARAM_INT(GL_MAX_ELEMENTS_VERTICES, -1)));
	caps.push_back(Capability(CHECK_GL_PARAM_INT(GL_MAX_VARYING_COMPONENTS, -1)));
	caps.push_back(Capability(CHECK_GL_PARAM_INT(GL_MAX_VERTEX_ATTRIBS, -1)));
	caps.push_back(Capability(CHECK_GL_PARAM_INT(GL_MAX_VERTEX_UNIFORM_COMPONENTS, -1)));
	caps.push_back(Capability(CHECK_GL_PARAM_INT(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, -1)));
}

void displayCapabilities(std::list<Capability> &capsNormal, std::list<Capability> &capsExp) {
	std::list<Capability>::iterator cN, cE;

	unsigned int maxLength = 0;
	for (cN = capsNormal.begin(); cN != capsNormal.end(); ++cN)
		maxLength = (cN->getLabel().size() > maxLength) ? cN->getLabel().size() : maxLength;

	for (cN = capsNormal.begin(), cE = capsExp.begin(); cN != capsNormal.end() && cE != capsExp.end(); ++cN, ++cE) {
		assert(cN->getLabel() == cE->getLabel());
		assert(cN->getType() == cE->getType());

		printf("%-*s", maxLength, cN->getLabel().c_str());

		if (cN->getType() == kTypeBool) {

			if      (cE->getBool() && !cN->getBool())
				printf(" experimental");
			else if (!cE->getBool() && cN->getBool())
				printf(" deprecated");
			else if (cE->getBool())
				printf(" yes");
			else if (!cE->getBool())
				printf(" no");

		} else if (cN->getType() == kTypeInt) {
			if (cN->getInt() == cE->getInt())
				printf(" %d", cN->getInt());
			else
				printf(" %d (%d)", cN->getInt(), cE->getInt());
		} else if (cN->getType() == kTypeString) {
			if (cN->getString() == cE->getString())
				printf(" %s", cN->getString().c_str());
			else
				printf(" %s (%s)", cN->getString().c_str(), cE->getString().c_str());
		}

		printf("\n");
	}
}

void dispHelp(const char *name) {
	printf("Usage: %s [option]\n\n", name);
	printf("  -h      --help           This text\n");
	printf("\n");
}

SDL_Surface *init(int width, int height, bool fullscreen, bool experimental) {
	if ((width <= 0) || (height <= 0))
		throw std::runtime_error("Invalid dimensions");

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::runtime_error(SDL_GetError());

	SDL_Surface *screen = initSize(width, height, fullscreen, experimental);
	if (!screen)
		throw std::runtime_error(std::string("Failed setting the video mode: ") + SDL_GetError());

	return screen;
}

SDL_Surface *initSize(int width, int height, bool fullscreen, bool experimental) {
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

	glewExperimental = experimental;
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
