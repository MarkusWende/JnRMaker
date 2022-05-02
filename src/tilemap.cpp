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
 * @file tilemap.cpp
 * @brief This file contains the tilemap class. Represents a tilemap containing sf::Sprites.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#include "tilemap.h"
#include "stb_image.h"

Tilemap::Tilemap(std::string name, glm::vec2 spriteSize, glm::vec2 spriteScale, std::string file)
{
    ResourceManager::LoadTexture(file.c_str(), GL_TRUE, name);
    name_ = name;
    num_cols_ = 0;
    num_rows_ = 0;
    sprite_size_ = spriteSize;
    sprite_scale_ = spriteScale;
    tilemap_id_max_ = 0;

    // Load image
    int width, height, channels;
    //unsigned char* image = SOIL_load_image(file.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
    unsigned char* image = stbi_load(file.c_str(), &width, &height, &channels, 4);
    
    if (NULL == image)
    {
        std::stringstream msg;
        msg << stbi_failure_reason() << ": " << file;
        MessageManager::AddMessage(msg, message_t::ERROR_T);
    }

    // Now generate texture
    tilemap_tex_array_.Generate(width, height, image, spriteSize);

    //ResourceManager::CreateTextureAtlasFromFile(name_, GL_TRUE, sprite_size_, sprite_scale_, file.c_str());
    loadTilemapFromTexture();

    stbi_image_free(image);
};

/**
 * @brief Constructor with no parameters for a tilemap.
 */
Tilemap::Tilemap ()
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


Tilemap::~Tilemap()
{
    tilemap_textures_.clear();
};

GLvoid Tilemap::AddTile(const std::string key, GLuint texID)
{
    try
    {
        if (num_rows_ == 0)
            num_rows_ = 1;
        GLuint tilemapSize = (GLuint)tilemap_textures_.size();
        unsigned char* data = (unsigned char*)malloc(sprite_size_.x * sprite_size_.y * 4);

        /* std::stringstream key;
        key << "r" << 0 << "c" << tilemapSize;
        std::string hashKey = ResourceManager::getNameHash(name_, key.str()); */
        glBindTexture(GL_TEXTURE_2D, texID);
#ifdef __EMSCRIPTEN__
        
        glTexSubImage2D(	GL_TEXTURE_2D,
                            0,
                            0,
                            0,
                            sprite_size_.x,
                            sprite_size_.y,
                            GL_RGBA,
                            GL_UNSIGNED_BYTE,
                            data);
        
#else
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
#endif
        glBindTexture(GL_TEXTURE_2D, 0);
        if (tilemap_textures_.find(key) == tilemap_textures_.end())
        {
            tilemap_ids_.insert(std::make_pair(tilemap_id_max_, key));
            tilemap_id_max_++;
            Texture2D tex;
            tex.Generate(sprite_size_.x, sprite_size_.y, data);
            tilemap_textures_[key] = tex;
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

GLuint Tilemap::GetTileID(std::string key)
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

GLboolean Tilemap::HashExists(std::string key)
{
    if (tilemap_textures_.find(key.c_str()) != tilemap_textures_.end())
    {
        return true;
    }
    
    return false;
}

std::vector<std::string> Tilemap::GetHashs()
{
    std::vector<std::string> keys;
    for (auto const& [key, val] : tilemap_textures_)
    {
        keys.push_back(key);
    }
    return keys;
}

GLvoid Tilemap::loadTilemapFromTexture()
{
    Texture2D tex = ResourceManager::GetTexture(name_);

    num_rows_ = tex.Height / sprite_size_.y;
    num_cols_ = tex.Width / sprite_size_.x;

    /* std::stringstream msg;
    msg << "rows: " << num_rows_ << "\tcols: " << num_cols_;
    MessageManager::AddMessage(msg, message_t::INFO); */

    for (GLuint i = 0; i < num_cols_; i++)
    {
        for (GLuint j = 0; j < num_rows_; j++)
        {
            unsigned char* data = (unsigned char*)malloc(sprite_size_.x * sprite_size_.y * 4);
            tex.Bind();
#ifdef __EMSCRIPTEN__
            glTexSubImage2D(	GL_TEXTURE_2D,
                                0,
                                i * sprite_size_.x,
                                j * sprite_size_.y,
                                sprite_size_.x,
                                sprite_size_.y,
                                GL_RGBA,
                                GL_UNSIGNED_BYTE,
                                data);
#else
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
#endif
            glBindTexture(GL_TEXTURE_2D, 0);
            std::stringstream key;
            key << "r" << j << "c" << i;
            //key << "default_border_" << sprite_size_.x << "x" << sprite_size_.y;
            std::string hashKey = ResourceManager::getNameHash(name_, key.str());
            
            tilemap_ids_[tilemap_id_max_] = hashKey;
            Texture2D newTex;
            newTex.Generate(sprite_size_.x, sprite_size_.y, data);
            tilemap_textures_[hashKey] = newTex;

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

GLvoid Tilemap::createTextureArray()
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
        GLuint texID = ResourceManager::GetTexture(val).ID;
        std::stringstream msg;
        msg << "val: " << val << "\tID: " << texID;
        MessageManager::AddMessage(msg, message_t::WARNING);
        glBindTexture(GL_TEXTURE_2D, texID);
        std::vector<unsigned char> subData(spriteDataSize);

        /* std::stringstream filename;
        filename << "test-" << key << ".bmp";
        SOIL_save_image(    filename.str().c_str(),
                            SOIL_SAVE_TYPE_BMP,
                            sprite_size_.x,
                            sprite_size_.y,
                            4,
                            subData.data()); */
        //std::copy(subData.begin(), subData.end(), data.begin() + ix * spriteDataSize);
        //data.insert( data.end(), subData.begin(), subData.end() );

#ifdef __EMSCRIPTEN__
        //unsigned char* subData = (unsigned char*)malloc(spriteDataSize);
        //std::vector<unsigned char> subData(spriteDataSize);
        glTexSubImage2D(	GL_TEXTURE_2D,
                            0,
                            0,
                            0,
                            sprite_size_.x,
                            sprite_size_.y,
                            GL_RGBA,
                            GL_UNSIGNED_BYTE,
                            subData.data());
#else
        //std::vector<unsigned char> subData(spriteDataSize);
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
#endif
        
        for (int row = 0; row < sprite_size_.y; ++row)
        {
            std::copy(  subData.begin() + row * 64,
                        subData.begin() + row * 64 + 64,
                        data.begin() + ix * 64 + row * 64 * tilemap_ids_.size()
                    );
        }
        ix++;
        glBindTexture(GL_TEXTURE_2D, 0);
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