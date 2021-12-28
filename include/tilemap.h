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

    Tilemap(std::string name, glm::vec2 spriteSize, glm::vec2 spriteScale)
    {
        name_ = name;
        num_cols_ = 0;
        num_rows_ = 0;
        sprite_size_ = spriteSize;
        sprite_scale_ = spriteScale;
        ResourceManager::CreateTextureAtlasEmpty(name, GL_TRUE, spriteSize, spriteScale);
    };

  /**
	 * @brief Constructor with no parameters for a tilemap.
	 */
    Tilemap ()
    {
        name_ = "default";
        sprite_size_ = { 16, 16 };
        sprite_scale_ = { 1.0f, 1.0f };
        num_cols_ = 0;
        num_rows_ = 0;
        //ResourceManager::CreateFramebuffer("default", 0, 0);
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

    GLvoid AddTile() {
        try
        {
            //Framebuffer fb = ResourceManager::GetFramebuffer(name_);
            //glm::vec2 size = fb.GetID()
        }
        catch(...)
        {
            std::stringstream msg;
            msg << "Tilemap is not expandable.";
            MessageManager::AddMessage(msg, message_t::ERROR_T);
        }
        
    }

    Texture2D* GetTile(std::string key)
    {
        return tilemap_.find(ResourceManager::getNameHash(name_,key))->second.get();
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
        Texture2D tex = ResourceManager::GetTexture(name_);

        num_rows_ = tex.Width / sprite_size_.x;
        num_cols_ = tex.Height / sprite_size_.y;

        for (GLuint i = 0; i < num_rows_; i++)
        {
            for (GLuint j = 0; j < num_cols_; j++)
            {
                unsigned char* data = (unsigned char*)malloc(sprite_size_.x * sprite_size_.y * 4);
                glGetTextureSubImage(   tex.ID,
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
                std::string hashKey = ResourceManager::getNameHash(name_, key.str());
                
                tilemap_.insert(std::make_pair(hashKey, new Texture2D()));
                tilemap_.find(hashKey)->second->Generate(sprite_size_.x, sprite_size_.y, data);
            }
        }

       /*  for (auto const& [key, val] : tilemap_)
        {
            std::stringstream msg;
            msg << "key: " << key;
            MessageManager::AddMessage(msg, message_t::INFO);
        } */
        
    }

    /* GLvoid createTexAtlas(Texture2D& tex)
    {
        tex.
        unsigned char* data = ...; // 16x16 texture atlas image data
        int tileW = ...;           // number of pixels in a row of 1 tile
        int tileH = ...;           // number of pixels in a column of 1 tile
        int channels = 4;          // 4 for RGBA

        int tilesX = 16;
        int tilesY = 16;
        int imageCount = tilesX * tilesY;

        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, this->Internal_Format, 
                    tileW, tileH, imageCount, 0,
                    this->Image_Format, GL_UNSIGNED_BYTE, nullptr);

        std::vector<unsigned char> tile(tileW * tileH * channels);
        int tileSizeX = tileW * channels;
        int rowLen    = tilesX * tileSizeX;

        for (int iy = 0; iy < tilesY; ++ iy)
        {
            for (int ix = 0; ix < tilesX; ++ ix)
            {
                unsigned char *ptr = data + iy*rowLen + ix*tileSizeX;
                for (int row = 0; row < tileH; ++ row)
                    std::copy(ptr + row*rowLen, ptr + row*rowLen + tileSizeX,
                            tile.begin() + row*tileSizeX);


                int i = iy * tilesX + ix;
                glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                    0, 0, i,
                    tileW, tileH, 1,
                    this->Image_Format, GL_UNSIGNED_BYTE, tile.data());
            }
        }
    }
 */
    std::map<std::string, std::shared_ptr<Texture2D>> tilemap_;

    glm::vec2     sprite_size_;                       /**< Width of all the sprites in the map. */
    glm::vec2     sprite_scale_;                      /**< Scale of all the sprites. */
    std::string   name_;                              /**< Name of the tilemap. */
    GLuint        num_cols_;                          /**< Number of columns of the tilemap. */
    GLuint        num_rows_;                          /**< Number of rows of the tilemap. */

};

#endif
