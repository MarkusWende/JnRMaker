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
 * @file tilemap.h
 * @brief This file contains the tilemap class. Represents a tilemap containing sf::Sprites.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#ifndef TILEMAP_H
#define TILEMAP_H

#include <map>
#include <memory>
#include <iostream>
#include <sstream>

#include <glm/vec2.hpp>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/OpenGL.hpp>

#include "resource_manager.h"

/**
 * @brief This class represents a tilemap containing sf::Sprites.
 */
class Tilemap
{
public:
  /**
	 * @brief Constructor for a tilemap.
	 * @param name The name of the tilemap.
	 * @param spriteWidth Width of a sprite in pixesl.
	 * @param spriteHeight height of a sprite in pixesl.
	 * @param scale The Scale of a sprite in the x and y axis.
	 */
  Tilemap(std::string name, GLuint spriteWidth, GLuint spriteHeight, glm::vec2 scale)
  {
    loadTilemap(name, spriteWidth, spriteHeight, scale);
  };

  /**
	 * @brief Constructor with no parameters for a tilemap.
	 */
  Tilemap ()
  {
    std::string name = "tiles_keen4";
    GLuint spriteWidth = 16;
    GLuint spriteHeight = 16;
    glm::vec2 scale = { 2.0f, 2.0f };
    loadTilemap(name, spriteWidth, spriteHeight, scale);
  };

  /**
	 * @brief Destructor.
	 */
  ~Tilemap() { };

  // Methods. Getters.
  /**
	 * @brief Get a sprite from the tilemap by name.
   * @param spriteName Name (key) of the sprite.
   * @return A sf::Sprite pointer.
	 */
  sf::Sprite* GetSprite(std::string spriteName) { return &tilemap_[spriteName]; };

  /**
	 * @brief Get the number of rows of the tilemap.
   * @return The number of rows as a GLuint.
	 */
  GLuint NumRows() { return num_rows_; };

  /**
	 * @brief Get the number of columns of the tilemap.
   * @return The number of columns as a GLuint.
	 */
  GLuint NumCols() { return num_cols_; };

private:
  /**
	 * @brief Load a Tilemap.
	 * @param name The name of the tilemap.
	 * @param spriteWidth Width of a sprite in pixesl.
	 * @param spriteHeight height of a sprite in pixesl.
	 * @param scale The Scale of a sprite in the x and y axis.
   * @return GLvoid.
	 */
  GLvoid loadTilemap(std::string name, GLuint spriteWidth, GLuint spriteHeight, glm::vec2 scale)
  {
    name_ = name;
    sprite_with_ = spriteWidth;
    sprite_height_ = spriteHeight;
    sprite_scale_ = scale;

    sf::Sprite spr;
    sf::Texture& tex = *ResourceManager::GetTexture(name);
    spr.setTexture(tex);
    spr.scale(scale.x, scale.y);

    glm::vec2 textureSize = { tex.getSize().x, tex.getSize().y };
    num_cols_ = textureSize.x / spriteWidth;
    num_rows_ = textureSize.y / spriteHeight;

    for (GLuint row = 0; row < num_rows_; row++) {
      for (GLuint col = 0; col < num_cols_; col++) {
        spr.setTextureRect(sf::IntRect(col*spriteWidth, row*spriteHeight, spriteWidth, spriteHeight));
        std::stringstream sprKey;
        sprKey << "r" << row << "c" << col;
        tilemap_.insert(std::make_pair(sprKey.str(), spr));
      }
    }
  }


  std::map<std::string, sf::Sprite> tilemap_;       /**< The tilemap is a map of sprites. */
  GLuint        sprite_with_;                       /**< Width of all the sprites in the map. */
  GLuint        sprite_height_;                     /**< Height of all the sprites in the map. */
  glm::vec2     sprite_scale_;                      /**< Scale of all the sprites. */
  std::string   name_;                              /**< Name of the tilemap. */
  GLuint        num_cols_;                          /**< Number of columns of the tilemap. */
  GLuint        num_rows_;                          /**< Number of rows of the tilemap. */

};

#endif
