#include <iostream>

#include "../include/gui.h"
#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui-SFML.h"
#include "../include/resource_manager.h"
#include "../include/message_manager.h"

// PUBLIC
Gui::Gui()
{
	init_();
	customGuiStyle_();
}

Gui::~Gui()
{

}


void Gui::Update(GLuint width, GLuint height)
{
	// Check if application window size has been changed
	if (width_ != width || height_ != height) {
		width_ = width;
		height_ = height;

		windowScene_.w = width_ * windowScene_.wPercent;
		windowScene_.h = height_ * windowScene_.hPercent;

		windowMessages_.w = width_ * windowMessages_.wPercent;
		windowMessages_.h = height_ * windowMessages_.hPercent;

		windowSideBarRight_.w = width_ * windowSideBarRight_.wPercent;
		windowSideBarRight_.h = height_ * windowSideBarRight_.hPercent;
	}

	ImGuiStyle& style = ImGui::GetStyle();
	style.DisplaySafeAreaPadding = ImVec2(0.0f, mainMenuBarHeight_ - 23);

	// Check if mouse is over scene window
	//if (ImGui::IsWindowHovered())
	//	scene.mouseOverScene = true;
	//else
	//	scene.mouseOverScene = false;
}

void Gui::Render()
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
		ImGui::SetNextWindowSize(ImVec2(windowScene_.w, windowScene_.h - mainMenuBarHeight_));
		ImGui::SetNextWindowPos(ImVec2(0, mainMenuBarHeight_));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
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

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

	// Messages Window
	{
		ImGui::SetNextWindowSize(ImVec2(windowMessages_.w, windowMessages_.h));
		ImGui::SetNextWindowPos(ImVec2(0, windowScene_.h));
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
				if (it->type == message_t::ERROR)
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
		ImGui::SetNextWindowSize(ImVec2(windowSideBarRight_.w, windowSideBarRight_.h - mainMenuBarHeight_));
		ImGui::SetNextWindowPos(ImVec2(windowScene_.w, mainMenuBarHeight_));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
		ImGui::Begin("SideBarRight", NULL, 	ImGuiWindowFlags_NoTitleBar |
																				ImGuiWindowFlags_NoResize |
																				ImGuiWindowFlags_NoMove |
																				ImGuiWindowFlags_NoCollapse |
																				ImGuiWindowFlags_HorizontalScrollbar);
		//ImGui::GetWindowDrawList()->AddImage((ImTextureID)ResourceManager::GetFramebuffer("imguiScene").texID, ImVec2(0, 25), ImVec2(sceneWidth, sceneHeight + 25));
		//ImGui::Image(ResourceManager::GetTexture("face"));
		sf::Sprite spr;
		sf::Texture& tex = *ResourceManager::GetTexture("tiles_keen4");
		spr.setTexture(tex);
		spr.scale(2.0f, 2.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.4));
		static GLuint PixelPerTile = 16;
		ImVec2 textureSize = tex.getSize();
		GLuint numberOfColumns = textureSize.x / PixelPerTile;
		//GLuint numberOfColumns = 10;
		GLuint numberOfRows = textureSize.y / PixelPerTile;
		//GLuint numberOfRows = 12;
		for (GLuint row = 0; row < numberOfRows; row++) {
			for (GLuint col = 0; col < numberOfColumns; col++) {
				spr.setTextureRect(sf::IntRect(col*PixelPerTile,row*PixelPerTile,PixelPerTile,PixelPerTile));
				ImGui::ImageButton(spr, 1, sf::Color(0, 0, 0, 0), sf::Color(200, 200, 200, 255));
				if (col < numberOfColumns-1)
					ImGui::SameLine();
			}
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

	// Imgui Demo Window
  ImGui::ShowDemoWindow();
}


// PRIVATE
void Gui::init_()
{
	windowScene_.wPercent = 0.8f;
	windowScene_.hPercent = 0.9f;

	windowMessages_.wPercent = 1.0f;
	windowMessages_.hPercent = 0.1f;

	windowSideBarRight_.wPercent = 0.2f;
	windowSideBarRight_.hPercent = 0.9f;

	mainMenuBarHeight_ = 0 + 23;
}

void Gui::customGuiStyle_()
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
	io.Fonts->AddFontFromFileTTF("../assets/fonts/ProggyTiny.ttf", 6.0f);
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
