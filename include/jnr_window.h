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

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui-SFML.h"

class JnRWindow
{
public:
	JnRWindow()														//!< constructor
	{
		window_.create(sf::VideoMode(1800, 1024), "[NewProject.jrm] - JnRMaker");
	  auto desktop = sf::VideoMode::getDesktopMode();
	  window_.setPosition(sf::Vector2i(desktop.width/2 - window_.getSize().x/3, desktop.height/2 - window_.getSize().y/2));
	  //window_.setFramerateLimit(60);
	  ImGui::SFML::Init(window_);
	}

	~JnRWindow()
	{
		window_.close();
	};

	sf::RenderWindow* Get() { return &window_; };

private:
	sf::RenderWindow window_;

};
#endif    // JNRWINDOW_H
