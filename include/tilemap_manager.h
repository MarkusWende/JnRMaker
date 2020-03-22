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
 * @file tilemap_manager.h
 * @brief This file contains the singleton tilemap manager class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#ifndef TILEMAP_MANAGER_H
#define TILEMAP_MANAGER_H

#include <map>
#include <memory>
#include <iostream>

#include <glm/vec2.hpp>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/OpenGL.hpp>

#include "resource_manager.h"
#include "tilemap.h"

/**
 * @brief The sindgleton tilemap manager class manages all tilemaps use in the application.
 */
class TilemapManager
{
public:
  static std::map<std::string, Tilemap> Tilemaps;                                 /**< Contains all loaded tilemaps. */

  /**
	 * @brief Add a new tilemap to the map of tilemaps.
   * @param name The name of the tilemap.
	 * @param spriteWidth The global width of all the sprites in pixesl.
	 * @param spriteHeight The global height of all the sprites in pixesl.
	 * @param scale The global scale of aall the sprite along the x and y axis.
   * @return GLvoid.
	 */
  static GLvoid AddTilemap(std::string name, GLuint spriteWidth, GLuint spriteHeight, glm::vec2 scale);

  /**
	 * @brief Get a tilemap by name.
	 * @param name The name of the tilemap.
	 * @return A pointer to the tilemap.
	 */
  static Tilemap* GetTilemap(std::string name);

private:
  // Private constructor, that is we do not want any actual tilemap manager objects. Its members and functions should be publicly available (static).
  TilemapManager() { }
  ~TilemapManager() { }

};

#endif
