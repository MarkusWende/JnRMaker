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
 * @file main.cpp
 * @brief Root application file. Application window initialization and main application loop.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui-SFML.h"
#include "../include/events.h"
#include "../include/gui.h"
#include "../include/resource_manager.h"
#include "../include/scene.h"


int main()
{
  //glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  sf::RenderWindow window(sf::VideoMode(1800, 1024), "[NewProject.jrm] - JnRMaker");
  auto desktop = sf::VideoMode::getDesktopMode();
  window.setPosition(sf::Vector2i(desktop.width/2 - window.getSize().x/3, desktop.height/2 - window.getSize().y/2));
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  // Create gui object
  Gui appGui;
  // Create default scene object
  Scene appScene(800, 800);
  // Update Gui to display the windows
  appGui.Update(window.getSize().x, window.getSize().y, appScene);

  sf::Clock deltaClock;

  unsigned int oldWidth = window.getSize().x;
  unsigned int oldHeight = window.getSize().y;
  while (window.isOpen()) {
    processEvents(window);

    ImGui::SFML::Update(window, deltaClock.restart());

    if (oldWidth != window.getSize().x || oldHeight != window.getSize().y)
      appGui.Update(window.getSize().x, window.getSize().y, appScene);
    appGui.Render();

    appScene.Render();

    window.clear();
    ImGui::SFML::Render(window);
    window.display();

    oldWidth = window.getSize().x;
    oldHeight = window.getSize().y;
  }

  ImGui::SFML::Shutdown();

  return 0;
}
