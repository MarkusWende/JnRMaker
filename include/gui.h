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

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "resource_manager.h"
#include "project_manager.h"
#include "message_manager.h"
#include "tilemap_manager.h"
#include "scene.h"


/**
 * @brief Gui window struct. Represents a gui window size with pixel and percentage attributes.
 */
struct GuiWindow {
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(w);
        ar & BOOST_SERIALIZATION_NVP(h);
        ar & BOOST_SERIALIZATION_NVP(wPercent);
        ar & BOOST_SERIALIZATION_NVP(hPercent);
    }

    GLuint w;                     /**< Width in pixel of the gui window. */
    GLuint h;                     /**< Height in pixel of the gui window. */
    GLfloat wPercent;             /**< Width in percent [0.0f .. 1.0f] of the gui window. */
    GLfloat hPercent;             /**< Height in percent [0.0f .. 1.0f] of the gui window. */
};

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
  GLvoid Update(GLuint width, GLuint height, Scene& scene);

  /**
	 * @brief Render all gui related entities, that are displayed in the application.
	 * @return GLvoid.
	 */
  GLvoid Render(Scene &scene);

  GLboolean IsOpen() { return state_ == gui_state_t::GUI_ACTIVE ? 1 : 0; };
  GLvoid Close() { state_ = gui_state_t::GUI_CLOSE; };

private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(width_);
        ar & BOOST_SERIALIZATION_NVP(height_);
        ar & BOOST_SERIALIZATION_NVP(window_scene_);
        ar & BOOST_SERIALIZATION_NVP(window_messages_);
        ar & BOOST_SERIALIZATION_NVP(window_sidebar_right_);
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
};

#endif	// GUI_H
