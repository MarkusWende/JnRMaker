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

#include "Scene.h"
#include "Gui.h"
#include "imgui.h"

/**
 * @brief A function that is called every main loop, to handle keyboard input.
 */
void processEvents(std::shared_ptr<Scene> scene, std::shared_ptr<Gui> gui)
{
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
	{
        gui->Close();
	}
        
    ImGuiIO& io = ImGui::GetIO();

    if (scene->IsMouseOverScene())
    {
        auto x = io.MouseDelta.x;
        auto y = io.MouseDelta.y;
        auto wheelY = io.MouseWheel;
        auto deltaTime = io.DeltaTime;
        auto sceneWidthPercentage = scene->GetWidth() / ImGui::GetIO().DisplaySize.x;;
		auto sceneHeightPercentage = scene->GetHeight() / ImGui::GetIO().DisplaySize.y;
        scene->GetCamera("SceneCamera")->SetSceneWidth(sceneWidthPercentage);
        scene->GetCamera("SceneCamera")->SetSceneHeight(sceneHeightPercentage);

        scene->SetMousePosition(glm::vec2(io.MousePos.x, io.MousePos.y));
        
        // Mouse dragging
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	    {
            if (scene->GetCamera("SceneCamera")->GetState() == CameraState::PERSPECTIVE)
            {
                scene->GetCamera("SceneCamera")->ProcessMouseRotate(x, y, deltaTime);
            }
        }
        
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
        {
            if ((scene->GetCamera("SceneCamera")->GetState() == CameraState::ORTHOGRAPHIC) ||
                    (scene->GetCamera("SceneCamera")->GetState() == CameraState::PERSPECTIVE))
            {
                scene->GetCamera("SceneCamera")->ProcessMouseDrag(x, y, deltaTime);
            }
        }

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            if (scene->GetCamera("SceneCamera")->GetState() == CameraState::ORTHOGRAPHIC)
                {
                    scene->PlaceTile();
                }
        }

        // Mouse clicks, single
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	    {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) == 2)
            {
                if (scene->GetCamera("SceneCamera")->GetState() == CameraState::ORTHOGRAPHIC)
                {
                    scene->RemoveTile();
                }
            }
            else if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) == 1)
            {
                if (scene->GetCamera("SceneCamera")->GetState() == CameraState::ORTHOGRAPHIC)
                {
                    scene->PlaceTile();
                }
            }
	    }

        // Mouse wheel
        if (io.MouseWheel != 0.0f)
        {
            scene->GetCamera("SceneCamera")->ProcessMouseScroll(wheelY, deltaTime);
        }
    }
}

#endif	// EVENTS_H
