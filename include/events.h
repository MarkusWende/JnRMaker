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

//#include <SFML/Graphics/RenderWindow.hpp>
//#include <SFML/Window/Event.hpp>

//#include "imgui/imgui-SFML.h"
#include "scene.h"
#include "gui.h"

unsigned int lastTime, currentTime;

/**
 * @brief A function that is called every main loop, to handle keyboard input.
 */
void processEvents(SDL_Window* window, Scene &scene, Gui& gui)
{
    lastTime = currentTime;
    currentTime = SDL_GetTicks();
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
    //ImGui::SFML::ProcessEvent(event);

        if (event.type == SDL_QUIT)
        {
            gui.Close();
        }

        if (scene.IsMouseOverScene())
        {
            // Pass the mouse position to the scene. The mouse position is import to calculate intersections.
            // TODO (Markus): Create a setter method for this.
            scene.SetMousePosition(glm::vec2(event.motion.x, event.motion.y));

            if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
            {
                float x = event.motion.xrel;
                float y = event.motion.yrel;

                if ((x != 107.0f && y < 1000.0f) && (scene.GetCamera("SceneCamera")->GetState() == CameraState::PERSPECTIVE))
                {
                    scene.GetCamera("SceneCamera")->ProcessMouseRotate(x, y, 0.002f);
                }
                else if (scene.GetCamera("SceneCamera")->GetState() == CameraState::ORTHOGRAPHIC)
                {
                    //std::stringstream msg;
                    //msg << "x: " << x << "\ty: " << y;
                    //MessageManager::AddMessage(msg, message_t::INFO);
                    scene.PlaceSprite();
                }
            }
            if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
            {
                float x = event.motion.xrel;
                float y = event.motion.yrel;

                if (x != 107.0f && y < 1000.0f)
                {
                    scene.GetCamera("SceneCamera")->ProcessMouseDrag(x, y, 0.002f);
                }
            }
            if (event.type == SDL_MOUSEWHEEL)
            {
                float y = event.wheel.y;
                scene.GetCamera("SceneCamera")->ProcessMouseScroll(y, 0.2f);
            }
        }

        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                gui.Close();
                break;
            default:
                break;
            }
        }
        /*
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
                    //scene.GetCamera("Editor")->SetMousePos(pos.x, pos.y);
                }
                else
                {
                    scene.GetCamera("FreeCamera")->Move(pos.x, pos.y);
                }
            }

            if (event.type == sf::Event::MouseWheelScrolled)
            {
                //scene.GetCamera("Editor")->Zoom(event.mouseWheelScroll.delta);
                //scene.GetCamera("Editor")->ZoomAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, event.mouseWheelScroll.delta);
            }
        }
        */
    }
}

#endif	// EVENTS_H
