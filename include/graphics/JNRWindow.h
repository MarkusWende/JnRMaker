/**
 * JnRMaker - A tile editor and jump and run game maker
 * See COPYRIGHT file at the top of the source tree.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the JnRMaker License Statement and
 * the GNU General Public License along with this program.
 *
 */

/**
 * @file JNRWindow.h
 * @brief This file contains the window class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */

#ifndef JNRWINDOW_H
#define JNRWINDOW_H

#pragma once
#include <string>
#include "MessageManager.h"

#include <GL/glew.h>    // Initialize with glewInit()

#include <SDL2/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES3)
#include <SDL_opengles3.h>
#else
#include <SDL_opengl.h>
#endif

#include <GLES3/gl32.h>

class JNRWindow
{
public:
	JNRWindow();
	~JNRWindow() {};

    void InitSDL();
    void InitGLEW();
    void CreateWindow();
    void CreateContext();
    void ConfigureOpenGL();

    std::string     GetGLSLVersion() { return glsl_version_; };
    SDL_Window*     GetWindow() { return window_; };
    SDL_GLContext   GetContext() { return gl_context_; };

    void CleanUp();

private:
    SDL_Window*     window_;
    SDL_GLContext   gl_context_;
    std::string glsl_version_;

};

#endif    // JNRWINDOW_H

