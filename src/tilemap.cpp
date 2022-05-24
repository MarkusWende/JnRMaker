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

Tilemap::Tilemap(std::string name, glm::vec2 spriteSize, glm::vec2 spriteScale, const unsigned char *data, int size)
{
    //ResourceManager::LoadTexture(file.c_str(), GL_TRUE, name);
    name_ = name;
    num_cols_ = 0;
    num_rows_ = 0;
    sprite_size_ = spriteSize;
    sprite_scale_ = spriteScale;
    tilemap_id_max_ = 0;

    // Load image
    int width = 0;
    int height = 0;
    int channels = 0;
    //unsigned char* image = SOIL_load_image(file.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

    if (!stbi_info_from_memory(data, size, &width, &height, &channels))
    {
        std::stringstream msg;
        msg << stbi_failure_reason();
        MessageManager::AddMessage(msg, message_t::ERROR_T);
        return;
    }

    unsigned char *img = stbi_load_from_memory(data, size, &width, &height, &channels, 4);
    // unsigned char* image_data = stbi_load_from_memory(data, size, &width, &height, &channelsInData, channels);

    /* exit if the image is larger than ~80MB */
    if (width && height > (80000000 / 4) / height)
    {
        std::stringstream msg;
        msg << "Image is bigger than 80MB.";
        MessageManager::AddMessage(msg, message_t::ERROR_T);
        return;
    }

    if (NULL == img)
    {
        std::stringstream msg;
        msg << stbi_failure_reason();
        MessageManager::AddMessage(msg, message_t::ERROR_T);
    }
    else
    {
        std::stringstream msg;
        msg << "Width: " << width << "\tHeight: " << height;
        MessageManager::AddMessage(msg, message_t::DEBUG);
        
        GLboolean alpha = GL_FALSE;
        if (channels == 4)
        {
            alpha = GL_TRUE;
        }
        else if (channels != 3 || channels != 4)
        {
            std::stringstream msg;
            msg << "Tilemap: channels are not right!";
            MessageManager::AddMessage(msg, message_t::ERROR_T);
        }

        ResourceManager::CreateTextureArray(img, width, height, (GLuint)sprite_size_.x, (GLuint)sprite_size_.y, alpha, name_);

        num_rows_ = (GLuint)height / (GLuint)sprite_size_.y;
        num_cols_ = (GLuint)width / (GLuint)sprite_size_.x;

        /* std::stringstream msg;
        msg << "rows: " << num_rows_ << "\tcols: " << num_cols_;
        MessageManager::AddMessage(msg, message_t::INFO); */

        for (GLuint i = 0; i < num_rows_; i++)
        {
            for (GLuint j = 0; j < num_cols_; j++)
            {
                
                std::stringstream key;
                key << "r" << i << "c" << j;
                std::string hashKey = ResourceManager::getNameHash(name_, key.str());
                tilemap_ids_.insert(std::make_pair(tilemap_id_max_, hashKey));
                // tilemap_textures_.insert(std::make_pair(hashKey, new Texture2D()));
                // tilemap_textures_.find(hashKey)->second->Generate(sprite_size_.x, sprite_size_.y, pixels);

                tilemap_id_max_++;
            }
        }
    }

    stbi_image_free(img);
}

Tilemap::Tilemap(std::string name, glm::vec2 spriteSize, glm::vec2 spriteScale, std::string file)
{
    //ResourceManager::LoadTexture(file.c_str(), GL_TRUE, name);
    name_ = name;
    num_cols_ = 0;
    num_rows_ = 0;
    sprite_size_ = spriteSize;
    sprite_scale_ = spriteScale;
    tilemap_id_max_ = 0;

    // Load image
    int width = 0;
    int height = 0;
    int channels = 0;
    //unsigned char* image = SOIL_load_image(file.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
    unsigned char* image = stbi_load(file.c_str(), &width, &height, &channels, 4);
    
    if (NULL == image)
    {
        std::stringstream msg;
        msg << stbi_failure_reason() << ": " << file;
        MessageManager::AddMessage(msg, message_t::ERROR_T);
    }
    else
    {
        GLboolean alpha = GL_FALSE;
        if (channels == 4)
        {
            alpha = GL_TRUE;
        }
        else if (channels != 3 || channels != 4)
        {
            std::stringstream msg;
            msg << "Tilemap: channels are not right!";
            MessageManager::AddMessage(msg, message_t::ERROR_T);
        }

        ResourceManager::CreateTextureArray(image, width, height, (GLuint)sprite_size_.x, (GLuint)sprite_size_.y, alpha, name_);

        num_rows_ = (GLuint)height / (GLuint)sprite_size_.y;
        num_cols_ = (GLuint)width / (GLuint)sprite_size_.x;

        /* std::stringstream msg;
        msg << "rows: " << num_rows_ << "\tcols: " << num_cols_;
        MessageManager::AddMessage(msg, message_t::INFO); */

        for (GLuint i = 0; i < num_rows_; i++)
        {
            for (GLuint j = 0; j < num_cols_; j++)
            {
                
                std::stringstream key;
                key << "r" << i << "c" << j;
                std::string hashKey = ResourceManager::getNameHash(name_, key.str());
                tilemap_ids_.insert(std::make_pair(tilemap_id_max_, hashKey));
                // tilemap_textures_.insert(std::make_pair(hashKey, new Texture2D()));
                // tilemap_textures_.find(hashKey)->second->Generate(sprite_size_.x, sprite_size_.y, pixels);

                tilemap_id_max_++;
            }
        }
    }

    stbi_image_free(image);
}

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
}


Tilemap::~Tilemap()
{
    
}

GLvoid Tilemap::AddTile(const std::string spriteName, GLuint spriteTexID)
{
    if (HashExists(spriteName))
    {
        return;
    }
    
    try
    {
        if (num_rows_ == 0)
            num_rows_ = 1;

        int channels = 4;
        int spriteDataSize = (int)sprite_size_.x * (int)sprite_size_.y * channels;
        GLuint tilemapSize = (GLuint)tilemap_ids_.size();

        num_cols_ = tilemapSize + 1;

        // std::stringstream key;
        // key << "r" << num_rows_ << "c" << num_cols_;
        // std::string hashKey = ResourceManager::getNameHash(name_, key.str());
        tilemap_ids_.insert(std::make_pair(tilemap_id_max_, spriteName));
        tilemap_id_max_++;

        std::vector<unsigned char> dataNewTexArray(spriteDataSize * tilemap_ids_.size());
        std::vector<unsigned char> dataNewTile(spriteDataSize);

        GLuint fboNewTile;
        glGenFramebuffers(1, &fboNewTile); 
        glBindFramebuffer(GL_FRAMEBUFFER, fboNewTile);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, spriteTexID, 0);

        glReadPixels(0, 0, (GLsizei)sprite_size_.x, (GLsizei)sprite_size_.y, GL_RGBA, GL_UNSIGNED_BYTE, dataNewTile.data());

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &fboNewTile);
        glBindTexture(GL_TEXTURE_2D, 0);

        size_t ix = 0;
        for (auto const& [id, hash] : tilemap_ids_)
        {
            std::vector<unsigned char> dataCurrentTile(spriteDataSize);
                
            // std::stringstream key;
            // key << "r" << 0 << "c" << ix;
            // std::string hashKey = ResourceManager::getNameHash(name_, key.str());

            GLuint currentTileID = 0;
            currentTileID = ResourceManager::GetTexture(hash).ID;
            glBindTexture(GL_TEXTURE_2D, currentTileID);

            GLuint fboCurrentTile;
            glGenFramebuffers(1, &fboCurrentTile); 
            glBindFramebuffer(GL_FRAMEBUFFER, fboCurrentTile);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, currentTileID, 0);

            glReadPixels(0, 0, (GLsizei)sprite_size_.x, (GLsizei)sprite_size_.y, GL_RGBA, GL_UNSIGNED_BYTE, dataCurrentTile.data());

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDeleteFramebuffers(1, &fboCurrentTile);
            glBindTexture(GL_TEXTURE_2D, 0);

            for (size_t row = 0; row < sprite_size_.y; ++row)
            {
                std::copy(  dataCurrentTile.begin() + row * 64,
                            dataCurrentTile.begin() + row * 64 + 64,
                            dataNewTexArray.begin() + ix * 64 + row * 64 * tilemap_ids_.size()
                        );
            }
            ix++;
        }

        ix--;

        for (int row = 0; row < sprite_size_.y; ++row)
        {
            std::copy(  dataNewTile.begin() + row * 64,
                        dataNewTile.begin() + row * 64 + 64,
                        dataNewTexArray.begin() + ix * 64 + row * 64 * tilemap_ids_.size()
                    );
        }

        ResourceManager::CreateTextureArray(dataNewTexArray.data(), (GLuint)sprite_size_.x * num_cols_, (GLuint)sprite_size_.y, (GLuint)sprite_size_.x, (GLuint)sprite_size_.y, GL_TRUE, name_);
        
        //unsigned char* data = (unsigned char*)malloc(sprite_size_.x * sprite_size_.y * 4);

        /* std::stringstream key;
        key << "r" << 0 << "c" << tilemapSize;
        std::string hashKey = ResourceManager::getNameHash(name_, key.str()); */





        // glBindTexture(GL_TEXTURE_2D, texID);
        // int data_size = sprite_size_.x * sprite_size_.y * 4;
        // unsigned char* pixels = (unsigned char*)malloc(data_size);

        // //GLuint textureObj = tex.ID; // the texture object - glGenTextures  

        // GLuint fbo;
        // glGenFramebuffers(1, &fbo); 
        // glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);

        // glReadPixels(0, 0, sprite_size_.x, sprite_size_.y, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glDeleteFramebuffers(1, &fbo);
        // glBindTexture(GL_TEXTURE_2D, 0);




#ifdef __EMSCRIPTEN__
        // glBindTexture(GL_TEXTURE_2D, texID);
        // auto *ptr = m_Pixels + (y * mWidth) * 4;
        // glTexSubImage2D( GL_TEXTURE_2D, 0, 0, y, mWidth, h, GL_RGBA, GL_UNSIGNED_BYTE, ptr );   
        // glBindTexture(GL_TEXTURE_2D, texID);
        // glTexSubImage2D(	GL_TEXTURE_2D,
        //                     0,
        //                     0,
        //                     0,
        //                     sprite_size_.x,
        //                     sprite_size_.y,
        //                     GL_RGBA,
        //                     GL_UNSIGNED_BYTE,
        //                     data);
        // glBindTexture(GL_TEXTURE_2D, 0);
        // GLenum err = glGetError();
        // if (err != GL_NO_ERROR)
        // {
        //     std::stringstream msg;
        //     msg << "Failed to copy sumbimage in Tilemap::AddTile(), Error: " << err;
        //     MessageManager::AddMessage(msg, message_t::ERROR_T);
        // }
#else
        // glGetTextureSubImage(   texID,
        //                         0,
        //                         0,
        //                         0,
        //                         0,
        //                         sprite_size_.x,
        //                         sprite_size_.y,
        //                         1,
        //                         GL_RGBA,
        //                         GL_UNSIGNED_BYTE,
        //                         sprite_size_.x * sprite_size_.y * 4,
        //                         data);
#endif
        // if (tilemap_textures_.find(key) == tilemap_textures_.end())
        // {
        //     tilemap_ids_.insert(std::make_pair(tilemap_id_max_, key));
        //     tilemap_id_max_++;
        //     // tilemap_textures_.insert(std::make_pair(key, new Texture2D()));
        //     // tilemap_textures_.find(key)->second->Generate(sprite_size_.x, sprite_size_.y, pixels);
        //     num_cols_ = tilemapSize + 1;
        //     createTextureArray();
        // }
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
    for (auto const& [id, hash] : tilemap_ids_)
    {
        if (hash.compare(key) == 0)
        {
            return true;
        }
    }
    
    return false;
}

std::vector<std::string> Tilemap::GetHashs()
{
    std::vector<std::string> hashes;
    for (auto const& [id, hash] : tilemap_ids_)
    {
        hashes.push_back(hash);
    }
    return hashes;
}

// GLvoid Tilemap::loadTilemapFromTexture()
// {
//     Texture2D tex = ResourceManager::GetTexture(name_);

//     num_rows_ = tex.Height / sprite_size_.y;
//     num_cols_ = tex.Width / sprite_size_.x;

//     /* std::stringstream msg;
//     msg << "rows: " << num_rows_ << "\tcols: " << num_cols_;
//     MessageManager::AddMessage(msg, message_t::INFO); */

//     for (GLuint i = 0; i < num_cols_; i++)
//     {
//         for (GLuint j = 0; j < num_rows_; j++)
//         {
            
//             tex.Bind();
//             int data_size = sprite_size_.x * sprite_size_.y * 4;
//             unsigned char* pixels = (unsigned char*)malloc(data_size);

//             GLuint textureObj = tex.ID; // the texture object - glGenTextures  

//             GLuint fbo;
//             glGenFramebuffers(1, &fbo); 
//             glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//             glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureObj, 0);

//             glReadPixels(i * sprite_size_.x, j * sprite_size_.y, sprite_size_.x, sprite_size_.y, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

//             glBindFramebuffer(GL_FRAMEBUFFER, 0);
//             glDeleteFramebuffers(1, &fbo);
//             // std::stringstream newName;
//             // newName << name_ << "_r" << j << "c" << i;
//             // ResourceManager::CreateTexture(pixels, sprite_size_.x, sprite_size_.y, GL_TRUE, newName.str());
//             glBindTexture(GL_TEXTURE_2D, 0);

// #ifdef __EMSCRIPTEN__
//             // tex.Bind();
//             // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//             // glTexSubImage2D(	GL_TEXTURE_2D,
//             //                     0,
//             //                     i * sprite_size_.x,
//             //                     j * sprite_size_.y,
//             //                     sprite_size_.x,
//             //                     sprite_size_.y,
//             //                     tex.Image_Format,
//             //                     GL_UNSIGNED_BYTE,
//             //                     data);
//             // glBindTexture(GL_TEXTURE_2D, 0);
//             // GLenum err = glGetError();
//             // if (err != GL_NO_ERROR)
//             // {
//             //     std::stringstream msg;
//             //     msg << "Failed to copy sumbimage in Tilemap::loadTilemapFromTexture(), Error: " << err;
//             //     MessageManager::AddMessage(msg, message_t::ERROR_T);
//             // }
// #else
//             // glGetTextureSubImage(   tex.ID,
//             //                         0,
//             //                         i * sprite_size_.x,
//             //                         j * sprite_size_.y,
//             //                         0,
//             //                         sprite_size_.x,
//             //                         sprite_size_.y,
//             //                         1,
//             //                         GL_RGBA,
//             //                         GL_UNSIGNED_BYTE,
//             //                         sprite_size_.x * sprite_size_.y * 4,
//             //                         data);
// #endif

//             std::stringstream key;
//             key << "r" << j << "c" << i;
//             std::string hashKey = ResourceManager::getNameHash(name_, key.str());
//             ResourceManager::CreateTexture(pixels, sprite_size_.x, sprite_size_.y, GL_TRUE, hashKey.c_str());
            
//             tilemap_ids_.insert(std::make_pair(tilemap_id_max_, hashKey));
//             tilemap_textures_.insert(std::make_pair(hashKey, new Texture2D()));
//             tilemap_textures_.find(hashKey)->second->Generate(sprite_size_.x, sprite_size_.y, pixels);

//             tilemap_id_max_++;
//         }
//     }

//     //createTextureArray();

//     /*  for (auto const& [key, val] : tilemap_)
//     {
//         std::stringstream msg;
//         msg << "key: " << key;
//         MessageManager::AddMessage(msg, message_t::INFO);
//     } */
    
// }

// GLvoid Tilemap::createTextureArray()
// {

//     int channels = 4;
//     int ix = 0;
//     /*
//     int tilesX = tilemap_textures_.size();
//     int rowLen = (int)sprite_size_.x * channels;
//     int channelSize = sprite_size_.x * sprite_size_.y * tilemap_textures_.size(); */
//     int spriteDataSize = (int)sprite_size_.x * (int)sprite_size_.y * channels;
//     //unsigned char* data = (unsigned char*)malloc(sprite_size_.x * sprite_size_.y * channels * tilemap_.size());
//     std::vector<unsigned char> data(spriteDataSize * tilemap_textures_.size());
    
//     for (auto const& [key, val] : tilemap_ids_)
//     {
//         GLuint texID = tilemap_textures_.find(val)->second.get()->ID;
//         //std::vector<unsigned char> subData(spriteDataSize);










//         glBindTexture(GL_TEXTURE_2D, texID);
//         //int data_size = sprite_size_.x * sprite_size_.y * 4;
//         std::vector<unsigned char> pixels(spriteDataSize);

//         //GLuint textureObj = tex.ID; // the texture object - glGenTextures  

//         GLuint fbo;
//         glGenFramebuffers(1, &fbo); 
//         glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//         glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);

//         glReadPixels(0, 0, sprite_size_.x, sprite_size_.y, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

//         glBindFramebuffer(GL_FRAMEBUFFER, 0);
//         glDeleteFramebuffers(1, &fbo);
       
//         glBindTexture(GL_TEXTURE_2D, 0);












//         /* std::stringstream filename;
//         filename << "test-" << key << ".bmp";
//         SOIL_save_image(    filename.str().c_str(),
//                             SOIL_SAVE_TYPE_BMP,
//                             sprite_size_.x,
//                             sprite_size_.y,
//                             4,
//                             subData.data()); */
//         //std::copy(subData.begin(), subData.end(), data.begin() + ix * spriteDataSize);
//         //data.insert( data.end(), subData.begin(), subData.end() );

// #ifdef __EMSCRIPTEN__
//         //unsigned char* subData = (unsigned char*)malloc(spriteDataSize);
//         // glBindTexture(GL_TEXTURE_2D, texID);
//         // //std::vector<unsigned char> subData(spriteDataSize);
//         // glTexSubImage2D(	GL_TEXTURE_2D,
//         //                     0,
//         //                     0,
//         //                     0,
//         //                     sprite_size_.x,
//         //                     sprite_size_.y,
//         //                     GL_RGBA,
//         //                     GL_UNSIGNED_BYTE,
//         //                     subData.data());
//         // glBindTexture(GL_TEXTURE_2D, 0);
//         // GLenum err = glGetError();
//         // if (err != GL_NO_ERROR)
//         // {
//         //     std::stringstream msg;
//         //     msg << "Failed to copy sumbimage in Tilemap::createTextureArray(), Error: " << err;
//         //     MessageManager::AddMessage(msg, message_t::ERROR_T);
//         // }

// #else
//         //std::vector<unsigned char> subData(spriteDataSize);
//         // glGetTextureSubImage(   texID,
//         //                             0,
//         //                             0,
//         //                             0,
//         //                             0,
//         //                             sprite_size_.x,
//         //                             sprite_size_.y,
//         //                             1,
//         //                             GL_RGBA,
//         //                             GL_UNSIGNED_BYTE,
//         //                             spriteDataSize,
//         //                             subData.data());
// #endif
//         for (int row = 0; row < sprite_size_.y; ++row)
//             std::copy(  pixels.begin() + row * 64,
//                         pixels.begin() + row * 64 + 64,
//                         data.begin() + ix * 64 + row * 64 * tilemap_ids_.size()
//                     );
//         ix++;
//     }

//     std::stringstream newName;
//     newName << name_ << "_data_" << sprite_size_.x * tilemap_textures_.size() << "x" << sprite_size_.y;
//     ResourceManager::CreateTexture(data.data(), sprite_size_.x * tilemap_textures_.size(), sprite_size_.y, GL_TRUE, newName.str());

//     /* int counter = 0;
//     for (size_t i = 0; i < data.size();)
//     {
//         if (i < 64)
//         {
//             data.at(i) = 255;
//             data.at(i+1) = 0;
//             data.at(i+2) = 0;
//             data.at(i+3) = 255;
//         } else
//         {
//             data.at(i) = 200;
//             data.at(i+1) = 255;
//             data.at(i+2) = 0;
//             data.at(i+3) = 255;
//         }  
//         i = i + 4;
//         counter++;
//     } */






    

//     //SOIL_save_image("test.bmp", SOIL_SAVE_TYPE_BMP, sprite_size_.x * tilemap_textures_.size(), sprite_size_.y, 4, data.data());
//     tilemap_tex_array_.Generate(sprite_size_.x * tilemap_textures_.size(), sprite_size_.y, data.data(), sprite_size_);

//     tilemap_tex_array_.Bind();
//     int data_size = tilemap_tex_array_.Width * tilemap_tex_array_.Height * 4;
//     unsigned char* pixels = (unsigned char*)malloc(data_size);

//     GLuint textureObj = tilemap_tex_array_.ID; // the texture object - glGenTextures  

//     GLuint fbo;
//     glGenFramebuffers(1, &fbo); 
//     glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_ARRAY, textureObj, 0);

//     glReadPixels(0, 0, tilemap_tex_array_.Width, tilemap_tex_array_.Height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

//     glBindFramebuffer(GL_FRAMEBUFFER, 0);
//     glDeleteFramebuffers(1, &fbo);
//     newName.str("");
//     newName << name_ << "_" << tilemap_tex_array_.Width << "x" << tilemap_tex_array_.Height;
//     ResourceManager::CreateTexture(pixels, sprite_size_.x * tilemap_textures_.size(), sprite_size_.y, GL_TRUE, newName.str());
//     glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
// }