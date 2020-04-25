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
	init();
	customGuiStyle();
}

Gui::~Gui()
{

}

GLvoid Gui::WindowUpdate(GLuint width, GLuint height)
{
	width_ = width;
	height_ = height;

	window_scene_.w = width_ * window_scene_.wPercent;
	window_scene_.h = height_ * window_scene_.hPercent;

	window_messages_.w = width_ * window_messages_.wPercent;
	window_messages_.h = height_ * (1.0f - window_scene_.hPercent);

	window_sidebar_right_.w = width_ * (1.0f - window_scene_.wPercent);
	window_sidebar_right_.h = height_ * window_scene_.hPercent;
}

GLvoid Gui::WindowUpdate()
{
	window_scene_.w = width_ * window_scene_.wPercent;
	window_scene_.h = height_ * window_scene_.hPercent;

	window_messages_.w = width_ * window_messages_.wPercent;
	window_messages_.h = height_ * (1.0f - window_scene_.hPercent);

	window_sidebar_right_.w = width_ * (1.0f - window_scene_.wPercent);
	window_sidebar_right_.h = height_ * window_scene_.hPercent;
}

GLvoid Gui::Render(Scene &scene)
{
  // Main menu
  {
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
		  if (ImGui::MenuItem("Open"))
		  {
			  file_browser_add_tiles_ = true;
		  }
		  if (ImGui::MenuItem("Quit"))
		  {
			  state_ = gui_state_t::GUI_CLOSE;
		  }

          ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit"))
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
          ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }

  if (file_browser_add_tiles_)
  {
	  fileBrowserAddTile(root_file_path_, true, ".png");
  }


	// Scene Render Window
	{
		ImGui::SetNextWindowSize(ImVec2(window_scene_.w, window_scene_.h));
		ImGui::SetNextWindowPos(ImVec2(0, main_menubar_height_));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::Begin("Rendering", NULL,	ImGuiWindowFlags_NoTitleBar |
										ImGuiWindowFlags_NoMove |
										ImGuiWindowFlags_NoScrollbar |
										ImGuiWindowFlags_NoScrollWithMouse |
										ImGuiWindowFlags_NoCollapse);

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
					//ImGui::SetCursorPos(ImVec2(0.f, 24.f));
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
					ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
					ImGui::BeginChildFrame(1, ImVec2(window_scene_.w, window_scene_.w));
					{
						ImGui::Image(tex->getTexture(),
							sf::Vector2f(scene.GetWidth(), scene.GetHeight()),
							sf::FloatRect(0, (float)scene.GetHeight(), (float)scene.GetWidth(), -(float)scene.GetHeight()),
							sf::Color(255, 255, 255, 255),
							sf::Color(0, 255, 0, 255));

						if (ImGui::IsItemHovered())
							scene.SetMouseOverScene(true);
						else
							scene.SetMouseOverScene(false);
					}
					ImGui::EndChildFrame();
					ImGui::PopStyleColor();
					ImGui::PopStyleVar();
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Depth"))
			{
				scene.SetMouseOverScene(false);

				GLuint texId = ResourceManager::GetTextureID("viewport");

				ImGui::GetWindowDrawList()->AddImage(texId, ImVec2(0, 50), ImVec2(scene.GetWidth(), scene.GetHeight() + 50));
				//ImGui::Image(textureID, size, uv0, uv1, sf::Color(255, 255, 255, 255), sf::Color(0, 255, 0, 255));

				/*
				static ImVec2 offset(0, 0);
				//ImGui::DragFloat2("size", (float*)&size, 0.5f, 1.0f, 200.0f, "%.0f");
				ImVec2 pos = ImGui::GetCursorScreenPos();
				ImVec4 clip_rect(pos.x, pos.y, pos.x + window_scene_.w, pos.y + window_scene_.h);
				ImGui::InvisibleButton("##dummy", ImVec2(window_scene_.w, window_scene_.h));
				if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
				{
					offset.x += ImGui::GetIO().MouseDelta.x;
					offset.y += ImGui::GetIO().MouseDelta.y;
				}



				ImGui::SetCursorPos(ImVec2(2.f, 24.f));
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
				ImGui::BeginChildFrame(1, ImVec2(window_scene_.w, window_scene_.w));
				{

					ImGui::Image(tex->getTexture(),
						sf::Vector2f(scene.GetWidth(), scene.GetHeight()),
						sf::FloatRect(offset.x, (float)scene.GetHeight(), (float)scene.GetWidth() + offset.x, -(float)scene.GetHeight()),
						sf::Color(255, 255, 255, 255),
						sf::Color(0, 255, 0, 255));


					//ImGui::GetWindowDrawList()->AddImage(textureID, ImVec2(0, 25), ImVec2(scene.GetWidth(), scene.GetHeight()));
				}
				ImGui::EndChildFrame();
				ImGui::PopStyleColor();
				*/
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Memory"))
			{
				sf::RenderTexture* tex = ResourceManager::GetRenderTexture("tex_used_tiles");

				ImGui::SetCursorPos(ImVec2(window_scene_.w / 2.0f, 33.0f));
				ImGui::Text("Tiles used and which will be saved.");
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f , 0.2f , 0.2f, 1.0f));
				ImGui::BeginChildFrame(2, ImVec2(window_scene_.w, 50.0f));
				{
					ImGui::Separator();

					ImGui::Image(tex->getTexture(),
						sf::Vector2f(tex->getSize().x, tex->getSize().y),
						sf::FloatRect(0, (float)tex->getSize().y, (float)tex->getSize().x, -(float)tex->getSize().y),
						sf::Color(255, 255, 255, 255),
						sf::Color(0, 255, 0, 0));
				}
				ImGui::EndChildFrame();
				ImGui::PopStyleColor();

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		GLuint windowWidth = ImGui::GetWindowWidth();
		GLuint windowHeight = ImGui::GetWindowHeight();

		if ((windowWidth != window_scene_.w) || (windowHeight != window_scene_.h))
		{
			window_scene_.wPercent = (GLfloat)windowWidth / (GLfloat)width_;
			window_scene_.hPercent = (GLfloat)windowHeight / (GLfloat)height_;

			WindowUpdate();
		}

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

	// Messages Window
	//if (false)
	{
		ImGui::SetNextWindowSize(ImVec2(window_messages_.w, window_messages_.h));
		ImGui::SetNextWindowPos(ImVec2(0, window_scene_.h + main_menubar_height_));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
		ImGui::Begin("Messages", NULL, 	ImGuiWindowFlags_NoTitleBar |
										ImGuiWindowFlags_NoMove |
										ImGuiWindowFlags_NoCollapse);

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
					ImGui::TextColored(ImVec4(0.8, 0.2, 0, 1), ("[Error]\t" + it->msg).c_str());
				}
				else if (it->type == message_t::INFO)
				{
					ImGui::TextColored(ImVec4(0, 0.8, 0, 1), ("[Info]\t " + it->msg).c_str());
				}
				else if (it->type == message_t::WARNING)
				{
					ImGui::TextColored(ImVec4(0.7, 0.7, 0, 1), ("[Warning]  " + it->msg).c_str());
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

		GLuint windowHeight = ImGui::GetWindowHeight();

		if (windowHeight != window_scene_.h)
		{
			window_scene_.hPercent = (GLfloat)(height_ - windowHeight) / (GLfloat)height_;

			WindowUpdate();
		}

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

	// Side Bar Right Window
	{
		ImGui::SetNextWindowSize(ImVec2(window_sidebar_right_.w, window_sidebar_right_.h));
		ImGui::SetNextWindowPos(ImVec2(window_scene_.w, main_menubar_height_));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
		ImGui::Begin("SideBarRight", NULL, 	ImGuiWindowFlags_NoTitleBar |
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
				scene.CreateMap(40, 25, { 16, 16 }, {2.0f, 2.0f});
			}

			static int selected = 0;
			for (int n = 0; n < 3; n++)
			{
				char buf[32];
				sprintf(buf, "Layer %d", n);
				if (ImGui::Selectable(buf, selected == n))
					selected = n;
			}

			if (selected == 0)
			{
				scene.SetActiveLayer(layer_t::BACK);
			}
			else if (selected == 1)
			{
				scene.SetActiveLayer(layer_t::PLAYER);
			}
			else if (selected == 2)
			{
				scene.SetActiveLayer(layer_t::FORE);
			}
		}

		if (ImGui::CollapsingHeader("Tiles"))
		{
			if (!tilemap_list_.empty())
			{
				if (ImGui::BeginCombo("##TilemapCombo", active_tilemap_name_.c_str()))
				{
					for (size_t n = 0; n < tilemap_list_.size(); n++)
					{
						std::string item = tilemap_list_.at(n);
						ImGui::PushID(n);
						if (ImGui::Selectable(tilemap_list_.at(n).c_str(), item.compare(active_tilemap_name_)))
						{
							active_tilemap_name_ = item;
						}

						ImGui::PopID();
					}
					ImGui::EndCombo();
				}

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
							// Change mouse cursor to hand
							if (ImGui::IsItemHovered() || ImGui::IsItemFocused())
								ImGui::SetMouseCursor(7);
							if (ImGui::ImageButton(*sprValue, 1, sf::Color(0, 0, 0, 0), sf::Color(200, 200, 200, 255)))
							{
								active_sprite_name_ = sprKey.str();
								scene.SetActiveTilemap(active_tilemap_name_);
								scene.SetActiveSprite(active_sprite_name_);
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
		}

		if (ImGui::CollapsingHeader("Sprites"))
		{

		}

		GLuint windowWidth = ImGui::GetWindowWidth();
		GLuint windowHeight = ImGui::GetWindowHeight();

		if ((windowWidth != window_scene_.w) || (windowHeight != window_scene_.h))
		{
			window_scene_.wPercent = (GLfloat)(width_ - windowWidth) / (GLfloat)width_;
			window_scene_.hPercent = (GLfloat)windowHeight / (GLfloat)height_;

			WindowUpdate();
		}

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

	// Imgui Demo Window
	ImGui::ShowDemoWindow();
}


// PRIVATE
GLvoid Gui::init()
{
	window_scene_.wPercent = 0.7f;
	window_scene_.hPercent = 0.9f;

	window_messages_.wPercent = 1.0f;
	window_messages_.hPercent = 1.0f - window_scene_.hPercent;

	window_sidebar_right_.wPercent = 1.0f - window_scene_.wPercent;
	window_sidebar_right_.hPercent = window_scene_.hPercent;

	main_menubar_height_ = 23;
	ImGuiStyle& style = ImGui::GetStyle();
	style.DisplaySafeAreaPadding = ImVec2(0.0f, 0.0f);

	state_ = gui_state_t::GUI_ACTIVE;

	file_browser_add_tiles_ = false;

	std::string path;
	char const* home = std::getenv("HOME");
	if (home || (home = getenv("USERPROFILE")))
	{
		path.replace(0, 1, home);
	}
	else
	{
		char const* hdrive = getenv("HOMEDRIVE");
		char const* hpath = getenv("HOMEPATH");
		path.replace(0, 1, std::string(hdrive) + hpath);
	}

	root_file_path_ = path;
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


GLvoid Gui::fileBrowserAddTile(fs::path& path, GLboolean extensionOnly, fs::path extension)
{
	//prepare for file browser popup
	ImGui::SetNextWindowSize(ImVec2(800, 580));
	ImGui::SetNextWindowPos(ImVec2(100, 100), 1);

	static bool displayLogicalDrives = false;
	bool isLogicalDrive = false;
	static std::string currentItem = ""; //current selection in the current directory
	static char newPath[250] = ""; //path which can be inserted via an ImGui::InputText
	if (!strcmp(newPath, "")) {
		std::strcpy(newPath, path.string().c_str());
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
				listDirectoryContent(path, displayLogicalDrives, isLogicalDrive, extensionOnly, extension, &currentItem, &isSelected);
				if (displayLogicalDrives) {
					strcpy(newPath, "~\\");
				}
				ImGui::TextWrapped(currentItem.c_str());
			}
		}
		catch (fs::filesystem_error e)
		{
			path = fs::current_path();
			strcpy(newPath, "~\\");
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
				strcpy(newPath, "");
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
				active_tilemap_name_ = str;
				tilemap_list_.push_back(str);
				ResourceManager::LoadTexture(active_tilemap_name_.c_str(), sf::Color(186, 254, 202, 255), active_tilemap_name_);
				TilemapManager::AddTilemap(active_tilemap_name_, 16, 16, { 2.0f, 2.0f });
				file_browser_add_tiles_ = false;
			}
			currentItem = "";
			strcpy(newPath, "");
		}

		//PopUp if no file was selected
		ImGui::SetNextWindowSize(ImVec2(180, 86));
		if (ImGui::BeginPopupModal("##NoFileselected", nullptr, 0))
		{
			ImGui::TextWrapped("No File selected.");
			if (ImGui::Button("Ok", ImVec2(120, 0)))
			{
				currentItem = "";
				strcpy(newPath, "");
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
		strcpy(newPath, "");
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
		catch (fs::filesystem_error e)
		{
			// TODO: Catch error
		}
	}
}

GLvoid Gui::listDirectoryContent(fs::path path, bool displayLogicalDrives, bool isLogicalDrive, bool extensionOnly, fs::path extension, std::string* currentItem, bool isSelected)
{
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
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8, 0.6, 0.3, 0.8));

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
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8, 0.6, 0.3, 0.8));
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
