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
#include <filesystem>

#include "resource_manager.h"
#include "project_manager.h"
#include "message_manager.h"
#include "tilemap_manager.h"
#include "scene.h"

#include "imgui/imgui.h"
//#include "imgui/imgui-SFML.h"

//#include <SFML/OpenGL.hpp>
//#include <SFML/Graphics/Sprite.hpp>
//#include <SFML/Graphics/Texture.hpp>
//#include <SFML/Graphics/RenderWindow.hpp>


#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;


/**
 * @brief Gui window struct. Represents a gui window size with pixel and percentage attributes.
 */
struct GuiWindow {
    template <class Archive>
    void serialize( Archive & ar, std::uint32_t const version )
    {
        ar( CEREAL_NVP(wPercent), CEREAL_NVP(hPercent) );
    }

    GLuint w;                     /**< Width in pixel of the gui window. */
    GLuint h;                     /**< Height in pixel of the gui window. */
    GLdouble wPercent;             /**< Width in percent [0.0f .. 1.0f] of the gui window. */
    GLdouble hPercent;             /**< Height in percent [0.0f .. 1.0f] of the gui window. */
};
CEREAL_CLASS_VERSION(GuiWindow, 1);

enum class gui_state_t {
    GUI_ACTIVE = 1,
    GUI_CLOSE = 0
};


/**
 * @brief The gui class holds all ImGui related attributes andd methods. Like displaying gui windows, tabs, buttons etc.
 */
class Gui
{
public:
    Gui();                                                      //!< constructor
    ~Gui();                                                     //!< destructor

    /**
	 * @brief Update the gui window dimensions if the application window size changed.
     * @param window Render window.
	 * @return GLvoid.
	 */
    GLvoid WindowUpdate(GLuint width, GLuint height);
    GLvoid WindowUpdate();

    /**
	 * @brief Render all gui related entities, that are displayed in the application.
	 * @return GLvoid.
	 */
    //GLvoid Render(Scene &scene);
    GLvoid Render(Scene& scene);

    GLboolean IsOpen() { return state_ == gui_state_t::GUI_ACTIVE ? 1 : 0; };
    GLvoid Close() { state_ = gui_state_t::GUI_CLOSE; };

private:
    friend class cereal::access;
    template <class Archive>
    void save(Archive& ar, std::uint32_t const version) const
    {
        ar( CEREAL_NVP(window_scene_) );
        ar( CEREAL_NVP(active_tilemap_name_), CEREAL_NVP(active_sprite_name_));
        ar( CEREAL_NVP(tilemap_list_) );
    }

    template <class Archive>
    void load(Archive& ar, std::uint32_t const version)
    {
        ar(window_scene_);
        WindowUpdate();
        ar(active_tilemap_name_, active_sprite_name_);
        ar(tilemap_list_);
        for (auto const& tilemapItem : tilemap_list_)
        {
            ResourceManager::LoadTexture(tilemapItem.c_str(), GL_TRUE, tilemapItem);
            TilemapManager::AddTilemap(tilemapItem, 16, 16, { 2.0f, 2.0f });
        }
    }

    GLuint                      width_;                     /**< Width of the application window. */
    GLuint                      height_;                    /**< Height of the application window. */
    GuiWindow                   window_scene_;              /**< Scene window object. Holds the dimensions of this window. */
    GuiWindow                   window_messages_;           /**< Messages window object. Holds the dimensions of this window. */
    GuiWindow                   window_sidebar_right_;      /**< Sidebar window object. Holds the dimensions of this window. */
    GLuint                      main_menubar_height_;       /**< Main menu bar height. */
    std::string                 active_tilemap_name_;      /**< Name of the tilemap which is currently displayed. */
    std::string                 active_sprite_name_;       /**< Name (key) of the sprite which is currently selected. */
    std::vector<std::string>    tilemap_list_;
    gui_state_t                 state_;
    fs::path					root_file_path_;
    GLboolean                   file_browser_add_tiles_;
    GLboolean                   update_sence_;

    /**
	    * @brief Initialize all gui related default attributes.
	    * @return GLvoid.
	    */
    GLvoid init();

    /**
	    * @brief Set the custom gui style attributes.
	    * @return GLvoid.
	    */
    GLvoid customGuiStyle();

    GLvoid fileBrowserAddTile(fs::path& path, GLboolean extension_only, fs::path extension);
    GLvoid listDirectoryContent(fs::path path, bool displayLogicalDrives, bool isLogicalDrive, bool extensionOnly, fs::path extension, std::string* currentItem, bool isSelected);
};
CEREAL_CLASS_VERSION(Gui, 1);

#endif	// GUI_H
