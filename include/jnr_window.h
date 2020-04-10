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

#include <stdio.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/OpenGL.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "time_helper.h"

class JnRWindow
{
public:
	JnRWindow()														//!< constructor
	{
        window_.create(sf::VideoMode(1800, 1024), "[NewProject.jrm] - JnRMaker");
        auto desktop = sf::VideoMode::getDesktopMode();
        window_.setPosition(sf::Vector2i(desktop.width/2 - window_.getSize().x/2, desktop.height/2 - window_.getSize().y/2));
#ifdef _WIN32
        ShowWindow(window_.getSystemHandle(), SW_MAXIMIZE);
#endif // _WIN32

        //window_.setFramerateLimit(60);
        ImGui::SFML::Init(window_);

        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* version = glGetString(GL_VERSION);
        const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

        GLint major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

				FILE* stream;
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

};
#endif    // JNRWINDOW_H
