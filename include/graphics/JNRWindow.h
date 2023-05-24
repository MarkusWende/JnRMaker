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
#include "UILogger.h"

class JNRWindow
{
public:
	JNRWindow(std::shared_ptr<ILogger> logger);
	~JNRWindow() {};

    void InitSDL();
    void InitGLEW();
    void CreateSDLWindow();
    void CreateSDLContext();
    void ConfigureOpenGL();

    std::string     GetGLSLVersion() { return glsl_version_; };
    SDL_Window*     GetWindow() { return window_; };
    SDL_GLContext   GetContext() { return gl_context_; };

    void CleanUp();

private:
    SDL_Window*     window_;
    SDL_GLContext   gl_context_;
    std::string glsl_version_;
    std::shared_ptr<ILogger> 				logger_;

};

#endif    // JNRWINDOW_H

