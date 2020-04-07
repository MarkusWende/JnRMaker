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


#include "../include/gui.h"

Gui::Gui()
{
	init_();
	customGuiStyle_();

	active_tilemap_name_ = "tiles_keen4";
	active_sprite_name_ = "r0c0";
}

Gui::~Gui()
{

}

GLvoid Gui::Update(GLuint width, GLuint height, Scene &scene)
{
	// Check if application window size has been changed
	if (width_ != width || height_ != height) {
		width_ = width;
		height_ = height;

		window_scene_.w = width_ * window_scene_.wPercent;
		window_scene_.h = height_ * window_scene_.hPercent;

		window_messages_.w = width_ * window_messages_.wPercent;
		window_messages_.h = height_ * window_messages_.hPercent;

		window_sidebar_right_.w = width_ * window_sidebar_right_.wPercent;
		window_sidebar_right_.h = height_ * window_sidebar_right_.hPercent;

		//scene.SetWidth(window_scene_.w-6);
		//scene.SetHeight(window_scene_.h-51);
		//scene.Update();
	}

	ImGuiStyle& style = ImGui::GetStyle();
	style.DisplaySafeAreaPadding = ImVec2(0.0f, main_menubar_height_ - 23);

	// Check if mouse is over scene window
	//if (ImGui::IsWindowHovered())
	//	scene.mouseOverScene = true;
	//else
	//	scene.mouseOverScene = false;
}

GLvoid Gui::Render(Scene &scene)
{
  // Main menu
  {
    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
          ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit"))
      {
          if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
          if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
          ImGui::Separator();
          if (ImGui::MenuItem("Cut", "CTRL+X")) {}
          if (ImGui::MenuItem("Copy", "CTRL+C")) {}
          if (ImGui::MenuItem("Paste", "CTRL+V")) {}
          ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }

	// Scene Render Window
	{
		ImGui::SetNextWindowSize(ImVec2(window_scene_.w, window_scene_.h - main_menubar_height_));
		ImGui::SetNextWindowPos(ImVec2(0, main_menubar_height_));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::Begin("Rendering", NULL,	ImGuiWindowFlags_NoTitleBar |
																		ImGuiWindowFlags_NoResize |
																		ImGuiWindowFlags_NoMove |
																		ImGuiWindowFlags_NoScrollbar |
																		ImGuiWindowFlags_NoScrollWithMouse |
																		ImGuiWindowFlags_NoCollapse);
		//ImGui::GetWindowDrawList()->AddImage((ImTextureID)ResourceManager::GetFramebuffer("imguiScene").texID, ImVec2(0, 25), ImVec2(sceneWidth, sceneHeight + 25));

		if (ImGui::BeginTabBar("SceneTabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Viewport"))
			{
				if (!scene.IsMapNull())
				{
					//sf::Sprite spr;
					sf::RenderTexture* tex = ResourceManager::GetRenderTexture("viewport");
					//spr.setTexture(tex->getTexture());
					//spr.setTextureRect(sf::IntRect(0, window_scene_.h, window_scene_.w, -window_scene_.h));
					ImGui::SetCursorPos(ImVec2(2.f, 24.f));

					ImGui::Image(tex->getTexture(),
						sf::Vector2f(scene.GetWidth(), scene.GetHeight()),
						sf::FloatRect(0, (float)scene.GetHeight(), (float)scene.GetWidth(), -(float)scene.GetHeight()),
						sf::Color(255, 255, 255, 255),
						sf::Color(0, 255, 0, 255));

					sf::RenderTexture* texBg = ResourceManager::GetRenderTexture("tex_background");
					//spr.setTexture(tex->getTexture());
					//spr.setTextureRect(sf::IntRect(0, window_scene_.h, window_scene_.w, -window_scene_.h));
					ImGui::SetCursorPos(ImVec2(800.f, 24.f));
					
					ImGui::Image(texBg->getTexture(),
						sf::Vector2f(32, texBg->getSize().y),
						sf::FloatRect(0, (float)texBg->getSize().y, (float)texBg->getSize().x, -(float)texBg->getSize().y),
						sf::Color(255, 255, 255, 255),
						sf::Color(0, 255, 0, 255));
						
					//ImGui::Image(texBg->getTexture());
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Depth"))
			{
				//ImGui::Checkbox("Show Floor", &show_main_dancefloor);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Wireframe"))
			{
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		if (ImGui::IsWindowHovered())
			scene.SetMouseOverScene(true);
		else
			scene.SetMouseOverScene(false);

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

	// Messages Window
	//if (false)
	{
		ImGui::SetNextWindowSize(ImVec2(window_messages_.w, window_messages_.h));
		ImGui::SetNextWindowPos(ImVec2(0, window_scene_.h));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
		ImGui::Begin("Messages", NULL, 	ImGuiWindowFlags_NoTitleBar |
																		ImGuiWindowFlags_NoResize |
																		ImGuiWindowFlags_NoMove |
																		ImGuiWindowFlags_NoCollapse);
		//ImGui::GetWindowDrawList()->AddImage((ImTextureID)ResourceManager::GetFramebuffer("imguiScene").texID, ImVec2(0, 25), ImVec2(sceneWidth, sceneHeight + 25));

		static float wrap_width = 1200.0f;
		ImGui::Text("(%.2f FPS)", ImGui::GetIO().Framerate); ImGui::SameLine();

		ImGui::Separator();

		// BeginChild: MessageList
		ImGui::BeginChild("##MessageList", ImVec2(0, 60.0f), false, ImGuiWindowFlags_HorizontalScrollbar);

		std::vector<Message>* ptrMessages;
		ptrMessages = MessageManager::GetMessages();
		if (ptrMessages->size() > 0)
		{
			for (auto it = ptrMessages->begin(); it != ptrMessages->end(); ++it)
			{
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
				ImGui::TextColored(ImVec4(1, 1, 1, 1), it->timeinfo.c_str());
				ImGui::SameLine(0, 2);
				ImGui::TextColored(ImVec4(1, 1, 1, 1), ":\t");

				std::string word;
				std::stringstream ss(it->msg);

				ImGui::SameLine(0, 5);
				if (it->type == message_t::ERROR_T)
				{
					ImGui::TextColored(ImVec4(1, 0, 0, 1), it->msg.c_str());
				}
				else if (it->type == message_t::INFO)
				{
					ImGui::TextColored(ImVec4(0, 1, 0, 1), it->msg.c_str());
				}
				else if (it->type == message_t::WARNING)
				{
					ImGui::TextColored(ImVec4(1, 1, 0, 1), it->msg.c_str());
				}
				else
				{
					ImGui::Text("%s", it->msg.c_str());
				}

			}
		}

		ImGui::SetScrollHere(1.0f);
		// EndChild: MessageList
		ImGui::EndChild();

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

	// Side Bar Right Window
	{
		ImGui::SetNextWindowSize(ImVec2(window_sidebar_right_.w, window_sidebar_right_.h - main_menubar_height_));
		ImGui::SetNextWindowPos(ImVec2(window_scene_.w, main_menubar_height_));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
		ImGui::Begin("SideBarRight", NULL, 	ImGuiWindowFlags_NoTitleBar |
											ImGuiWindowFlags_NoResize |
											ImGuiWindowFlags_NoMove |
											ImGuiWindowFlags_NoCollapse);

		if (!scene.IsMapNull())
		{
			ImGui::SetCursorPos(ImVec2(15.f, 5.f));
			GLuint width = scene.GetMapWidth();
			GLuint height = scene.GetMapHeight();
			height = (int)((350.0f / (float)width) * (float)height);
			ImGui::BeginChild("Minimap", ImVec2(400, height + 30));
			ImGui::Text("Minimap");
		
			sf::RenderTexture* tex = ResourceManager::GetRenderTexture("minimap");
			ImGui::SetCursorPos(ImVec2(0.f, 18.f));
			ImGui::Image(tex->getTexture(),
				sf::Vector2f(350, height),
				sf::FloatRect(0, (float)scene.GetHeight(), (float)scene.GetWidth(), -(float)scene.GetHeight()),
				sf::Color(255, 255, 255, 255),
				sf::Color(0, 255, 0, 255));
			ImGui::EndChild();
		}
		

		if (ImGui::CollapsingHeader("Map"))
		{
			if (ImGui::Button("Create"))
			{
				scene.CreateMap(16, 8, { 16, 16 }, {2.0f, 2.0f});
			}
		}

		if (ImGui::CollapsingHeader("Tiles"))
		{
			ImGui::BeginChild("TileSelector", ImVec2(0, 500), true, ImGuiWindowFlags_HorizontalScrollbar);

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

			Tilemap* tilemap = TilemapManager::GetTilemap(active_tilemap_name_);

			GLuint i = 0;
			ImGuiListClipper clipper(tilemap->NumRows());
			while (clipper.Step())
			{
				for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
				{
					for (GLuint col = 0; col < tilemap->NumCols(); col++)
					{
						std::stringstream sprKey;
						sprKey << "r" << row << "c" << col;
						sf::Sprite* sprValue = tilemap->GetSprite(sprKey.str());
						ImGui::PushID(i);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.4));
						if (ImGui::ImageButton(*sprValue, 1, sf::Color(0, 0, 0, 0), sf::Color(200, 200, 200, 255)))
						{
							active_sprite_name_ = sprKey.str();
							scene.SetCurrentTilemap(active_tilemap_name_);
							scene.SetCurrentSprite(active_sprite_name_);
						}
						ImGui::PopStyleColor(2);
						ImGui::PopID();
						if (col < tilemap->NumCols() - 1)
							ImGui::SameLine();
						i++;
					}
				}
			}

			ImGui::PopStyleVar();

			//ImGui::EndChildFrame();
			ImGui::EndChild();
		}

		if (ImGui::CollapsingHeader("Sprites"))
		{

		}

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

	// Imgui Demo Window
	ImGui::ShowDemoWindow();
}


// PRIVATE
GLvoid Gui::init_()
{
	window_scene_.wPercent = 0.8f;
	window_scene_.hPercent = 0.9f;

	window_messages_.wPercent = 1.0f;
	window_messages_.hPercent = 0.1f;

	window_sidebar_right_.wPercent = 0.2f;
	window_sidebar_right_.hPercent = 0.9f;

	main_menubar_height_ = 0 + 23;
}

GLvoid Gui::customGuiStyle_()
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
	io.Fonts->AddFontFromFileTTF("assets/fonts/ProggyTiny.ttf", 6.0f);
	//io.Fonts->AddFontFromFileTTF("../assets/fonts/Roboto-Medium.ttf", 12.0f);
	//io.Fonts->AddFontFromFileTTF("../assets/fonts/Cousine-Regular.ttf", 12.0f);
	//io.Fonts->AddFontFromFileTTF("../assets/fonts/DroidSans.ttf", 12.0f);
	//io.Fonts->AddFontFromFileTTF("../assets/fonts/arialuni.ttf", 12.0f);
  //io.Fonts->AddFontFromFileTTF("../assets/fonts/arial.ttf", 12.0f);
  //io.Fonts->AddFontFromFileTTF("../assets/fonts/arialbd.ttf", 12.0f);
  //io.Fonts->AddFontFromFileTTF("../assets/fonts/arialbi.ttf", 12.0f);
  //io.Fonts->AddFontFromFileTTF("../assets/fonts/ariali.ttf", 12.0f);
  //io.Fonts->AddFontFromFileTTF("../assets/fonts/ariblk.ttf", 12.0f);
  //io.Fonts->AddFontFromFileTTF("../assets/fonts/Karla-Regular.ttf", 12.0f);
  //io.Fonts->AddFontFromFileTTF("../assets/fonts/ProggyClean.ttf", 12.0f);
  //io.Fonts->AddFontFromFileTTF("../assets/fonts/vrinda.ttf", 12.0f);
  //io.Fonts->AddFontFromFileTTF("../assets/fonts/vrindab.ttf", 12.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	// Gui shape
	ImGuiStyle* style = &ImGui::GetStyle();
	style->Alpha = 1.0f;								// Global alpha applies to everything in ImGui.
	style->WindowPadding = ImVec2(15, 15);				    // Padding within a window.
	style->WindowRounding = 0.0f;						        // Radius of window corners rounding. Set to 0.0f to have rectangular windows.
	style->WindowBorderSize = 1.0f;						        // Thickness of border around windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
	style->WindowMinSize = ImVec2(20, 20);				    // Minimum window size. This is a global setting. If you want to constraint individual windows, use SetNextWindowSizeConstraints().
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);               // Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered.
	style->ChildRounding = 0.0f;						        // Radius of child window corners rounding. Set to 0.0f to have rectangular windows.
	style->ChildBorderSize = 1.0f;						        // Thickness of border around child windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
	style->PopupRounding = 0.0f;						        // Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)
	style->PopupBorderSize = 1.0f;						        // Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
	style->FramePadding = ImVec2(5, 5);					    // Padding within a framed rectangle (used by most widgets).
	style->FrameRounding = 0.0f;						        // Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).
	style->FrameBorderSize = 0.0f;						        // Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
	style->ItemSpacing = ImVec2(5, 4);						// Horizontal and vertical spacing between widgets/lines.
	style->ItemInnerSpacing = ImVec2(8, 6);				        // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).
	style->TouchExtraPadding = ImVec2(5, 5);				        // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
	style->IndentSpacing = 25.0f;						    // Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
	style->ColumnsMinSpacing = 0.0f;					            // Minimum horizontal spacing between two columns.
	style->ScrollbarSize = 15.0f;						    // Width of the vertical scrollbar, Height of the horizontal scrollbar.
	style->ScrollbarRounding = 0.0f;					            // Radius of grab corners for scrollbar.
	style->GrabMinSize = 5.0f;							    // Minimum width/height of a grab box for slider/scrollbar.
	style->GrabRounding = 0.0f;							    // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
	style->TabRounding = 0.0f;							    // Radius of upper corners of a tab. Set to 0.0f to have rectangular tabs.
	style->TabBorderSize = 1.0f;						        // Thickness of border around tabs.
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
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
	style->Colors[ImGuiCol_Tab] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
	style->Colors[ImGuiCol_TabHovered] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
}
