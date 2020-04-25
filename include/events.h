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
 * @file events.h
 * @brief This file contains functions that handle events like keyboard and mouse inputs.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#ifndef EVENTS_H
#define EVENTS_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "imgui/imgui-SFML.h"
#include "scene.h"
#include "gui.h"


/**
 * @brief A function that is called every main loop, to handle keyboard input.
 */
void processEvents(sf::RenderWindow &window, Scene &scene, Gui& gui)
{
  sf::Event event;
  while (window.pollEvent(event)) {
    ImGui::SFML::ProcessEvent(event);

    if (event.type == sf::Event::Closed) {
        gui.Close();
    }

    if (event.type == sf::Event::Resized)
    {
        // update the view to the new size of the window
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(visibleArea));
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        gui.Close();
    }

    if (scene.IsMouseOverScene())
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            scene.SetAddSpriteFlag();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
        {
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            if (event.type == sf::Event::MouseButtonPressed)
            {
                scene.GetCamera("Editor")->SetMousePos(pos.x, pos.y);
            }
            else
            {
                scene.GetCamera("Editor")->Move(pos.x, pos.y);
            }
        }

        if (event.type == sf::Event::MouseWheelScrolled)
        {
            //scene.GetCamera("Editor")->Zoom(event.mouseWheelScroll.delta);
            scene.GetCamera("Editor")->ZoomAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, event.mouseWheelScroll.delta);
        }
    }
  }
}

#endif	// EVENTS_H
