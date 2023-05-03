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

#include "ResourceManager.h"
//#include "EntitySolid.h"
//#include "SolidSprite.h"

#ifdef __EMSCRIPTEN
#include <GL/glext.h>
#endif

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
    Tilemap(std::string name, glm::vec2 spriteSize, glm::vec2 spriteScale, std::string file);
    Tilemap(std::string name, glm::vec2 spriteSize, glm::vec2 spriteScale, const unsigned char *data, int size);
    Tilemap();
    ~Tilemap();

    // Methods. Getters.
    /**
	 * @brief Get a sprite from the tilemap by name.
     * @param spriteName Name (key) of the sprite.
     * @return A sf::Sprite pointer.
	 */
    //sf::Sprite* GetSprite(std::string spriteName) { return &tilemap_[spriteName]; };

    glm::vec2 GetSpriteSize() { return { sprite_size_.x, sprite_size_ .y}; };
    glm::vec2 GetSpriteScale() { return {sprite_scale_.x, sprite_scale_.y}; };
    //TextureArray GetTexArray() { return tilemap_tex_array_; };
    TextureArray GetTexArray() { return ResourceManager::GetTextureArray(name_); };

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

    GLvoid AddTile(const std::string key, GLuint texID);

    //Texture2D* GetTile(std::string key) { return tilemap_textures_.find(key)->second.get(); };
    Texture2D GetTile(std::string key) { return ResourceManager::GetTexture(key); };

    GLuint GetTileID(std::string key);

    GLboolean HashExists(std::string key);

    std::vector<std::string> GetHashs();

    GLvoid Delete();

private:
  /**
	 * @brief Load a Tilemap.
	 * @param name The name of the tilemap.
	 * @param spriteWidth Width of a sprite in pixesl.
	 * @param spriteHeight height of a sprite in pixesl.
	 * @param scale The Scale of a sprite in the x and y axis.
   * @return GLvoid.
	 */
    //GLvoid loadTilemapFromTexture();

    //GLvoid createTextureArray();
 
    //std::map<std::string, std::shared_ptr<Texture2D>>   tilemap_textures_;
    std::map<GLuint, std::string>                       tilemap_ids_;
    //TextureArray    tilemap_tex_array_;
    GLuint          tilemap_id_max_;

    glm::vec2       sprite_size_;                       /**< Width of all the sprites in the map. */
    glm::vec2       sprite_scale_;                      /**< Scale of all the sprites. */
    std::string     name_;                              /**< Name of the tilemap. */
    GLuint          num_cols_;                          /**< Number of columns of the tilemap. */
    GLuint          num_rows_;                          /**< Number of rows of the tilemap. */
};

#endif