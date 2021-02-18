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
 * @file jnr_window.h
 * @brief This file contains the window class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */

#ifndef JNRWINDOW_H
#define JNRWINDOW_H

#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

class JNRWindow {
public:
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
	SDL_Window* GetWindow();

	/**
	* @brief Gets the SDL_Window object.
	* @return Returns the SDL_Window object.
	*/
	SDL_GLContext GetGLContext();

	/**
	* @brief Gets the GLSL-Version.
	* @return Returns the GLSL-Version.
	*/
	const char* GetGLSLVersion();

private:
	/**
	* @brief SDL_Window object
	*/
	SDL_Window* window_;

	/**
	* @brief SDL Display Mode
	*/
	SDL_DisplayMode display_mode_;

	/**
	* @brief SDL GLContext
	*/
	SDL_GLContext gl_context_;

	/**
	* @brief GLSL-Version
	*/
	const char* glsl_version_;
};




/*
#include <GL/glew.h>

#include <stdio.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/OpenGL.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "time_helper.h"
#include "project_manager.h"

class JnRWindow
{
public:
	JnRWindow()														//!< constructor
	{
        settings_.depthBits = 32;
        settings_.stencilBits = 8;
        settings_.antialiasingLevel = 4;
        settings_.majorVersion = 4;
        settings_.minorVersion = 5;
        window_.create(sf::VideoMode(1800, 1024), ProjectManager::GetName() + " - JnRMaker", sf::Style::Default, settings_);
        auto desktop = sf::VideoMode::getDesktopMode();
        window_.setPosition(sf::Vector2i(desktop.width/2 - window_.getSize().x/2, desktop.height/2 - window_.getSize().y/2));
#ifdef _WIN32
        ShowWindow(window_.getSystemHandle(), SW_MAXIMIZE);
#endif // _WIN32

        //window_.setFramerateLimit(60);
        FILE* stream;
        //glewExperimental = GL_TRUE;
        if ( GLEW_OK != glewInit())
        {
#ifdef _WIN32
            freopen_s(&stream, "log.txt", "w", stdout);
            fprintf(stream, "Failed to initialize GLEW\n");
            fclose(stream);
#endif // _WIN32
#ifdef __linux__
            stream = fopen("./log.txt", "w");
            fprintf(stream, "Failed to initialize GLEW\n");
            fclose(stream);
#endif // __linux__
        }
        

        ImGui::SFML::Init(window_);

        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* version = glGetString(GL_VERSION);
        const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

        GLint major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

#ifdef _WIN32
        freopen_s(&stream, "log.txt", "w", stdout);

				if (stream)
        {
            fprintf(stream, "%s\tSuccessful initialized OpenGL...\n", time_helper::GetTimeinfo().c_str());
            fprintf(stream, "\t\t\t\t\tGL Vendor\t\t: %s\n", vendor);
            fprintf(stream, "\t\t\t\t\tGL Renderer\t\t: %s\n", renderer);
            fprintf(stream, "\t\t\t\t\tGL Version (string)\t: %s\n", version);
            fprintf(stream, "\t\t\t\t\tGL Version (integer)\t: %d.%d\n", major, minor);
            fprintf(stream, "\t\t\t\t\tGLSL Version\t\t: %s\n", glslVersion);
        }
#endif // _WIN32
#ifdef __linux__
			stream = fopen("./log.txt", "w");
			fprintf(stream, "%s\tSuccessful initialized OpenGL...\n", time_helper::GetTimeinfo().c_str());
			fprintf(stream, "\t\t\t\t\tGL Vendor\t\t: %s\n", vendor);
			fprintf(stream, "\t\t\t\t\tGL Renderer\t\t: %s\n", renderer);
			fprintf(stream, "\t\t\t\t\tGL Version (string)\t: %s\n", version);
			fprintf(stream, "\t\t\t\t\tGL Version (integer)\t: %d.%d\n", major, minor);
			fprintf(stream, "\t\t\t\t\tGLSL Version\t\t: %s\n", glslVersion);
#endif // __linux__
			fclose(stream);

	}

	~JnRWindow()
	{
		window_.close();
	};

	sf::RenderWindow*   Get() { return &window_; };
    sf::Vector2i        GetMousePosition() { return sf::Mouse::getPosition(window_); };

private:
	sf::RenderWindow window_;
    sf::ContextSettings settings_;

};
*/
#endif    // JNRWINDOW_H

