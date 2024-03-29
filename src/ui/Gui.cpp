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
 * @file guip.cpp
 * @brief This file contains the gui class implementation.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif // __EMSCRIPTEN__
#include <clip.h>
#include "Gui.h"
#include "IconsFontAwesome6.h"

#define DEVELOPMENT

void PasteToSystemClipboard(const char* text)
{
#ifdef __EMSCRIPTEN__
    EM_ASM_({
        const text = UTF8ToString($0);
        navigator.clipboard.writeText(text).catch(function(error) {
            console.error('Failed to write text to clipboard:', error);
        });
    }, text);
#else
	clip::set_text(text);
#endif
}

Gui::Gui(std::shared_ptr<ILogger> logger, std::shared_ptr<Resources> resources)
{
	ui_logger_ = std::dynamic_pointer_cast<UILogger>(logger);
	resources_ = resources;
	init();
	customGuiStyle();

	//active_tilemap_name_ = "resources/assets/tiles/game-tiles_cut.png";
	
	//tilemap_list_.push_back(active_tilemap_name_);
	//TilemapManager::AddTilemap(active_tilemap_name_, { 16, 16 }, { 1.0f, 1.0f }, active_tilemap_name_);
}

Gui::~Gui()
{

}

GLvoid Gui::WindowUpdate(std::shared_ptr<Scene> scene, GLuint width, GLuint height)
{
	if (glm::abs(width_ - width) > 2 || glm::abs(height_ - height) > 2)
	{
		width_ = width;
		height_ = height;

		WindowUpdate(scene);
	}
}

GLvoid Gui::WindowUpdate(std::shared_ptr<Scene> scene)
{
	window_scene_.w = (GLuint)(width_ * (glm::round(window_scene_.wPercent * 100.0f) / 100.0f));
	window_scene_.h = (GLuint)(height_ * (glm::round(window_scene_.hPercent * 100.0f) / 100.0f));

	window_messages_.w = (GLuint)(width_ * (glm::round(window_messages_.wPercent * 100.0f) / 100.0f));
	window_messages_.h = (GLuint)(height_ * (1.0f - (glm::round(window_scene_.hPercent * 100.0f) / 100.0f)));

	window_sidebar_right_.w = (GLuint)(width_ * (1.0f - (glm::round(window_scene_.wPercent * 100.0f) / 100.0f)));
	window_sidebar_right_.h = (GLuint)(height_ * (glm::round(window_scene_.hPercent * 100.0f) / 100.0f));

	scene->Update(window_scene_.w-5, window_scene_.h-25);
	scene->GetCamera("SceneCamera")->SetSceneWidth(window_scene_.wPercent);
	scene->GetCamera("SceneCamera")->SetSceneHeight(window_scene_.hPercent);
}

void Gui::Draw(std::shared_ptr<Scene> scene)
{
	DrawMenuMain(scene);
	DrawWindowView(scene);
	DrawWindowExplorer(scene);
	DrawWindowSettings(scene);
	DrawWindowStatusbar();
	DrawPopupMessages();

	if (show_demo_imgui_)
    {
        ImGui::ShowDemoWindow();
    }

    if (show_backend_checker_show_)
    {
        drawBackendCheckerWindow();
    }

	if (ImGui::BeginPopup("MyHelpMenu"))
	{
		ImGui::Selectable("Hello!");
		ImGui::EndPopup();
	}
}

GLvoid Gui::DrawMenuMain(std::shared_ptr<Scene> scene)
{
	(void)scene->IsMapNull();
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				ProjectManager::SetStatus(project_status_t::SAVE);
			}
			if (ImGui::MenuItem("Load"))
			{
				ProjectManager::SetStatus(project_status_t::LOAD);
			}
#ifndef __EMSCRIPTEN__
			if (ImGui::MenuItem("Quit"))
			{
				state_ = gui_state_t::GUI_CLOSE;
			}
#endif // !__EMSCRIPTEN__
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tiles"))
		{
			if (ImGui::MenuItem("Open"))
			{
				// Open new tilemap
				resources_->AddTilemap();
			}
            if(ImGui::MenuItem("Save"))
            {
#ifdef __EMSCRIPTEN__
                saveJSONFile();
#endif
            }
			ImGui::EndMenu();
		}
		#ifdef DEVELOPMENT
        if (ImGui::BeginMenu("Testing"))
        {
            if (ImGui::BeginMenu("Messages"))
            {
                if (ImGui::MenuItem("Test Info"))
                {
                    // Test messages
                    std::stringstream msg;
                    msg << "Thats a test info text." << std::endl;
					ui_logger_->Popup(log_t::INFO, "%s", msg.str().c_str());
                }

                if (ImGui::MenuItem("Test Warning"))
                {
                    // Test messages
                    std::stringstream msg;
                    msg << "Thats a test warning text." << std::endl;
					ui_logger_->Popup(log_t::WARNING, "%s", msg.str().c_str());
                }

                if (ImGui::MenuItem("Test Error"))
                {
                    // Test messages
                    std::stringstream msg;
                    msg << "Thats a test error text." << std::endl;
					ui_logger_->Popup(log_t::ERROR_T, "%s", msg.str().c_str());
                }

                if (ImGui::MenuItem("Test Debug"))
                {
                    // Test messages
                    std::stringstream msg;
                    msg << "Thats a test debug text." << std::endl;
					ui_logger_->Popup(log_t::DEBUG, "%s", msg.str().c_str());
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Demos"))
            {
                ImGui::Checkbox("ImGui", &show_demo_imgui_);
                ImGui::Checkbox("BackendChecker", &show_backend_checker_show_);
    
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Function"))
            {
                
            }

            ImGui::EndMenu();
        }
#endif

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Fullscreen"))
            {
#ifdef __EMSCRIPTEN__
                viewFullscreen();
#endif
            }

            ImGui::EndMenu();
        }

#ifdef _WIN32
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0));
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		auto marker_min = ImVec2(ImGui::GetWindowSize().x - static_cast<float>(main_menubar_height_), 0.0f);
		auto marker_max = ImVec2(ImGui::GetWindowSize().x, static_cast<float>(main_menubar_height_));
		//draw_list->AddRectFilled(marker_min, marker_max, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f)));
		ImGui::PushFont(icons_13_);
		auto bgColor = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_WindowBg));
		bgColor.w = 0.5f;
		auto hoveredItem = -1;

		ImGui::SetCursorPos(ImVec2(marker_min.x + 5.0f, 0.0f));
		static auto dummy = false;
		if (ImGui::Selectable(ICON_FA_XMARK, &dummy, ImGuiSelectableFlags_None, ImVec2(static_cast<float>(main_menubar_height_), static_cast<float>(main_menubar_height_))))
			state_ = gui_state_t::GUI_CLOSE;
		if (ImGui::IsItemHovered())
		{
			draw_list->AddRectFilled(marker_min, marker_max, ImGui::ColorConvertFloat4ToU32(bgColor));
			hoveredItem = 2;
		}

		// marker_min.x -= main_menubar_height_;
		// marker_max.x -= main_menubar_height_;
		// ImGui::SetCursorPos(ImVec2(marker_min.x + 5.0f, 0.0f));
		// if (ImGui::Selectable(ICON_FA_SQUARE, nullptr, ImGuiSelectableFlags_None, ImVec2(main_menubar_height_, main_menubar_height_)))
		// 	state_ = gui_state_t::GUI_MAXIMIZE;
		// if (ImGui::IsItemHovered() && (hoveredItem == -1))
		// {
		// 	draw_list->AddRectFilled(marker_min, marker_max, ImGui::ColorConvertFloat4ToU32(bgColor));
		// 	hoveredItem = 2;
		// }

		ImGui::PopFont();

		// marker_min.x -= main_menubar_height_;
		// marker_max.x -= main_menubar_height_;
		// ImGui::SetCursorPos(ImVec2(marker_min.x + 5.0f, 0.0f));
		// if (ImGui::Selectable("_", nullptr, ImGuiSelectableFlags_None, ImVec2(main_menubar_height_, main_menubar_height_)))
		// 	state_ = gui_state_t::GUI_MINIMIZE;
		// if (ImGui::IsItemHovered() && (hoveredItem == -1))
		// 	draw_list->AddRectFilled(marker_min, marker_max, ImGui::ColorConvertFloat4ToU32(bgColor));

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
#endif
		ImGui::EndMainMenuBar();
	}
}

GLvoid Gui::DrawWindowView(std::shared_ptr<Scene> scene)
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImGui::SetNextWindowSize(ImVec2(window_scene_.w + style->WindowBorderSize, window_scene_.h + style->WindowBorderSize));
	ImGui::SetNextWindowPos(ImVec2(0, (float)main_menubar_height_));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0));
	//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0));
	ImGui::Begin("View", NULL,	ImGuiWindowFlags_NoTitleBar |
									ImGuiWindowFlags_NoMove |
									ImGuiWindowFlags_NoScrollbar |
									ImGuiWindowFlags_NoScrollWithMouse |
									ImGuiWindowFlags_NoCollapse);

	if (ImGui::BeginTabBar("ViewTabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Scene"))
		{
			//if (!scene->IsMapNull())
			{
				GLfloat yOff = 47;
				GLfloat xOff = 3;

				/*
				sf::RenderTexture* tex = ResourceManagerOld::GetRenderTexture("viewport");
				ImGui::Image(tex->getTexture(),
					sf::Vector2f(scene->GetWidth(), scene->GetHeight()),
					sf::FloatRect(0, (float)scene->GetHeight(), (float)scene->GetWidth(), -(float)scene->GetHeight()),
					sf::Color(255, 255, 255, 255),
					sf::Color(0, 255, 0, 255));
					*/
				GLuint64 fbID = ResourceManagerOld::GetFramebuffer("scene").GetTextureID();
				ImGui::GetWindowDrawList()->AddImage((ImTextureID)fbID, ImVec2(xOff, yOff), ImVec2(scene->GetWidth() + xOff, scene->GetHeight() + yOff));
				//ImGui::GetWindowDrawList()->AddImage((ImTextureID)ResourceManagerOld::GetFramebuffer("imguiScene").GetTextureID(), ImVec2(0, 0), ImVec2(scene->GetWidth(), scene->GetHeight()));
				ImVec2 mousePos = ImGui::GetMousePos();
				if (mousePos.x > xOff && mousePos.x < (scene->GetWidth() + xOff))
				{
					if (mousePos.y > yOff && mousePos.y < (scene->GetHeight() + yOff))
					{
						scene->SetMouseOverScene(true);
					}
					else
					{
						scene->SetMouseOverScene(false);
					}
				}
				else
				{
					scene->SetMouseOverScene(false);
				}
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Memory"))
		{
			ImGui::SetCursorPos(ImVec2(window_scene_.w / 2.0f, 33.0f));
			ImGui::Text("Tiles used and which will be saved.");
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f , 0.2f , 0.2f, 1.0f));
			ImGui::BeginChildFrame(2, ImVec2((float)window_scene_.w, (float)window_scene_.h - 50.0f));
			{
				ImGui::Separator();
				double cummSize = 0.0;
				for (const auto& [key, value] : ResourceManagerOld::GetTextureMap())
				{
					GLuint64 texID = (GLuint64)value.ID;
					double size = (double)(value.Width * value.Height * 4) / 1024.0;
					cummSize += size;

					ImGui::Text("key: %s\tid: %llu\tsize: %0.2fkB", key.c_str(), texID, size);
					ImGui::Image((ImTextureID)texID,
						ImVec2((float)value.Width * 2.0f, (float)value.Height * 2.0f),
						ImVec2(0,0),
						ImVec2(1,1),
						ImColor(255, 255, 255, 255),
						ImColor(0, 255, 0, 255));
				}
				
				ImGui::Separator();
				ImGui::Text("size: %0.2fkB", cummSize);
			}
			ImGui::EndChildFrame();
			ImGui::PopStyleColor();

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	GLuint windowWidth = (GLuint)ImGui::GetWindowWidth() - 1;
	GLuint windowHeight = (GLuint)ImGui::GetWindowHeight() - 1;

	if ((windowWidth != window_scene_.w) || (windowHeight != window_scene_.h))
	{
		window_scene_.wPercent = (GLfloat)windowWidth / (GLfloat)width_;
		window_scene_.hPercent = (GLfloat)windowHeight / (GLfloat)height_;

		WindowUpdate(scene);
	}

	ImGui::End();
	//ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

GLvoid Gui::DrawWindowSettings(std::shared_ptr<Scene> scene)
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImGui::SetNextWindowSize(ImVec2(window_sidebar_right_.w + style->WindowBorderSize, window_sidebar_right_.h + style->WindowBorderSize - statusbar_height_ + 2.0f));
	ImGui::SetNextWindowPos(ImVec2((float)window_scene_.w, (float)main_menubar_height_));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 1));
	ImGui::Begin("SideBarRight", NULL, 	ImGuiWindowFlags_NoTitleBar |
										ImGuiWindowFlags_NoMove |
										ImGuiWindowFlags_NoCollapse);


	if (ImGui::BeginTabBar("SettingsTabs", ImGuiTabBarFlags_None))
	{
		DrawTabWorld(scene);
		DrawTabCamera(scene);
		DrawTabSprites();
		
		ImGui::EndTabBar();
	}


	// if (!scene->IsMapNull())
	// {
	// 	ImGui::SetCursorPos(ImVec2(15.f, 5.f));
	// 	glm::vec2 mapSize = glm::vec2((float)scene->GetMapWidth(), (float)scene->GetMapHeight());
	// 	mapSize = mapSize * scene->GetSpriteSize() * scene->GetSpriteScale();
	// 	//height = (int)((350.0f / (float)width) * (float)height);
	// 	ImGui::BeginChild("Minimap", ImVec2(400, 30));
	// 	ImGui::Text("Minimap");
	// 	ImGui::Text("width: %f -- height: %f", mapSize.x, mapSize.y);

	// 	//sf::RenderTexture* tex = ResourceManagerOld::GetRenderTexture("minimap");
	// 	ImGui::SetCursorPos(ImVec2(0.f, 18.f));
	// 	/*
	// 	ImGui::Image(tex->getTexture(),
	// 		sf::Vector2f(350, height),
	// 		sf::FloatRect(0, (float)scene->GetMapHeight(), (float)scene->GetMapWidth(), -(float)scene->GetMapHeight()),
	// 		sf::Color(255, 255, 255, 255),
	// 		sf::Color(0, 255, 0, 255));
	// 		*/
	// 	ImGui::EndChild();
	// }


	GLuint windowWidth = (GLuint)ImGui::GetWindowWidth() - 1;
	GLuint windowHeight = (GLuint)ImGui::GetWindowHeight() - 1;

	if ((windowWidth != window_sidebar_right_.w) || (windowHeight != window_sidebar_right_.h))
	{
		window_scene_.wPercent = (GLfloat)(width_ - windowWidth) / (GLfloat)width_;
		window_scene_.hPercent = (GLfloat)windowHeight / (GLfloat)height_;

		WindowUpdate(scene);
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void Gui::DrawTabWorld(std::shared_ptr<Scene> scene)
{
	if (ImGui::BeginTabItem("World"))
	{
		static int mapSize[2] = { 20, 20};
		static std::string spriteSizeStr = "16x16";
		std::vector<std::string> spriteSizeStrVector = { "16x16", "24x24", "32x32", "64x64" };
		if (ImGui::BeginCombo("##SpriteSize", spriteSizeStr.c_str()))
		{
			for (size_t n = 0; n < spriteSizeStrVector.size(); n++)
			{
				std::string item = spriteSizeStrVector.at(n);
				ImGui::PushID((int)n);
				if (ImGui::Selectable(spriteSizeStrVector.at(n).c_str(), item.compare(spriteSizeStr)))
				{
					spriteSizeStr = item;
				}

				ImGui::PopID();
			}
			ImGui::EndCombo();
		}

		ImGui::Separator();
		ImGui::Text("Map size");
		ImGui::SliderInt2("width x height", mapSize, 1, 255);
		ImGui::Separator();

		if (ImGui::Button("New"))
		{
			std::string delimiter = "x";
			size_t pos = 0;
			std::vector<std::string> tokens;
			std::string s = spriteSizeStr;
			while ((pos = s.find(delimiter)) != std::string::npos)
			{
				tokens.push_back(s.substr(0, pos));
				s.erase(0, pos + delimiter.length());
			}
			tokens.push_back(s);

			if (tokens.size() == 2)
			{
				int width = std::stoi(tokens.at(0));
				int height = std::stoi(tokens.at(1));

				if ((width == 16 || width == 24 || width == 32 || width == 64) && (width == height))
					scene->CreateLevel((mapSize[0]), (mapSize[1]), { width, height }, { 1.0f, 1.0f });
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Resize"))
		{
			scene->ResizeLevel(mapSize[0], mapSize[1]);
		}
		
		static int selected = 0;
		for (int n = 0; n < 3; n++) {
			char buf[32];
#ifdef _WIN32
			sprintf_s(buf, "Layer %d", n);
#else
			sprintf(buf, "Layer %d", n);
#endif
			if (ImGui::Selectable(buf, selected == n))
				selected = n;
		}

		if (selected == 0) {
			scene->SetActiveLayer(layer_t::BACK);
		}
		else if (selected == 1) {
			scene->SetActiveLayer(layer_t::PLAYER);
		}
		else if (selected == 2) {
			scene->SetActiveLayer(layer_t::FORE);
		}
		ImGui::EndTabItem();
	}
}

void Gui::DrawTabCamera(std::shared_ptr<Scene> scene)
{
	if (ImGui::BeginTabItem("Camera"))
	{
		ImGui::Text("Pos: (%.2f|%.2f|%.2f)", scene->GetCamera("SceneCamera")->GetPosition().x, scene->GetCamera("SceneCamera")->GetPosition().y, scene->GetCamera("SceneCamera")->GetPosition().z);
		ImGui::Text("Yaw: %.2f", scene->GetCamera("SceneCamera")->GetYaw());
		ImGui::Text("Pitch: %.2f", scene->GetCamera("SceneCamera")->GetPitch());
		ImGui::Text("Roll: %.2f", scene->GetCamera("SceneCamera")->GetRoll());
		ImGui::Text("Zoom: %.2f", scene->GetCamera("SceneCamera")->GetZoom());

		if (ImGui::Button("Reset")) {
			scene->GetCamera("SceneCamera")->Reset();
		}

		// auto cState = scene->GetCamera("SceneCamera")->GetState();
		if (ImGui::Button("Persp")) {
			scene->GetCamera("SceneCamera")->SetState(CameraState::PERSPECTIVE);
			// cState = CameraState::PERSPECTIVE;
		}
		if (ImGui::Button("Ortho")) {
			scene->GetCamera("SceneCamera")->SetState(CameraState::ORTHOGRAPHIC);
			// cState = CameraState::ORTHOGRAPHIC;
		}
		ImGui::EndTabItem();
	}
}

void Gui::DrawTabSprites()
{
	if (ImGui::BeginTabItem("Sprites"))
	{
		
		ImGui::EndTabItem();
	}
}

void Gui::DrawWindowExplorer(std::shared_ptr<Scene> scene)
{
	ImGui::SetNextWindowSize(ImVec2((float)window_messages_.w, (float)window_messages_.h - statusbar_height_));
	ImGui::SetNextWindowPos(ImVec2(0.0f, (float)window_scene_.h + (float)main_menubar_height_));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 5.0f));

	ImGui::Begin("BottomBar", NULL,	ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoDocking);

	if (ImGui::BeginTabBar("ExplorerTabs", ImGuiTabBarFlags_None))
	{
		DrawTabMessages();
		DrawTabTileExplorer(scene);
		
		ImGui::EndTabBar();
	}

	GLuint windowHeight = (GLuint)ImGui::GetWindowHeight();

	if (windowHeight != window_messages_.h)
	{
		window_scene_.hPercent = (GLfloat)(height_ - windowHeight) / (GLfloat)height_;

		WindowUpdate(scene);
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void Gui::DrawTabMessages()
{
    if (ImGui::BeginTabItem("Messages"))
	{
		auto io = ImGui::GetIO();
		auto wrap_width = io.DisplaySize.x - 20.0f;
		//ImGui::Text("(%.2f FPS)", ImGui::GetIO().Framerate); ImGui::SameLine();
		//ImGui::SetCursorPos(ImVec2(0.f, 18.f));
		//ImGui::Separator();

		// BeginChild: MessageList
		ImGui::SetCursorPos(ImVec2(5.f, 30.f));
		ImGui::BeginChild("##MessageList", ImVec2(0, (float)window_messages_.h - 60.0f - statusbar_height_), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (ui_logger_)
		{	
    		auto logs = ui_logger_->GetLogs();
			if (logs->size() > 0)
			{
				ImGuiListClipper clipper;
				clipper.Begin((int)logs->size());
				while (clipper.Step())
				{
					for (auto it = clipper.DisplayStart; it != clipper.DisplayEnd; it++)
					{
	#ifndef DEVELOPMENT
						if (logs->at(it).type == log_t::DEBUG)
							continue;
	#endif 
						ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
						ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", logs->at(it).timeinfo.c_str());
						ImGui::SameLine(0, 2);
						ImGui::TextColored(ImVec4(1, 1, 1, 1), ":\t");

						auto msgSymbolColor = ImVec4(0.42f, 0.85f, 1.0f, 1.0f);
						auto message = logs->at(it).msg.c_str();
						std::stringstream ssMsg;

						ImGui::SameLine(0, 5);
						switch (logs->at(it).type)
						{
						case log_t::ERROR_T:
							msgSymbolColor = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
							ssMsg << "[ Error ]  ";
							break;
						case log_t::INFO:
							msgSymbolColor = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
							ssMsg << "[ Info  ]  ";
							break;
						case log_t::WARNING:
							msgSymbolColor = ImVec4(0.92f, 0.56f, 0.9f, 1.0f);
							ssMsg << "[Warning]  ";
							break;
						case log_t::DEBUG:
							ssMsg << "[ Debug ]  ";
							break;
						case log_t::DEBUG_WS:
							ssMsg << "[ Debug ]  ";
							break;

						default:
							msgSymbolColor = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
							ImGui::TextColored(msgSymbolColor, "[ Info  ]  %s", logs->at(it).msg.c_str());
							break;
						}

						ssMsg << message;
						ImGui::PushStyleColor(ImGuiCol_Text, msgSymbolColor);
						if (ImGui::Selectable(ssMsg.str().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
							PasteToSystemClipboard(ssMsg.str().c_str());
							ui_logger_->LogStatus("Text copied. Use Ctrl+V to paste.");
						}
						ImGui::PopStyleColor();
					}
				}
			}
		}

		// EndChild: MessageList
		ImGui::EndChild();
		ImGui::SetScrollHereY(1.0f);
		ImGui::EndTabItem();
	}
}

void Gui::DrawWindowStatusbar()
{
	ImGuiIO& io = ImGui::GetIO();
    float screenHeight = io.DisplaySize.y;
    float windowWidth = io.DisplaySize.x;

	ImGui::SetNextWindowPos(ImVec2(0, screenHeight - 30.0f));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, 30.0f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));

	ImGui::Begin("Statusbar", NULL,	ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoDocking  |
									ImGuiWindowFlags_NoResize   |
									ImGuiWindowFlags_NoScrollbar);

	ImGui::SetCursorPos(ImVec2(5.f, 8.f));
	ImGui::BeginChild("##StatusMessage", ImVec2(windowWidth/3.0f, 0), false);

	static float startTime = 0.0f;
	float fadeDuration = 2.0f;  // Fade duration in seconds

	if (ui_logger_->IsNewStatus())
	{
		startTime = ImGui::GetTime();
	}

	float currentTime = ImGui::GetTime();
	float elapsed = currentTime - startTime;
	float fadeAlpha = 1.0f - (elapsed / fadeDuration);
	if (fadeAlpha < 0.0f)
		fadeAlpha = 0.0f;
	else if (fadeAlpha > 1.0f)
		fadeAlpha = 1.0f;

	if (ui_logger_)
	{	
		auto logs = ui_logger_->GetStatusLogs();
		if (logs->begin() != logs->end())
		{
			auto message = logs->front().msg.c_str();
			ImVec2 textSize = ImGui::CalcTextSize(message);
			ImVec2 startPos = ImGui::GetCursorScreenPos();
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(ImVec2(startPos.x, startPos.y), ImVec2(startPos.x + textSize.x + 6.0f, startPos.y + textSize.y + 4.0f), ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, fadeAlpha)), 2.0f);
			ImGui::SetCursorPos(ImVec2(3.f, 2.f));
			ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_WindowBg)), "%s", message);
		}
	}

	// EndChild: MessageList
	ImGui::EndChild();
	ImGui::SetCursorPos(ImVec2(windowWidth - 70.f, 8.f));
	ImGui::BeginChild("##FPS", ImVec2(70, 20.f), false);

	ImGui::Text("%.1f FPS", io.Framerate);

	// EndChild: MessageList
	ImGui::EndChild();

	ImGui::End();
	ImGui::PopStyleVar();
}

void Gui::DrawPopupMessages()
{
	auto logs = ui_logger_->GetLogs();
	if (logs->size() > 0)
	{
        for (size_t it = 0; it < logs->size(); it++)
        {
#ifndef DEVELOPMENT
            if (logs->at(it).type == message_t::DEBUG)
                continue;
#endif 
            //std::string word;
            //std::stringstream ss(logs->at(it).msg);
            //char msgChr[250] = "";
            char msgSymbol[250] = ICON_FA_BUG;
            std::stringstream popupID;
            popupID << "##" << logs->at(it).timeinfo.c_str() << ":" << logs->at(it).msg.c_str();
            ImVec4 msgSymbolColor = ImVec4(0.42f, 0.85f, 1.0f, 1.0f);

            ImGui::SameLine(0, 5);
            if (logs->at(it).type == log_t::ERROR_T)
            {
                msgSymbolColor = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
#if defined(_WIN32)
                strcpy_s(msgSymbol, ICON_FA_XMARK);
#else
                std::strcpy(msgSymbol, ICON_FA_XMARK);
#endif
            }
            else if (logs->at(it).type == log_t::INFO)
            {
                msgSymbolColor = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
#if defined(_WIN32)
                strcpy_s(msgSymbol, ICON_FA_CIRCLE_EXCLAMATION);
#else
                std::strcpy(msgSymbol, ICON_FA_CIRCLE_EXCLAMATION);
#endif
            }
            else if (logs->at(it).type == log_t::WARNING)
            {
                msgSymbolColor = ImVec4(0.92f, 0.56f, 0.9f, 1.0f);
#if defined(_WIN32)
                strcpy_s(msgSymbol, ICON_FA_TRIANGLE_EXCLAMATION);
#else
                std::strcpy(msgSymbol, ICON_FA_TRIANGLE_EXCLAMATION);
#endif
            }

            static int numPopups = 0;
            if (logs->at(it).popup && !numPopups)
            {
                ImGui::OpenPopup(popupID.str().c_str());
                numPopups++;
                logs->at(it).popup = false;
            }

            // Always center this window when appearing
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));


            if (ImGui::BeginPopupModal(popupID.str().c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::PushFont(icons_40_);
                ImGui::TextColored(msgSymbolColor, "%s", msgSymbol); ImGui::SameLine();
                ImGui::PopFont();
                ImVec2 popupPos = ImGui::GetCursorPos();
                ImGui::SetCursorPos(ImVec2(popupPos.x + 10.0f, popupPos.y + 15.0f));
                ImGui::Text("%s\n", logs->at(it).msg.c_str());
                ImGui::Text("  ");

                ImGui::Separator();
                float buttomPos = ImGui::GetCursorPos().x + (ImGui::GetWindowSize().x / 2.0f - 70.0f);
                ImGui::SetCursorPosX(buttomPos);
                if (ImGui::Button("OK", ImVec2(120.0f, 0.0f)))
                { 
                    numPopups--;
                    logs->at(it).popup = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
        }
	}
}

void Gui::DrawTabTileExplorer(std::shared_ptr<Scene> scene)
{
	std::shared_ptr<std::vector<std::string>> tileMenagerKeysPtr = resources_->GetTileManagerKeys();

	// Check if the shared pointer is valid
	if (tileMenagerKeysPtr)
	{
		if (ImGui::BeginTabItem("Tiles"))
		{
			static ImGuiTableFlags flags1 = 
				ImGuiTableFlags_Resizable |
				ImGuiTableFlags_Reorderable |
				ImGuiTableFlags_Hideable |
				ImGuiTableFlags_Borders |
				ImGuiTableFlags_ContextMenuInBody;

			if (ImGui::BeginTable("table_context_menu", 2, flags1))
			{
				const float TEXT_BASE_WIDT = ImGui::CalcTextSize("A").x;
				ImGui::TableSetupColumn("Tilemaps", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDT * 25.0f);
				ImGui::TableSetupColumn("Tiles");

				// [1.1]] Right-click on the TableHeadersRow() line to open the default table context menu.
				ImGui::TableHeadersRow();

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);

				static int selected = 0;
				int index = 0;
				// Dereference the shared pointer to access the underlying vector
    			auto tileMenagerKeys = *tileMenagerKeysPtr;
				for (const auto& key : tileMenagerKeys)
				{
					if(key != "")
					{
						ImGui::PushID(key.c_str());
						char label[128];
	#ifdef _WIN32
						sprintf_s(label, "%s", key.c_str());
	#else
						sprintf(label, "%s", key.c_str());
	#endif
						if (ImGui::Selectable(label, selected == index))
						{
							resources_->SetActiveTilemap(key.c_str());
							selected = index;
						}
						ImGui::PopID();
					}
					index++;
				}

				ImGui::TableSetColumnIndex(1);
				static glm::vec2 tileButtonScale = glm::vec2(2.0f, 2.0f);
				auto tilemap = resources_->GetActiveTilemap();
				std::vector<std::string> tilemapHashes = tilemap->GetHashs();

				ImGui::BeginChild("##TileSelector", ImVec2(0,(float)window_messages_.h - 100.0f - statusbar_height_), true, ImGuiWindowFlags_HorizontalScrollbar);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

				GLuint i = 0;
				ImGuiListClipper clipper;
				clipper.Begin(tilemap->NumRows());

				while (clipper.Step())
				{
					for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
					{
						for (GLuint col = 0; col < tilemap->NumCols(); col++)
						{
							//std::stringstream sprKey;
							//sprKey << "r" << row << "c" << col;
							GLuint64 tile = (GLuint64)tilemap->GetTile(tilemapHashes.at(i)).ID;
							auto buttonWidth = tilemap->GetSpriteSize().x * tilemap->GetSpriteScale().x * tileButtonScale.x;
							auto buttonHeight = tilemap->GetSpriteSize().y * tilemap->GetSpriteScale().y * tileButtonScale.y;
							ImGui::PushID(i);
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.4f));
							// Change mouse cursor to hand
							if (ImGui::IsItemHovered() || ImGui::IsItemFocused())
							{
								ImGui::SetMouseCursor(7);
							}
							if (
								ImGui::ImageButton(
									(ImTextureID)tile,
									ImVec2(buttonWidth, buttonHeight),
									ImVec2(0.0f, 0.0f),
									ImVec2(1.0f, 1.0f),
									1,
									ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
									ImVec4(0.8f, 0.8f, 0.8f, 1.0f))
								)
							{
								if (!scene->IsMapNull())
								{
									scene->SetActiveSprite(tilemapHashes.at(i));
									GLuint brushTexID = tilemap->GetTile(tilemapHashes.at(i)).ID;
									scene->GetSprite("brush")->AssignTextureID(brushTexID);

									/* std::stringstream msg;
									msg << tilemapHashes.at(i);
									MessageManager::AddMessage(msg, message_t::INFO); */
								}
							}

							ImGui::PopStyleColor(2);
							ImGui::PopID();
							if (col < (tilemap->NumCols() - 1))
								ImGui::SameLine();
							i++;
						}
					}
				}

				ImGui::PopStyleVar();
				ImGui::EndChild();
				ImGui::EndTable();
			}
			
			ImGui::EndTabItem();
		}
	}
}

void Gui::ShowBackendCheckerWindow()
{
    ImGui::Begin("Dear ImGui Backend Checker");

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Dear ImGui %s Backend Checker", ImGui::GetVersion());
    ImGui::Text("io.BackendPlatformName: %s", io.BackendPlatformName ? io.BackendPlatformName : "NULL");
    ImGui::Text("io.BackendRendererName: %s", io.BackendRendererName ? io.BackendRendererName : "NULL");
    ImGui::Separator();
    
    if (ImGui::TreeNode("0001: Renderer: Large Mesh Support"))
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        {
            static int vtx_count = 60000;
            ImGui::SliderInt("VtxCount##1", &vtx_count, 0, 100000);
            ImVec2 p = ImGui::GetCursorScreenPos();
            for (int n = 0; n < vtx_count / 4; n++)
            {
                float off_x = (float)(n % 100) * 3.0f;
                float off_y = (float)(n % 100) * 1.0f;
                ImU32 col = IM_COL32(((n * 17) & 255), ((n * 59) & 255), ((n * 83) & 255), 255);
                draw_list->AddRectFilled(ImVec2(p.x + off_x, p.y + off_y), ImVec2(p.x + off_x + 50, p.y + off_y + 50), col);
            }
            ImGui::Dummy(ImVec2(300 + 50, 100 + 50));
            ImGui::Text("VtxBuffer.Size = %d", draw_list->VtxBuffer.Size);
        }
        {
            static int vtx_count = 60000;
            ImGui::SliderInt("VtxCount##2", &vtx_count, 0, 100000);
            ImVec2 p = ImGui::GetCursorScreenPos();
            for (int n = 0; n < vtx_count / (10*4); n++)
            {
                float off_x = (float)(n % 100) * 3.0f;
                float off_y = (float)(n % 100) * 1.0f;
                ImU32 col = IM_COL32(((n * 17) & 255), ((n * 59) & 255), ((n * 83) & 255), 255);
                draw_list->AddText(ImVec2(p.x + off_x, p.y + off_y), col, "ABCDEFGHIJ");
            }
            ImGui::Dummy(ImVec2(300 + 50, 100 + 20));
            ImGui::Text("VtxBuffer.Size = %d", draw_list->VtxBuffer.Size);
        }
        ImGui::TreePop();
    }

    ImGui::End();
}

// PRIVATE
GLvoid Gui::init()
{
	ui_logger_->Log(log_t::DEBUG, "Initializing Gui");
	window_scene_.wPercent = 0.85f;
	window_scene_.hPercent = 0.75f;

	window_messages_.wPercent = 1.0f;
	window_messages_.hPercent = 1.0f - window_scene_.hPercent;

	window_sidebar_right_.wPercent = 1.0f - window_scene_.wPercent;
	window_sidebar_right_.hPercent = window_scene_.hPercent;

	main_menubar_height_ = 23;
	statusbar_height_ = 50.0f;
	ImGuiStyle& style = ImGui::GetStyle();
	style.DisplaySafeAreaPadding = ImVec2(0.0f, 0.0f);

	state_ = gui_state_t::GUI_ACTIVE;

	show_demo_imgui_ = false;
    show_backend_checker_show_ = false;
}

void Gui::drawBackendCheckerWindow()
{
    ImGui::Begin("Dear ImGui Backend Checker");

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Dear ImGui %s Backend Checker", ImGui::GetVersion());
    ImGui::Text("io.BackendPlatformName: %s", io.BackendPlatformName ? io.BackendPlatformName : "NULL");
    ImGui::Text("io.BackendRendererName: %s", io.BackendRendererName ? io.BackendRendererName : "NULL");
    ImGui::Separator();
    
    if (ImGui::TreeNode("0001: Renderer: Large Mesh Support"))
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        {
            static int vtx_count = 60000;
            ImGui::SliderInt("VtxCount##1", &vtx_count, 0, 100000);
            ImVec2 p = ImGui::GetCursorScreenPos();
            for (int n = 0; n < vtx_count / 4; n++)
            {
                float off_x = (float)(n % 100) * 3.0f;
                float off_y = (float)(n % 100) * 1.0f;
                ImU32 col = IM_COL32(((n * 17) & 255), ((n * 59) & 255), ((n * 83) & 255), 255);
                draw_list->AddRectFilled(ImVec2(p.x + off_x, p.y + off_y), ImVec2(p.x + off_x + 50, p.y + off_y + 50), col);
            }
            ImGui::Dummy(ImVec2(300 + 50, 100 + 50));
            ImGui::Text("VtxBuffer.Size = %d", draw_list->VtxBuffer.Size);
        }
        {
            static int vtx_count = 60000;
            ImGui::SliderInt("VtxCount##2", &vtx_count, 0, 100000);
            ImVec2 p = ImGui::GetCursorScreenPos();
            for (int n = 0; n < vtx_count / (10*4); n++)
            {
                float off_x = (float)(n % 100) * 3.0f;
                float off_y = (float)(n % 100) * 1.0f;
                ImU32 col = IM_COL32(((n * 17) & 255), ((n * 59) & 255), ((n * 83) & 255), 255);
                draw_list->AddText(ImVec2(p.x + off_x, p.y + off_y), col, "ABCDEFGHIJ");
            }
            ImGui::Dummy(ImVec2(300 + 50, 100 + 20));
            ImGui::Text("VtxBuffer.Size = %d", draw_list->VtxBuffer.Size);
        }
        ImGui::TreePop();
    }

    ImGui::End();
}

GLvoid Gui::customGuiStyle()
{
	// Custom style
	// Font
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash
	font_default_ = io.Fonts->AddFontFromFileTTF("resources/fonts/Roboto-Medium.ttf", 13.0f);
    ImFontConfig config;
    //config.MergeMode = true;
    //config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    //io.Fonts->AddFontFromFileTTF("resources/fonts/fa-brands-400.ttf", 14.0f, &config, icon_ranges);
    //io.Fonts->AddFontFromFileTTF("resources/fonts/fa-regular-400.ttf", 14.0f, &config, icon_ranges);
    //io.Fonts->AddFontFromFileTTF("resources/fonts/fa-regular-400.ttf", 13.0f, &config, icon_ranges);
    //io.Fonts->AddFontFromFileTTF("resources/fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);
    icons_13_ = io.Fonts->AddFontFromFileTTF("resources/fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);
    icons_40_ = io.Fonts->AddFontFromFileTTF("resources/fonts/fa-solid-900.ttf", 40.0f, &config, icon_ranges);

	// Gui shape
	ImGuiStyle* style = &ImGui::GetStyle();
	style->Alpha = 1.0f;								// Global alpha applies to everything in ImGui.
	style->WindowPadding = ImVec2(15, 15);				    // Padding within a window.
	style->WindowRounding = 0.0f;						        // Radius of window corners rounding. Set to 0.0f to have rectangular windows.
	style->WindowBorderSize = 1.0f;						        // Thickness of border around windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
	style->WindowMinSize = ImVec2(20, 20);				    // Minimum window size. This is a global setting. If you want to constraint individual windows, use SetNextWindowSizeConstraints().
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);               // Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered.
	style->ChildRounding = 0.0f;						        // Radius of child window corners rounding. Set to 0.0f to have rectangular windows.
	style->ChildBorderSize = 0.2f;						        // Thickness of border around child windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
	style->PopupRounding = 0.0f;						        // Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)
	style->PopupBorderSize = 0.0f;						        // Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
	style->FramePadding = ImVec2(5, 5);					    // Padding within a framed rectangle (used by most widgets).
	style->FrameRounding = 0.0f;						        // Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).
	style->FrameBorderSize = 0.0f;						        // Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
	style->ItemSpacing = ImVec2(5, 4);						// Horizontal and vertical spacing between widgets/lines.
	style->ItemInnerSpacing = ImVec2(8, 6);				        // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).
	style->TouchExtraPadding = ImVec2(5, 5);				        // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
	style->IndentSpacing = 25.0f;						    // Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
	style->ColumnsMinSpacing = 0.0f;					            // Minimum horizontal spacing between two columns.
	style->ScrollbarSize = 10.0f;						    // Width of the vertical scrollbar, Height of the horizontal scrollbar.
	style->ScrollbarRounding = 0.0f;					            // Radius of grab corners for scrollbar.
	style->GrabMinSize = 5.0f;							    // Minimum width/height of a grab box for slider/scrollbar.
	style->GrabRounding = 0.0f;							    // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
	style->TabRounding = 0.0f;							    // Radius of upper corners of a tab. Set to 0.0f to have rectangular tabs.
	style->TabBorderSize = 0.0f;						        // Thickness of border around tabs.
	style->ButtonTextAlign = ImVec2(0.5f, 0.5f);               // Alignment of button text when button is larger than text. Defaults to (0.5f,0.5f) for horizontally+vertically centered.
	//style->DisplayWindowPadding   = ImVec2(0, 0);						// Window position are clamped to be visible within the display area by at least this amount. Only applies to regular windows.
	//style->DisplaySafeAreaPadding = ImVec2(0, 0);						// If you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly!
	//style->MouseCursorScale       = 0.0f;								// Scale software rendered mouse cursor (when io.MouseDrawCursor is enabled). May be removed later.
	style->AntiAliasedLines = false;					        // Enable anti-aliasing on lines/borders. Disable if you are really tight on CPU/GPU.
	style->AntiAliasedFill = false;						    // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
	style->CurveTessellationTol = 5.0f;					            // Tessellation tolerance when using PathBezierCurveTo() without a specific number of segments. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.

	// Colors
	style->Colors[ImGuiCol_Text] = ImVec4(0.8f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
	//style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.3f, 0.3f, 0.3f, 0.5f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_ComboBg]            = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.44f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	//style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_CloseButton]        = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	//style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	//style->Colors[ImGuiCol_CloseButtonActive]  = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	//style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
	style->Colors[ImGuiCol_Tab] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
	style->Colors[ImGuiCol_TabHovered] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
	style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
}