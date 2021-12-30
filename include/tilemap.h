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
    Tilemap(std::string name, glm::vec2 spriteSize, glm::vec2 spriteScale, std::string file)
    {
        ResourceManager::LoadTexture(file.c_str(), GL_TRUE, name);
        name_ = name;
        num_cols_ = 0;
        num_rows_ = 0;
        sprite_size_ = spriteSize;
        sprite_scale_ = spriteScale;
        tilemap_id_max_ = 0;

        // Load image
        int width, height;
        unsigned char* image = SOIL_load_image(file.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
        
        if (NULL == image)
        {
            std::stringstream msg;
            msg << SOIL_last_result() << ": " << file;
            MessageManager::AddMessage(msg, message_t::ERROR_T);
        }

        // Now generate texture
        tilemap_tex_array_.Generate(width, height, image, spriteSize);

        //ResourceManager::CreateTextureAtlasFromFile(name_, GL_TRUE, sprite_size_, sprite_scale_, file.c_str());
        loadTilemapFromTexture();
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
        tilemap_id_max_ = 0;
        //ResourceManager::CreateTextureAtlasEmpty(name_, GL_TRUE, sprite_size_, sprite_scale_);
        //ResourceManager::CreateFramebuffer("default", 0, 0);
    };

    /**
	 * @brief Destructor.
	 */
    ~Tilemap()
    {
        tilemap_textures_.clear();
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
    TextureArray GetTexArray() { return tilemap_tex_array_; };

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

    GLvoid AddTile(const std::string key, GLuint texID)
    {
        try
        {
            if (num_rows_ == 0)
                num_rows_ = 1;
            GLuint tilemapSize = (GLuint)tilemap_textures_.size();
            unsigned char* data = (unsigned char*)malloc(sprite_size_.x * sprite_size_.y * 4);
            glGetTextureSubImage(   texID,
                                    0,
                                    0,
                                    0,
                                    0,
                                    sprite_size_.x,
                                    sprite_size_.y,
                                    1,
                                    GL_RGBA,
                                    GL_UNSIGNED_BYTE,
                                    sprite_size_.x * sprite_size_.y * 4,
                                    data);

            /* std::stringstream key;
            key << "r" << 0 << "c" << tilemapSize;
            std::string hashKey = ResourceManager::getNameHash(name_, key.str()); */
            
            if (tilemap_textures_.find(key) == tilemap_textures_.end())
            {
                tilemap_ids_.insert(std::make_pair(tilemap_id_max_, key));
                tilemap_id_max_++;
                tilemap_textures_.insert(std::make_pair(key, new Texture2D()));
                tilemap_textures_.find(key)->second->Generate(sprite_size_.x, sprite_size_.y, data);
                num_cols_ = tilemapSize + 1;
                createTextureArray();
            }
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
        return tilemap_textures_.find(key)->second.get();
        //return &ResourceManager::GetTexture(name_);
    };

    GLuint GetTileID(std::string key)
    {
        GLuint tileID = 0;
        for (auto const& [id, hash] : tilemap_ids_)
		{
            if (hash.compare(key) == 0)
            {
                tileID = id;
                break;
            }
        }

        return tileID;
    }

    GLboolean HashExists(std::string key)
    {
        if (tilemap_textures_.find(key.c_str()) != tilemap_textures_.end())
        {
            return true;
        }
        
        return false;
    }

    std::vector<std::string> GetHashs()
    {
        std::vector<std::string> keys;
        for (auto const& [key, val] : tilemap_textures_)
		{
            keys.push_back(key);
        }
        return keys;
    }

private:
  /**
	 * @brief Load a Tilemap.
	 * @param name The name of the tilemap.
	 * @param spriteWidth Width of a sprite in pixesl.
	 * @param spriteHeight height of a sprite in pixesl.
	 * @param scale The Scale of a sprite in the x and y axis.
   * @return GLvoid.
	 */
    GLvoid loadTilemapFromTexture()
    {
        Texture2D tex = ResourceManager::GetTexture(name_);

        num_rows_ = tex.Height / sprite_size_.y;
        num_cols_ = tex.Width / sprite_size_.x;

        std::stringstream msg;
        msg << "rows: " << num_rows_ << "\tcols: " << num_cols_;
        MessageManager::AddMessage(msg, message_t::INFO);

        for (GLuint i = 0; i < num_cols_; i++)
        {
            for (GLuint j = 0; j < num_rows_; j++)
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
                key << "r" << j << "c" << i;
                std::string hashKey = ResourceManager::getNameHash(name_, key.str());
                
                tilemap_ids_.insert(std::make_pair(tilemap_id_max_, hashKey));
                tilemap_textures_.insert(std::make_pair(hashKey, new Texture2D()));
                tilemap_textures_.find(hashKey)->second->Generate(sprite_size_.x, sprite_size_.y, data);

                tilemap_id_max_++;
            }
        }

       /*  for (auto const& [key, val] : tilemap_)
        {
            std::stringstream msg;
            msg << "key: " << key;
            MessageManager::AddMessage(msg, message_t::INFO);
        } */
        
    }

    GLvoid createTextureArray()
    {

        int channels = 4;
        int ix = 0;
        /*
        int tilesX = tilemap_textures_.size();
        int rowLen = (int)sprite_size_.x * channels;
        int channelSize = sprite_size_.x * sprite_size_.y * tilemap_textures_.size(); */
        int spriteDataSize = (int)sprite_size_.x * (int)sprite_size_.y * channels;
        //unsigned char* data = (unsigned char*)malloc(sprite_size_.x * sprite_size_.y * channels * tilemap_.size());
        std::vector<unsigned char> data(spriteDataSize * tilemap_textures_.size());
        
        for (auto const& [key, val] : tilemap_ids_)
        {
            GLuint texID = tilemap_textures_.find(val)->second.get()->ID;
            //unsigned char* subData = (unsigned char*)malloc(spriteDataSize);
            std::vector<unsigned char> subData(spriteDataSize);
            glGetTextureSubImage(   texID,
                                        0,
                                        0,
                                        0,
                                        0,
                                        sprite_size_.x,
                                        sprite_size_.y,
                                        1,
                                        GL_RGBA,
                                        GL_UNSIGNED_BYTE,
                                        spriteDataSize,
                                        subData.data());
            
            /* std::stringstream filename;
            filename << "test-" << key << ".bmp";
            SOIL_save_image(    filename.str().c_str(),
                                SOIL_SAVE_TYPE_BMP,
                                sprite_size_.x,
                                sprite_size_.y,
                                4,
                                subData.data()); */
			//std::copy(subData.begin(), subData.end(), data.begin() + ix * spriteDataSize);
            for (int row = 0; row < sprite_size_.y; ++row)
				std::copy(  subData.begin() + row * 64,
                            subData.begin() + row * 64 + 64,
                            data.begin() + ix * 64 + row * 64 * tilemap_ids_.size()
                        );
            //data.insert( data.end(), subData.begin(), subData.end() );
            ix++;
        }

        /* int counter = 0;
        for (size_t i = 0; i < data.size();)
        {
            if (i < 64)
            {
                data.at(i) = 255;
                data.at(i+1) = 0;
                data.at(i+2) = 0;
                data.at(i+3) = 255;
            } else
            {
                data.at(i) = 200;
                data.at(i+1) = 255;
                data.at(i+2) = 0;
                data.at(i+3) = 255;
            }  
            i = i + 4;
            counter++;
        } */

        //SOIL_save_image("test.bmp", SOIL_SAVE_TYPE_BMP, sprite_size_.x * tilemap_textures_.size(), sprite_size_.y, 4, data.data());
        tilemap_tex_array_.Generate(sprite_size_.x * tilemap_textures_.size(), sprite_size_.y, data.data(), sprite_size_);
    }
 
    std::map<std::string, std::shared_ptr<Texture2D>>   tilemap_textures_;
    std::map<GLuint, std::string>                       tilemap_ids_;
    TextureArray    tilemap_tex_array_;
    GLuint          tilemap_id_max_;

    glm::vec2       sprite_size_;                       /**< Width of all the sprites in the map. */
    glm::vec2       sprite_scale_;                      /**< Scale of all the sprites. */
    std::string     name_;                              /**< Name of the tilemap. */
    GLuint          num_cols_;                          /**< Number of columns of the tilemap. */
    GLuint          num_rows_;                          /**< Number of rows of the tilemap. */
};

#endif
