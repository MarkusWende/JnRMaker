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

//#include <SFML/Graphics/Image.hpp>
//#include <SFML/Graphics/Texture.hpp>
//#include <SFML/Graphics/RenderTexture.hpp>
//#include <SFML/Graphics/Color.hpp>
//#include <SFML/Graphics/Sprite.hpp>
//#include <SFML/OpenGL.hpp>

#include "resource_manager.h"
//#include "entity_solid.h"
//#include "solid_sprite.h"

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
    Tilemap(std::string name, glm::vec2 spriteSize, glm::vec2 SpriteScale, std::string file)
    {
        ResourceManager::LoadTexture(file.c_str(), GL_TRUE, name);
        name_ = name;
        sprite_size_ = spriteSize;
        sprite_scale_ = SpriteScale;
        loadTilemap(file);
    };

    Tilemap(std::string name, glm::vec2 spriteSize, glm::vec2 SpriteScale)
    {
        name_ = name;
        sprite_size_ = spriteSize;
        sprite_scale_ = SpriteScale;
    };

  /**
	 * @brief Constructor with no parameters for a tilemap.
	 */
    Tilemap ()
    {
        name_ = "default";
        sprite_size_ = { 16, 16 };;
        sprite_scale_ = { 1.0f, 1.0f };
    };

  /**
	 * @brief Destructor.
	 */
  ~Tilemap()
  {
      tilemap_.clear();
  };

  // Methods. Getters.
  /**
	 * @brief Get a sprite from the tilemap by name.
   * @param spriteName Name (key) of the sprite.
   * @return A sf::Sprite pointer.
	 */
  //sf::Sprite* GetSprite(std::string spriteName) { return &tilemap_[spriteName]; };

  glm::vec2 GetSpriteSize() { return { sprite_size_.x, sprite_size_ .y}; };
  glm::vec2 GetSpriteScale() { return {sprite_scale_.x, sprite_scale_.y}; };

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

  GLvoid AddTile();

    Texture2D* GetTile(std::string key)
    {
        return tilemap_.find(key)->second.get();
        //return &ResourceManager::GetTexture(name_);
    };

private:
  /**
	 * @brief Load a Tilemap.
	 * @param name The name of the tilemap.
	 * @param spriteWidth Width of a sprite in pixesl.
	 * @param spriteHeight height of a sprite in pixesl.
	 * @param scale The Scale of a sprite in the x and y axis.
   * @return GLvoid.
	 */
  
    GLvoid loadTilemap(std::string file)
    {
        Texture2D* tex = &ResourceManager::GetTexture(name_);

        num_rows_ = tex->Width / sprite_size_.x;
        num_cols_ = tex->Height / sprite_size_.y;

        for (GLuint i = 0; i < num_rows_; i++)
        {
            for (GLuint j = 0; j < num_cols_; j++)
            {
                unsigned char* data = (unsigned char*)malloc(sprite_size_.x * sprite_size_.y * 4);
                glGetTextureSubImage(   tex->ID,
                                        0, 
                                        i * sprite_size_.x,
                                        j * sprite_size_.y,
                                        0, 
                                        sprite_size_.x, 
                                        sprite_size_.y, 
                                        1, 
                                        GL_RGBA, 
                                        GL_UNSIGNED_BYTE, 
                                        sprite_size_.x * sprite_size_.y * 4, 
                                        data);

                std::stringstream key;
                key << "r" << i << "c" << j;
                ResourceManager::getNameHash(name_, key.str());
                tilemap_.insert(std::make_pair(key.str(), new Texture2D()));
                tilemap_.find(key.str())->second->Generate(sprite_size_.x, sprite_size_.y, data);
            }
        }
    }

    std::map<std::string, std::shared_ptr<Texture2D>> tilemap_;

  glm::vec2     sprite_size_;                       /**< Width of all the sprites in the map. */
  glm::vec2     sprite_scale_;                      /**< Scale of all the sprites. */
  std::string   name_;                              /**< Name of the tilemap. */
  GLuint        num_cols_;                          /**< Number of columns of the tilemap. */
  GLuint        num_rows_;                          /**< Number of rows of the tilemap. */

};

#endif
