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
 * @file gui.h
 * @brief This file contains the gui class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#ifndef GUI_H
#define GUI_H

#include <iostream>

#include "ResourceManagerOld.h"
#include "ProjectManager.h"
#include "Scene.h"
#include "imgui.h"
//#include "imgui/imgui-SFML.h"

//#include <SFML/OpenGL.hpp>
//#include <SFML/Graphics/Sprite.hpp>
//#include <SFML/Graphics/Texture.hpp>
//#include <SFML/Graphics/RenderWindow.hpp>
#include "UILogger.h"
#include "Resources.h"

#ifdef _WIN32
#define __STDC_WANT_LIB_EXT1__ 1
#include <cstring>
#include <filesystem>
#include <windows.h>
namespace fs = std::filesystem;
#endif // _WIN32
#ifdef __linux__
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#include <unistd.h>
#endif

/**
 * @brief Gui window struct. Represents a gui window size with pixel and percentage attributes.
 */
struct GuiWindow
{
    GLuint w;                     /**< Width in pixel of the gui window. */
    GLuint h;                     /**< Height in pixel of the gui window. */
    GLfloat wPercent;             /**< Width in percent [0.0f .. 1.0f] of the gui window. */
    GLfloat hPercent;             /**< Height in percent [0.0f .. 1.0f] of the gui window. */
};
#ifdef __EMSCRIPTEN__

#else

#endif

enum class gui_state_t {
	GUI_MINIMIZE = 3,
	GUI_MAXIMIZE = 2,
    GUI_ACTIVE = 1,
    GUI_CLOSE = 0
};


/**
 * @brief The gui class holds all ImGui related attributes andd methods. Like displaying gui windows, tabs, buttons etc.
 */
class Gui
{
public:
    Gui(std::shared_ptr<ILogger> logger, std::shared_ptr<Resources> resources);                    //!< constructor
    ~Gui();                                                     //!< destructor

    /**
	 * @brief Update the gui window dimensions if the application window size changed.
     * @param window Render window.
	 * @return GLvoid.
	 */
    GLvoid WindowUpdate(std::shared_ptr<Scene> scene, GLuint width, GLuint height);
    GLvoid WindowUpdate(std::shared_ptr<Scene> scene);

    /**
	 * @brief Draw gui windows.
	 * @return Bool Void.
	 */
	void Draw(std::shared_ptr<Scene> scene);

    /**
	 * @brief Draw gui windows.
	 * @return Bool Void.
	 */
	void DrawMenuMain(std::shared_ptr<Scene> scene);

    /**
	 * @brief Draw gui windows.
	 * @return Bool Void.
	 */
	void DrawWindowView(std::shared_ptr<Scene> scene);
    /**
	 * @brief Draw gui windows.
	 * @return Bool Void.
	 */
	void DrawWindowSettings(std::shared_ptr<Scene> scene);

    /**
	 * @brief Draw the message tab.
	 * @return Bool Void.
	 */
	void DrawTabWorld(std::shared_ptr<Scene> scene);

    /**
	 * @brief Draw the message tab.
	 * @return Bool Void.
	 */
	void DrawTabCamera(std::shared_ptr<Scene> scene);

    /**
	 * @brief Draw the message tab.
	 * @return Bool Void.
	 */
	void DrawTabSprites();

    /**
	 * @brief Draw the explorer window.
	 * @return Bool Void.
	 */
	void DrawWindowExplorer(std::shared_ptr<Scene> scene);

    /**
	 * @brief Draw the message tab.
	 * @return Bool Void.
	 */
	void DrawTabMessages();

    void DrawPopupMessages();

	void DrawWindowStatusbar();

    /**
	 * @brief Draw the message tab.
	 * @return Bool Void.
	 */
	void DrawTabTileExplorer(std::shared_ptr<Scene> scene);

    /**
	 * @brief Show a backend information window.
	 * @return Bool Void.
	 */
	void ShowBackendCheckerWindow();

    GLboolean IsOpen() { return state_ != gui_state_t::GUI_CLOSE ? 1 : 0; };
	GLboolean IsMinimized() { return state_ == gui_state_t::GUI_MINIMIZE ? 1 : 0; };
	GLboolean IsMaximized() { return state_ == gui_state_t::GUI_MAXIMIZE ? 1 : 0; };
    GLvoid Close() { state_ = gui_state_t::GUI_CLOSE; };

private:
    GLuint                      			width_;                     /**< Width of the application window. */
    GLuint                      			height_;                    /**< Height of the application window. */
    GuiWindow                   			window_scene_;              /**< Scene window object. Holds the dimensions of this window. */
    GuiWindow                   			window_messages_;           /**< Messages window object. Holds the dimensions of this window. */
    GuiWindow                   			window_sidebar_right_;      /**< Sidebar window object. Holds the dimensions of this window. */
    GLuint                      			main_menubar_height_;       /**< Main menu bar height. */
	GLfloat                      			statusbar_height_;       /**< Main menu bar height. */
    //std::string                 			active_tilemap_name_;      /**< Name of the tilemap which is currently displayed. */
    //std::string                 			active_sprite_name_;       /**< Name (key) of the sprite which is currently selected. */
    //std::vector<std::string>    			tilemap_list_;
    gui_state_t                 			state_;

    ImFont                      			*font_default_;
	ImFont                      			*icons_13_;
	ImFont                      			*icons_40_;

    bool                        			show_demo_imgui_;
    bool                        			show_backend_checker_show_;

	std::shared_ptr<UILogger> 				ui_logger_;
	std::shared_ptr<Resources>				resources_;

    /**
	    * @brief Initialize all gui related default attributes.
	    * @return GLvoid.
	    */
    GLvoid init();

    GLvoid drawBackendCheckerWindow();

    /**
	    * @brief Set the custom gui style attributes.
	    * @return GLvoid.
	    */
    GLvoid customGuiStyle();

#ifdef __EMSCRIPTEN__
    GLvoid fileBrowserAddTile();
    GLvoid listDirectoryContent();
#else
    GLvoid fileBrowserAddTile(std::shared_ptr<Scene> scene, fs::path& path, GLboolean extension_only, fs::path extension);
    GLvoid listDirectoryContent(fs::path path, bool displayLogicalDrives, bool isLogicalDrive, bool extensionOnly, fs::path extension, std::string* currentItem, bool isSelected);
#endif
    
};

#endif	// GUI_H
