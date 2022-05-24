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


#include "gui.h"
#include "IconsFontAwesome6.h"

#define DEVELOPMENT

#ifdef __EMSCRIPTEN__
template<typename T, size_t sizeOfArray>
constexpr size_t getElementCount(T (&)[sizeOfArray]) {
    return sizeOfArray;
}

void openTilemapFile(std::string const& name, std::string const& type, emscripten::val data, int dataSize)
{
    std::stringstream msg;
    //for (auto & s : myString)
    auto image = data.as<std::string>();
    const std::vector<unsigned char> rawArray = emscripten::vecFromJSArray<unsigned char>(data);
    const unsigned char* rawData = rawArray.data();

    if ((type.compare("image/jpeg") == 0) || type.compare("image/png") == 0)
    {
        //ResourceManager::CreateTexture(rawData, dataSize, "loadedImage");
		TilemapManager::AddTilemap(name, { 16, 16 }, { 1.0f, 1.0f }, rawData, dataSize);
    }
    else
    {
        msg << name << " is not a supported format.";
        MessageManager::AddMessage(msg, message_t::ERROR_T);
        getLocalTilemapFile();
        return;
    }
    
    msg << "Loaded: " << name << "\tType: " << type << "\tSize: " << ((float)dataSize/1024.0f) << "KB\n";
    MessageManager::AddMessage(msg, message_t::DEBUG);
}

emscripten::val getBytes()
{
    auto text = R"(
    {
        "happy": true,
        "pi": 3.141
    }
    )"_json;
    const auto& tmp = text.dump(2);
    std::basic_string<unsigned char> str(tmp.data(), std::next(tmp.data(), tmp.size()));
    return emscripten::val(emscripten::typed_memory_view(str.size(), str.data()));
}

EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::function("getBytes", &getBytes);
    emscripten::function("openTilemapFile", &openTilemapFile);
}
#endif

Gui::Gui()
{
	init();
	customGuiStyle();

	//active_tilemap_name_ = "data/assets/tiles/game-tiles_cut.png";
	
	//tilemap_list_.push_back(active_tilemap_name_);
	//TilemapManager::AddTilemap(active_tilemap_name_, { 16, 16 }, { 1.0f, 1.0f }, active_tilemap_name_);
}

Gui::~Gui()
{

}

GLvoid Gui::WindowUpdate(Scene *scene, GLuint width, GLuint height)
{
	if (glm::abs(width_ - width) > 2 || glm::abs(height_ - height) > 2)
	{
		width_ = width;
		height_ = height;

		WindowUpdate(scene);
	}
}

GLvoid Gui::WindowUpdate(Scene *scene)
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

void Gui::Draw(Scene *scene)
{
	if (file_browser_add_tiles_)
	{
#ifdef __EMSCRIPTEN__
    	fileBrowserAddTile();
#else
    	fileBrowserAddTile(scene, root_file_path_, true, ".png");
#endif
	}

	DrawMenuMain(scene);
	DrawWindowView(scene);
	DrawWindowExplorer(scene);
	DrawWindowSettings(scene);

	if (show_demo_imgui_)
    {
        ImGui::ShowDemoWindow();
    }

    if (show_backend_checker_show_)
    {
        drawBackendCheckerWindow();
    }
}

GLvoid Gui::DrawMenuMain(Scene *scene)
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
			if (ImGui::MenuItem("Quit"))
			{
				state_ = gui_state_t::GUI_CLOSE;
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tiles"))
		{
			if (ImGui::MenuItem("Open"))
			{
				// Open new tile
#ifdef __EMSCRIPTEN__
                getLocalTilemapFile();
#else
				file_browser_add_tiles_ = true;
#endif
			}
            if(ImGui::MenuItem("Save"))
            {
#ifdef __EMSCRIPTEN__
                saveLocalFile();
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
                    MessageManager::AddMessage(msg, message_t::INFO);
                }

                if (ImGui::MenuItem("Test Warning"))
                {
                    // Test messages
                    std::stringstream msg;
                    msg << "Thats a test warning text." << std::endl;
                    MessageManager::AddMessage(msg, message_t::WARNING);
                }

                if (ImGui::MenuItem("Test Error"))
                {
                    // Test messages
                    std::stringstream msg;
                    msg << "Thats a test error text." << std::endl;
                    MessageManager::AddMessage(msg, message_t::ERROR_T);
                }

                if (ImGui::MenuItem("Test Debug"))
                {
                    // Test messages
                    std::stringstream msg;
                    msg << "Thats a test debug text." << std::endl;
                    MessageManager::AddMessage(msg, message_t::DEBUG);
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
                viewFullscreen();
            }

            ImGui::EndMenu();
        }

		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetCursorPosX(io.DisplaySize.x - 50.0f);
		ImGui::Text("%.1f FPS", io.Framerate);

		ImGui::EndMainMenuBar();
	}
}

GLvoid Gui::DrawWindowView(Scene *scene)
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
				GLfloat yOff = 45;
				GLfloat xOff = 3;

				/*
				sf::RenderTexture* tex = ResourceManager::GetRenderTexture("viewport");
				ImGui::Image(tex->getTexture(),
					sf::Vector2f(scene->GetWidth(), scene->GetHeight()),
					sf::FloatRect(0, (float)scene->GetHeight(), (float)scene->GetWidth(), -(float)scene->GetHeight()),
					sf::Color(255, 255, 255, 255),
					sf::Color(0, 255, 0, 255));
					*/
				GLuint64 fbID = ResourceManager::GetFramebuffer("scene").GetTextureID();
				ImGui::GetWindowDrawList()->AddImage((ImTextureID)fbID, ImVec2(xOff, yOff), ImVec2(scene->GetWidth() + xOff, scene->GetHeight() + yOff));
				//ImGui::GetWindowDrawList()->AddImage((ImTextureID)ResourceManager::GetFramebuffer("imguiScene").GetTextureID(), ImVec2(0, 0), ImVec2(scene->GetWidth(), scene->GetHeight()));
				ImVec2 mousePos = ImGui::GetMousePos();
				if ((mousePos.x > xOff && mousePos.x < (scene->GetWidth() + xOff)) && !file_browser_add_tiles_)
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

				for (const auto& [key, value] : ResourceManager::GetTextureMap())
				{
					GLuint64 texID = (GLuint64)value.ID;
					ImGui::Text("key: %s\tid: %llu", key.c_str(), texID);
					ImGui::Image((ImTextureID)texID,
						ImVec2((float)value.Width * 2.0f, (float)value.Height * 2.0f),
						ImVec2(0,0),
						ImVec2(1,1),
						ImColor(255, 255, 255, 255),
						ImColor(0, 255, 0, 255));
				}
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

GLvoid Gui::DrawWindowSettings(Scene *scene)
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImGui::SetNextWindowSize(ImVec2(window_sidebar_right_.w + style->WindowBorderSize, window_sidebar_right_.h + style->WindowBorderSize));
	ImGui::SetNextWindowPos(ImVec2((float)window_scene_.w, (float)main_menubar_height_));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 1));
	ImGui::Begin("SideBarRight", NULL, 	ImGuiWindowFlags_NoTitleBar |
										ImGuiWindowFlags_NoMove |
										ImGuiWindowFlags_NoCollapse);

	if (!scene->IsMapNull())
	{
		ImGui::SetCursorPos(ImVec2(15.f, 5.f));
		glm::vec2 mapSize = glm::vec2((float)scene->GetMapWidth(), (float)scene->GetMapHeight());
		mapSize = mapSize * scene->GetSpriteSize() * scene->GetSpriteScale();
		//height = (int)((350.0f / (float)width) * (float)height);
		ImGui::BeginChild("Minimap", ImVec2(400, 30));
		ImGui::Text("Minimap");
		ImGui::Text("width: %f -- height: %f", mapSize.x, mapSize.y);

		//sf::RenderTexture* tex = ResourceManager::GetRenderTexture("minimap");
		ImGui::SetCursorPos(ImVec2(0.f, 18.f));
		/*
		ImGui::Image(tex->getTexture(),
			sf::Vector2f(350, height),
			sf::FloatRect(0, (float)scene->GetMapHeight(), (float)scene->GetMapWidth(), -(float)scene->GetMapHeight()),
			sf::Color(255, 255, 255, 255),
			sf::Color(0, 255, 0, 255));
			*/
		ImGui::EndChild();
	}

	if (ImGui::CollapsingHeader("Map"))
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

		if (ImGui::Button("Create")) {
			std::string delimiter = "x";
			size_t pos = 0;
			std::vector<std::string> tokens;
			std::string s = spriteSizeStr;
			while ((pos = s.find(delimiter)) != std::string::npos) {
				tokens.push_back(s.substr(0, pos));
				s.erase(0, pos + delimiter.length());
			}
			tokens.push_back(s);

			if (tokens.size() == 2) {
				int width = std::stoi(tokens.at(0));
				int height = std::stoi(tokens.at(1));

				if ((width == 16 || width == 24 || width == 32 || width == 64) && (width == height)) {
					scene->CreateMap(mapSize[0], mapSize[1], { width, height }, { 1.0f, 1.0f });
				}
			}
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
	}

	if (ImGui::CollapsingHeader("Camera"))
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
	}

	if (ImGui::CollapsingHeader("Sprites"))
	{
		// Texture2D tex = ResourceManager::GetTexture("testing123");
		// if (
		// 	ImGui::ImageButton(
		// 		(ImTextureID)tex.ID,
		// 		ImVec2(64, 16),
		// 		ImVec2(0, 0),
		// 		ImVec2(1, 1),
		// 		1,
		// 		ImVec4(0, 0, 0, 0),
		// 		ImVec4(0.8, 0.8, 0.8, 1))
		// 	)
		// {
			
		// }
	}

	if (ImGui::CollapsingHeader("Inputs, Navigation & Focus"))
    {
        ImGuiIO& io = ImGui::GetIO();

        // Display ImGuiIO output flags
        ImGui::Text("WantCaptureMouse: %d", io.WantCaptureMouse);
        ImGui::Text("WantCaptureMouseUnlessPopupClose: %d", io.WantCaptureMouseUnlessPopupClose);
        ImGui::Text("WantCaptureKeyboard: %d", io.WantCaptureKeyboard);
        ImGui::Text("WantTextInput: %d", io.WantTextInput);
        ImGui::Text("WantSetMousePos: %d", io.WantSetMousePos);
        ImGui::Text("NavActive: %d, NavVisible: %d", io.NavActive, io.NavVisible);

        // Display Mouse state
        //IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Mouse State");
        if (ImGui::TreeNode("Mouse State"))
        {
            if (ImGui::IsMousePosValid())
                ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
            else
                ImGui::Text("Mouse pos: <INVALID>");
            ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);

            int count = IM_ARRAYSIZE(io.MouseDown);
            ImGui::Text("Mouse down:");         for (int i = 0; i < count; i++) if (ImGui::IsMouseDown(i))      { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
            ImGui::Text("Mouse clicked:");      for (int i = 0; i < count; i++) if (ImGui::IsMouseClicked(i))   { ImGui::SameLine(); ImGui::Text("b%d (%d)", i, ImGui::GetMouseClickedCount(i)); }
            ImGui::Text("Mouse released:");     for (int i = 0; i < count; i++) if (ImGui::IsMouseReleased(i))  { ImGui::SameLine(); ImGui::Text("b%d", i); }
            ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
            ImGui::Text("Pen Pressure: %.1f", io.PenPressure); // Note: currently unused
            ImGui::TreePop();
        }

        // Display Keyboard/Mouse state
        //IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Keyboard & Navigation State");
        if (ImGui::TreeNode("Keyboard & Navigation State"))
        {
            //ImGui::Text("Keys down:");          for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyDown(i))        { ImGui::SameLine(); ImGui::Text("%d (0x%X) (%.02f secs)", i, i, io.KeysDownDuration[i]); }
            ImGui::Text("Keys pressed:");       for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i))     { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
            ImGui::Text("Keys release:");       for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i))    { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
            ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
            ImGui::Text("Chars queue:");        for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); } // FIXME: We should convert 'c' to UTF-8 here but the functions are not public.

            ImGui::Text("NavInputs down:");     for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputs[i] > 0.0f)              { ImGui::SameLine(); ImGui::Text("[%d] %.2f (%.02f secs)", i, io.NavInputs[i], io.NavInputsDownDuration[i]); }
            ImGui::Text("NavInputs pressed:");  for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputsDownDuration[i] == 0.0f) { ImGui::SameLine(); ImGui::Text("[%d]", i); }

            ImGui::Button("Hovering me sets the\nkeyboard capture flag");
            if (ImGui::IsItemHovered())
                ImGui::CaptureKeyboardFromApp(true);
            ImGui::SameLine();
            ImGui::Button("Holding me clears the\nthe keyboard capture flag");
            if (ImGui::IsItemActive())
                ImGui::CaptureKeyboardFromApp(false);
            ImGui::TreePop();
        }

        //IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Tabbing");
        if (ImGui::TreeNode("Tabbing"))
        {
            ImGui::Text("Use TAB/SHIFT+TAB to cycle through keyboard editable fields.");
            static char buf[32] = "hello";
            ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
            ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
            ImGui::InputText("3", buf, IM_ARRAYSIZE(buf));
            ImGui::PushAllowKeyboardFocus(false);
            ImGui::InputText("4 (tab skip)", buf, IM_ARRAYSIZE(buf));
            ImGui::SameLine(); //HelpMarker("Item won't be cycled through when using TAB or Shift+Tab.");
            ImGui::PopAllowKeyboardFocus();
            ImGui::InputText("5", buf, IM_ARRAYSIZE(buf));
            ImGui::TreePop();
        }

        //IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Focus from code");
        if (ImGui::TreeNode("Focus from code"))
        {
            bool focus_1 = ImGui::Button("Focus on 1"); ImGui::SameLine();
            bool focus_2 = ImGui::Button("Focus on 2"); ImGui::SameLine();
            bool focus_3 = ImGui::Button("Focus on 3");
            int has_focus = 0;
            static char buf[128] = "click on a button to set focus";

            if (focus_1) ImGui::SetKeyboardFocusHere();
            ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
            if (ImGui::IsItemActive()) has_focus = 1;

            if (focus_2) ImGui::SetKeyboardFocusHere();
            ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
            if (ImGui::IsItemActive()) has_focus = 2;

            ImGui::PushAllowKeyboardFocus(false);
            if (focus_3) ImGui::SetKeyboardFocusHere();
            ImGui::InputText("3 (tab skip)", buf, IM_ARRAYSIZE(buf));
            if (ImGui::IsItemActive()) has_focus = 3;
            ImGui::SameLine(); //HelpMarker("Item won't be cycled through when using TAB or Shift+Tab.");
            ImGui::PopAllowKeyboardFocus();

            if (has_focus)
                ImGui::Text("Item with focus: %d", has_focus);
            else
                ImGui::Text("Item with focus: <none>");

            // Use >= 0 parameter to SetKeyboardFocusHere() to focus an upcoming item
            static float f3[3] = { 0.0f, 0.0f, 0.0f };
            int focus_ahead = -1;
            if (ImGui::Button("Focus on X")) { focus_ahead = 0; } ImGui::SameLine();
            if (ImGui::Button("Focus on Y")) { focus_ahead = 1; } ImGui::SameLine();
            if (ImGui::Button("Focus on Z")) { focus_ahead = 2; }
            if (focus_ahead != -1) ImGui::SetKeyboardFocusHere(focus_ahead);
            ImGui::SliderFloat3("Float3", &f3[0], 0.0f, 1.0f);

            ImGui::TextWrapped("NB: Cursor & selection are preserved when refocusing last used item in code.");
            ImGui::TreePop();
        }

        //IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Dragging");
        if (ImGui::TreeNode("Dragging"))
        {
            ImGui::TextWrapped("You can use ImGui::GetMouseDragDelta(0) to query for the dragged amount on any widget.");
            for (int button = 0; button < 3; button++)
            {
                ImGui::Text("IsMouseDragging(%d):", button);
                ImGui::Text("  w/ default threshold: %d,", ImGui::IsMouseDragging(button));
                ImGui::Text("  w/ zero threshold: %d,", ImGui::IsMouseDragging(button, 0.0f));
                ImGui::Text("  w/ large threshold: %d,", ImGui::IsMouseDragging(button, 20.0f));
            }

            ImGui::Button("Drag Me");
            if (ImGui::IsItemActive())
                ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f); // Draw a line between the button and the mouse cursor

            // Drag operations gets "unlocked" when the mouse has moved past a certain threshold
            // (the default threshold is stored in io.MouseDragThreshold). You can request a lower or higher
            // threshold using the second parameter of IsMouseDragging() and GetMouseDragDelta().
            ImVec2 value_raw = ImGui::GetMouseDragDelta(0, 0.0f);
            ImVec2 value_with_lock_threshold = ImGui::GetMouseDragDelta(0);
            ImVec2 mouse_delta = io.MouseDelta;
            ImGui::Text("GetMouseDragDelta(0):");
            ImGui::Text("  w/ default threshold: (%.1f, %.1f)", value_with_lock_threshold.x, value_with_lock_threshold.y);
            ImGui::Text("  w/ zero threshold: (%.1f, %.1f)", value_raw.x, value_raw.y);
            ImGui::Text("io.MouseDelta: (%.1f, %.1f)", mouse_delta.x, mouse_delta.y);
            ImGui::TreePop();
        }

        //IMGUI_DEMO_MARKER("Inputs, Navigation & Focus/Mouse cursors");
        if (ImGui::TreeNode("Mouse cursors"))
        {
            const char* mouse_cursors_names[] = { "Arrow", "TextInput", "ResizeAll", "ResizeNS", "ResizeEW", "ResizeNESW", "ResizeNWSE", "Hand", "NotAllowed" };
            IM_ASSERT(IM_ARRAYSIZE(mouse_cursors_names) == ImGuiMouseCursor_COUNT);

            ImGuiMouseCursor current = ImGui::GetMouseCursor();
            ImGui::Text("Current mouse cursor = %d: %s", current, mouse_cursors_names[current]);
            ImGui::Text("Hover to see mouse cursors:");
            ImGui::SameLine();/*  HelpMarker(
                "Your application can render a different mouse cursor based on what ImGui::GetMouseCursor() returns. "
                "If software cursor rendering (io.MouseDrawCursor) is set ImGui will draw the right cursor for you, "
                "otherwise your backend needs to handle it."); */
            for (int i = 0; i < ImGuiMouseCursor_COUNT; i++)
            {
                char label[32];
#ifdef _WIN32
				sprintf_s(label, "Mouse cursor %d: %s", i, mouse_cursors_names[i]);
#else
                sprintf(label, "Mouse cursor %d: %s", i, mouse_cursors_names[i]);
#endif
                ImGui::Bullet(); ImGui::Selectable(label, false);
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(i);
            }
            ImGui::TreePop();
        }
    }

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

void Gui::DrawWindowExplorer(Scene *scene)
{
	ImGui::SetNextWindowSize(ImVec2((float)window_messages_.w, (float)window_messages_.h));
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
		ImGui::BeginChild("##MessageList", ImVec2(0, (float)window_messages_.h - 60.0f), false, ImGuiWindowFlags_HorizontalScrollbar);

		std::vector<Message>* ptrMessages;
		ptrMessages = MessageManager::GetMessages();
		if (ptrMessages->size() > 0)
		{
			ImGuiListClipper clipper((int)ptrMessages->size());
			while (clipper.Step())
			{
				for (auto it = clipper.DisplayStart; it != clipper.DisplayEnd; it++)
				{
#ifndef DEVELOPMENT
                	if (ptrMessages->at(it).type == message_t::DEBUG)
                    	continue;
#endif 
					ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
					ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", ptrMessages->at(it).timeinfo.c_str());
					ImGui::SameLine(0, 2);
					ImGui::TextColored(ImVec4(1, 1, 1, 1), ":\t");

					std::string word;
					std::stringstream ss(ptrMessages->at(it).msg);

					ImGui::SameLine(0, 5);
					if (ptrMessages->at(it).type == message_t::ERROR_T)
					{
						ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "[ Error ]  %s", ptrMessages->at(it).msg.c_str());
					}
					else if (ptrMessages->at(it).type == message_t::INFO)
					{
						ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), "[ Info  ]  %s", ptrMessages->at(it).msg.c_str());
					}
					else if (ptrMessages->at(it).type == message_t::WARNING)
					{
						ImGui::TextColored(ImVec4(0.92f, 0.56f, 0.9f, 1.0f), "[Warning]  %s", ptrMessages->at(it).msg.c_str());
					}
					else if (ptrMessages->at(it).type == message_t::DEBUG)
					{
						ImGui::TextColored(ImVec4(0.42f, 0.85f, 1.0f, 1.0f), "[ Debug ]  %s", ptrMessages->at(it).msg.c_str());
					}
					else if (ptrMessages->at(it).type == message_t::DEBUG_WS)
					{
						ImGui::TextColored(ImVec4(0.42f, 0.85f, 1.0f, 1.0f), "[ Debug ]  %s", ptrMessages->at(it).msg.c_str());
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

void Gui::DrawTabTileExplorer(Scene *scene)
{
	if (!TilemapManager::IsEmpty())
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		if (ImGui::BeginTabItem("Tiles"))
		{
			static ImGuiTableFlags flags1 = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders | ImGuiTableFlags_ContextMenuInBody;
			if (ImGui::BeginTable("table_context_menu", 2, flags1))
			{
				ImGui::TableSetupColumn("Tilemaps");
				ImGui::TableSetupColumn("Tiles");

				// [1.1]] Right-click on the TableHeadersRow() line to open the default table context menu.
				ImGui::TableHeadersRow();

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);

				for (auto const& [key, val] : TilemapManager::Tilemaps)
				{
					if(key != "")
					{
						ImGui::PushID(key.c_str());
						if (ImGui::Selectable(key.c_str(), key.compare(scene->GetActiveTilemap())))
						{
							scene->SetActiveTilemap(key);
						}
						ImGui::PopID();
					}
				}

				ImGui::TableSetColumnIndex(1);
				static glm::vec2 tileButtonScale = glm::vec2(2.0f, 2.0f);
				Tilemap* tilemap = TilemapManager::GetTilemap(scene->GetActiveTilemap());
				std::vector<std::string> tilemapHashes = tilemap->GetHashs();

				ImGui::BeginChild("##TileSelector",
					ImVec2(0,
						((tilemap->NumRows() > 0) ? tilemap->NumRows() : 1.0f)
						* tilemap->GetSpriteSize().y
						* tilemap->GetSpriteScale().y 
						* tileButtonScale.y + 5.0f + style->ScrollbarSize),
					true,
					ImGuiWindowFlags_HorizontalScrollbar
				);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

				GLuint i = 0;
				ImGuiListClipper clipper(tilemap->NumRows());

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
	window_scene_.wPercent = 0.7f;
	window_scene_.hPercent = 0.6f;

	window_messages_.wPercent = 1.0f;
	window_messages_.hPercent = 1.0f - window_scene_.hPercent;

	window_sidebar_right_.wPercent = 1.0f - window_scene_.wPercent;
	window_sidebar_right_.hPercent = window_scene_.hPercent;

	main_menubar_height_ = 23;
	ImGuiStyle& style = ImGui::GetStyle();
	style.DisplaySafeAreaPadding = ImVec2(0.0f, 0.0f);

	state_ = gui_state_t::GUI_ACTIVE;

	file_browser_add_tiles_ = false;
#ifdef __EMSCRIPTEN__
	file_browser_emscripten_open_ = false;
#endif

	show_demo_imgui_ = false;
    show_backend_checker_show_ = false;
#ifdef _WIN32
	TCHAR path[256];
	GetCurrentDirectory(256, path);
	root_file_path_ = path;
#elif __linux__
    root_file_path_ = fs::current_path();
#elif __EMSCRIPTEN__

#endif

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
	font_default_ = io.Fonts->AddFontFromFileTTF("data/fonts/Roboto-Medium.ttf", 13.0f);
    ImFontConfig config;
    //config.MergeMode = true;
    //config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    //io.Fonts->AddFontFromFileTTF("data/fonts/fa-brands-400.ttf", 14.0f, &config, icon_ranges);
    //io.Fonts->AddFontFromFileTTF("data/fonts/fa-regular-400.ttf", 14.0f, &config, icon_ranges);
    //io.Fonts->AddFontFromFileTTF("data/fonts/fa-regular-400.ttf", 13.0f, &config, icon_ranges);
    //io.Fonts->AddFontFromFileTTF("data/fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);
    icons_13_ = io.Fonts->AddFontFromFileTTF("data/fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);
    icons_40_ = io.Fonts->AddFontFromFileTTF("data/fonts/fa-solid-900.ttf", 40.0f, &config, icon_ranges);

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

#ifdef __EMSCRIPTEN__
GLvoid Gui::fileBrowserAddTile()
{
	file_browser_emscripten_open_ = true;
}

GLvoid Gui::listDirectoryContent()
{
}
#else
GLvoid Gui::fileBrowserAddTile(Scene* scene, fs::path& path, GLboolean extensionOnly, fs::path extension)
{
	//prepare for file browser popup
	ImGui::SetNextWindowSize(ImVec2(800, 580));
	ImGui::SetNextWindowPos(ImVec2(100, 100), 1);

	static bool displayLogicalDrives = false;
	bool isLogicalDrive = false;
	static std::string currentItem = ""; //current selection in the current directory
	static char newPath[250] = ""; //path which can be inserted via an ImGui::InputText
	if (!strcmp(newPath, ""))
	{
#if defined(_WIN32)
		strcpy_s(newPath, path.string().c_str());
#else
		std::strcpy(newPath, path.string().c_str());
#endif
	}


	ImGui::OpenPopup("File Browser");
	if (ImGui::BeginPopupModal("File Browser", nullptr, 0))
	{
		try
		{
			if (fs::exists(path))
			{
				ImGui::PushItemWidth(645);
				//Path as InputText
				ImGui::InputText("##Current", newPath, 300);
				ImGui::PopItemWidth();

				bool isSelected = (currentItem.compare((path.parent_path()).string()));

				ImGui::SameLine();
				//Button to navigate to parent directory
				if (ImGui::Button("Up", ImVec2(120, 0)))
				{
					currentItem = fs::canonical(path).string();
					if (currentItem == fs::canonical(fs::canonical(currentItem).parent_path()).string())
					{
						displayLogicalDrives = true;
					}

					currentItem = fs::canonical(path.parent_path()).string();
				}

				//ImGui::Separator();

				//List of files in the current directory

				listDirectoryContent(path, displayLogicalDrives, isLogicalDrive, extensionOnly, extension, &currentItem, isSelected);

				
				if (displayLogicalDrives)
				{
#if defined(_WIN32)
					strcpy_s(newPath, "~\\");
#else
					strcpy(newPath, "~\\");
#endif
				}
				ImGui::TextWrapped(currentItem.c_str());
			}
		}
		catch (...)
		{
			path = fs::current_path();
#if defined(_WIN32)
			strcpy_s(newPath, "~\\");
#else
			strcpy(newPath, "~\\");
#endif
			displayLogicalDrives = true;

			ImGui::OpenPopup("##FilesystemError");
		}

		ImGui::SetNextWindowSize(ImVec2(180, 86));
		if (ImGui::BeginPopupModal("##FilesystemError", nullptr, 0))
		{
			ImGui::TextWrapped("Cannot open that directory.");
			if (ImGui::Button("Ok", ImVec2(120, 0)))
			{
				currentItem = "";
#if defined(_WIN32)
				strcpy_s(newPath, "");
#else
				strcpy(newPath, "");
#endif
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		//Button for loading selected item
		if (ImGui::Button("Ok", ImVec2(120, 0)))
		{

			std::string str = &currentItem[0u];

			if (str == "")
			{
				ImGui::OpenPopup("##NoFileselected");
			}
			else
			{
				//active_tilemap_name_ = str;
				//tilemap_list_.push_back(str);
				//ResourceManager::LoadTexture(str.c_str(), GL_TRUE, str);
				// TODO: find or ask for sprite size and scale
				TilemapManager::AddTilemap(str, { 16, 16 }, { 1.0f, 1.0f }, str);
				scene->SetActiveTilemap(str);
				file_browser_add_tiles_ = false;
				ImGui::CloseCurrentPopup();
			}
			currentItem = "";
#if defined(_WIN32)
			strcpy_s(newPath, "");
#else
			strcpy(newPath, "");
#endif
		}

		//PopUp if no file was selected
		ImGui::SetNextWindowSize(ImVec2(180, 86));
		if (ImGui::BeginPopupModal("##NoFileselected", nullptr, 0))
		{
			ImGui::TextWrapped("No File selected.");
			if (ImGui::Button("Ok", ImVec2(120, 0)))
			{
				currentItem = "";
#if defined(_WIN32)
				strcpy_s(newPath, "~\\");
#else
				strcpy(newPath, "");
#endif
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			file_browser_add_tiles_ = false;
		}

		ImGui::SetItemDefaultFocus();
		ImGui::EndPopup();
	}

	//reset if navigated to a new directory
	if (isLogicalDrive || fs::is_directory(currentItem))
	{
		path = currentItem;
		currentItem = "";
#if defined(_WIN32)
		strcpy_s(newPath, "");
#else
		strcpy(newPath, "");
#endif
	}
	else {
		//if valid path was inserted into InputText navigate to that directory
		try
		{
			if (fs::exists(newPath) && strcmp(newPath, path.string().c_str()))
			{
				path = newPath;
			}
		}
		catch (...)
		{
			// TODO: Catch error
		}
	}
}


GLvoid
Gui::listDirectoryContent(
	fs::path path,
	bool displayLogicalDrives,
	bool isLogicalDrive,
	bool extensionOnly,
	fs::path extension,
	std::string* currentItem,
	bool isSelected)
{
#ifndef _WIN32
	(void)isLogicalDrive;
#endif

	ImGui::BeginChild("##ListOfFiles", ImVec2(0, 460.0f), false, ImGuiWindowFlags_HorizontalScrollbar);

	if (displayLogicalDrives == false)
	{
		for (const auto& entry : fs::directory_iterator(path))
		{
			std::string entryString = (fs::path(entry).filename()).string();
			//std::string entryExtension = fs::path(entry).extension().string();
			isSelected = (currentItem->compare(entryString));
			//current_item = fs::path(entry).string();


			if (fs::path(entry).extension() == ".sys") {}	//exclude .sys-Files which can be problematic if acessed
			else {
				if (fs::is_directory(entry))
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.6f, 0.3f, 0.8f));

				if (extensionOnly)
				{
					if (fs::path(entry).extension() == extension || fs::is_directory(fs::status(entry)))
					{
						if (ImGui::Selectable(entryString.c_str(), !isSelected, ImGuiSelectableFlags_DontClosePopups))
						{
							try {
								*currentItem = fs::canonical(entry).string();
							}
							catch (int e) {
								std::stringstream msg;
								msg << "Exception occurred. Exception Nr. " << e << std::endl;
								MessageManager::AddMessage(msg, message_t::ERROR_T);
							}
						}
					}
				}
				else
				{
					if (ImGui::Selectable(entryString.c_str(), !isSelected, ImGuiSelectableFlags_DontClosePopups))
					{
						try {
							*currentItem = fs::canonical(entry).string();
						}
						catch (int e) {
							std::stringstream msg;
							msg << "Exception occurred. Exception Nr. " << e << std::endl;
							MessageManager::AddMessage(msg, message_t::ERROR_T);
						}
					}
				}

				if (fs::is_directory(entry))
					ImGui::PopStyleColor();
				//if (is_selected)
				//	ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
		}
	}
	else
	{
#ifdef _WIN32
		WCHAR szDrive[] = L"A:\\";
		DWORD uDriveMask = GetLogicalDrives();
		if (uDriveMask == 0) {
			std::stringstream msg;
			msg << "GetLogicalDrives failed!" << std::endl;
			MessageManager::AddMessage(msg, message_t::ERROR_T);
		}
		else
		{
			while (uDriveMask)
			{
				if (uDriveMask & 1)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.6f, 0.3f, 0.8f));
					std::string name = (const char*)szDrive;
					name += ":\\";
					if (ImGui::Selectable(name.c_str(), !isSelected, ImGuiSelectableFlags_DontClosePopups)) {
						try {
							*currentItem = fs::path(name).string();
							isLogicalDrive = true;
							displayLogicalDrives = false;
						}
						catch (int e) {
							std::stringstream msg;
							msg << "Exception occurred. Exception Nr. " << e << std::endl;
							MessageManager::AddMessage(msg, message_t::ERROR_T);
						}
					}
					ImGui::PopStyleColor();
				}
				szDrive[0]++;
				uDriveMask >>= 1;
			}
		}
		#endif // _WIN32
	}
	ImGui::EndChild();
}
#endif