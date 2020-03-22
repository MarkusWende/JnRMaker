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

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui-SFML.h"
#include "../include/resource_manager.h"
#include "../include/message_manager.h"
#include "../include/tilemap_manager.h"
#include "../include/scene.h"


/**
 * @brief Gui window struct. Represents a gui window size with pixel and percentage attributes.
 */
struct GuiWindow {
  GLuint w;                     /**< Width in pixel of the gui window. */
  GLuint h;                     /**< Height in pixel of the gui window. */
  GLfloat wPercent;             /**< Width in percent [0.0f .. 1.0f] of the gui window. */
  GLfloat hPercent;             /**< Height in percent [0.0f .. 1.0f] of the gui window. */
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
   * @param width The application width.
   * @param height The application height.
	 * @return GLvoid.
	 */
  GLvoid Update(GLuint width, GLuint height, Scene &scene);

  /**
	 * @brief Render all gui related entities, that are displayed in the application.
	 * @return GLvoid.
	 */
  GLvoid Render();

private:
  GLuint          width_;                     /**< Width of the application window. */
  GLuint          height_;                    /**< Height of the application window. */
  GuiWindow       window_scene_;              /**< Scene window object. Holds the dimensions of this window. */
  GuiWindow       window_messages_;           /**< Messages window object. Holds the dimensions of this window. */
  GuiWindow       window_sidebar_right_;      /**< Sidebar window object. Holds the dimensions of this window. */
  GLuint          main_menubar_height_;       /**< Main menu bar height. */
  std::string     current_tilemap_name_;      /**< Name of the tilemap which is currently displayed. */

  /**
	 * @brief Initialize all gui related default attributes.
	 * @return GLvoid.
	 */
  GLvoid init_();

  /**
	 * @brief Set the custom gui style attributes.
	 * @return GLvoid.
	 */
  GLvoid customGuiStyle_();
};

#endif	// GUI_H
