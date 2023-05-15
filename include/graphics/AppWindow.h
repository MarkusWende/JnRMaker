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

#ifndef APPWINDOW_H
#define APPWINDOW_H

#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include <memory>

#ifdef __EMSCRIPTEN__
    
#else
    #include <SDL2/SDL_image.h>
#endif

// #include <GLFW/glfw3.h>
// #include <iostream>

// #include "../include/ResourceManager.h"

#include "MessageManager.h"

class AppWindow {
public:
	AppWindow() {};
	~AppWindow() {};

	/**
	* @brief Calls necessary functions before using SDL
	* @return Return whether process was successful
	*/
	int SetupSDL();

	/**
	* @brief Calls necessary functions to clean up before shutting down the program
	* @return Void.
	*/
	void CleanUp();

	/**
	* @brief Loads and displays the loading window
	* @return Void.
	*/
	void ShowLoadingWindow();

	/**
	* @brief Creates new SDL window
	* @param width Width of the new window
	* @param height Height of the new window
	* @param name Name displayed in the top bar of the new window.
	* @return Void.
	*/
	void CreateNewWindow(int width, int height, const char* name);

	/**
	* @brief Gets current width of the window.
	* @return Returns the current width of the window.
	*/
	int GetWidth();

	/**
	* @brief Sets the width of the window.
	* @param width New width of the window.
	* @return Void
	*/
	void SetWidth(int width);

	/**
	* @brief Gets current height of the window.
	* @return Returns the current height of the window.
	*/
	int GetHeight();

	/**
	* @brief Sets the height of the window.
	* @param height New height of the window.
	* @return Void.
	*/
	void SetHeight(int height);

	/**
	* @brief Sets the size of the window.
	* @param width New width of the window.
	* @param height New height of the window.
	* @return Void.
	*/
	void SetSize(int width, int height);

	/**
	* @brief Gets the current name displayed in the topbar of the window.
	* @return Returns the name of the window.
	*/
	const char* GetName();

	/**
	* @brief Sets the name displayed in the topbar of the window.
	* @param name New name of the window.
	* @return Void.
	*/
	void SetName(const char* name);

	/**
	* @brief Gets the SDL_Window object.
	* @return Returns the SDL_Window object.
	*/
	std::shared_ptr<SDL_Window> GetWindow();

	/**
	* @brief Gets the SDL_Window object.
	* @return Returns the SDL_Window object.
	*/
	std::shared_ptr<SDL_GLContext> GetGLContext();

	/**
	* @brief Gets the GLSL-Version.
	* @return Returns the GLSL-Version.
	*/
	const char* GetGLSLVersion();

private:
	/**
	* @brief SDL_Window object
	*/
	std::shared_ptr<SDL_Window> window_;

	/**
	* @brief SDL Display Mode
	*/
	SDL_DisplayMode display_mode_;

	/**
	* @brief SDL GLContext
	*/
	std::shared_ptr<SDL_GLContext> gl_context_;

	/**
	* @brief GLSL-Version
	*/
	std::string glsl_version_;
};

#endif    // JNRWINDOW_H

