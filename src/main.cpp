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

#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>

#include <SFML/Window/Mouse.hpp>

#include "../include/events.h"
#include "../include/gui.h"
#include "../include/resource_manager.h"
#include "../include/project_manager.h"
#include "../include/message_manager.h"
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
    //appGui.WindowUpdate(appScene, window->getSize().x, window->getSize().y);

    sf::Clock deltaClock;

    // Always save the window size each loop cycle
    GLuint oldWidth = window->getSize().x;
    GLuint oldHeight = window->getSize().y;

    ProjectManager::SetStatus(project_status_t::IDLE);
    std::string projectName = "TestProject";
    ProjectManager::SetName(projectName);

    // Main Loop
    while (appGui.IsOpen())
    {
        processEvents(*window, appScene, appGui);

        ImGui::SFML::Update(*window, deltaClock.restart());
        // If the window size change, update gui and scene
        if (oldWidth != window->getSize().x || oldHeight != window->getSize().y)
            appGui.WindowUpdate(appScene, window->getSize().x, window->getSize().y);

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

        if (ProjectManager::GetStatus() == project_status_t::SAVE)
        {
            // create and open a character archive for output
            std::ofstream ofs(ProjectManager::GetName() + ".jrm");

            // save data to archive
            {
                cereal::XMLOutputArchive ar(ofs, cereal::XMLOutputArchive::Options(6, true, false));
                //cereal::JSONOutputArchive ar(ofs);
                //cereal::BinaryOutputArchive ar(ofs);
                ar( CEREAL_NVP(appGui) );
                ar( CEREAL_NVP(appScene) );
                //ar( cereal::make_nvp("window_with_", oldWidth), cereal::make_nvp("window_height_", oldHeight) );

                long double c = pow(0.5, 1000);
                ar ( cereal::make_nvp("a_very_small_value_", c) );
            }

            ProjectManager::SetStatus(project_status_t::IDLE);
        }
        else if (ProjectManager::GetStatus() == project_status_t::LOAD)
        {
            std::ifstream is(ProjectManager::GetName() + ".jrm");
            cereal::XMLInputArchive ar(is);
            //cereal::JSONInputArchive ar(is);
            //cereal::BinaryInputArchive ar(is);

            GLuint width;
            GLuint height;

            ar( appGui );
            ar( appScene );
            //ar( width, height);

            //window->setSize({ width, height });

            ProjectManager::SetStatus(project_status_t::IDLE);
        }
    }

    window->close();
    window = nullptr;
    ImGui::SFML::Shutdown();

    return 0;
}