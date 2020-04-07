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
**/

 #include <iostream>

#include <SFML/Window/Mouse.hpp>

 #include "../include/events.h"
 #include "../include/gui.h"
 #include "../include/resource_manager.h"
 #include "../include/scene.h"
 #include "../include/jnr_window.h"

int main()
{
  //glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  JnRWindow appWindow;
  sf::RenderWindow* window = appWindow.Get();

  // Create gui object
  Gui appGui;
  // Create default scene object
  Scene appScene(640, 480);
  // Update Gui to display the windows
  appGui.Update(window->getSize().x, window->getSize().y, appScene);

  sf::Clock deltaClock;

  // Always save the window size each loop cycle
  unsigned int oldWidth = window->getSize().x;
  unsigned int oldHeight = window->getSize().y;

  // Main Loop
  while (window->isOpen()) {
    processEvents(*window, appScene);

    ImGui::SFML::Update(*window, deltaClock.restart());
    // If the window size change, update gui and scene
    if (oldWidth != window->getSize().x || oldHeight != window->getSize().y)
      appGui.Update(window->getSize().x, window->getSize().y, appScene);

    // Render Gui
    appGui.Render(appScene);
    // Render Scene
    
    appScene.Render(appWindow.GetMousePosition());
    // Clear window and render it
    window->clear();
    ImGui::SFML::Render(*window);
    window->display();
    // Save old window size
    oldWidth = window->getSize().x;
    oldHeight = window->getSize().y;
  }

  window = nullptr;
  ImGui::SFML::Shutdown();

  return 0;
}
