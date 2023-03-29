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
  * @file entity_level_layer.h
  * @brief This file contains the solid entity class.
  *
  * @author Markus Wende
  * https://github.com/MarkusWende
  */

#include "entity_world_layer.h"


// PUBLIC:
/////////////////////////////////////////////////////////////////////////////////////////////////////////
WorldLayer::WorldLayer()
{

}

WorldLayer::WorldLayer(std::string name, GLuint width, GLuint height, glm::vec2 spriteSize, GLuint borderSize)
{
    name_ = name;
    width_ = width;
    height_ = height;
    width_pixels_ = width_ * (GLuint)spriteSize.x;
    height_pixels_ = height_ * (GLuint)spriteSize.y;
    tile_scale_ = { 1.0f, 1.0f };
    tile_size_ = spriteSize;
    border_size_ = (GLfloat)borderSize;
    //tile_id_max_ = 0.0f;

    // std::stringstream keyEmptyFile;
    // keyEmptyFile << "default_empty_" << spriteSize.x << "x" << spriteSize.y;
    // std::stringstream keyEmptyPath;
    // keyEmptyPath << "data/assets/sprites/" << keyEmptyFile.str().c_str() << ".png";
    // hash_map_empty_key_ = ResourceManager::getNameHash(name, "r0c0");

    std::stringstream keyBorderFile;
    keyBorderFile << "default_" << spriteSize.x << "x" << spriteSize.y;
    std::stringstream keyBorderPath;
    keyBorderPath << "data/assets/tiles/" << keyBorderFile.str().c_str() << ".png";
    //tile_hash_id_map_.insert(std::make_pair("border", 0.0f));
    hash_map_border_key_ = ResourceManager::getNameHash(name, "r0c1");

    hash_map_.resize((size_t)height_ + 2 * (size_t)border_size_, std::vector<std::string>((size_t)width_ + 2 * (size_t)border_size_, ""));

    // std::stringstream msg;
    // // msg << "hash_map_empty_key_: " << hash_map_empty_key_ << std::endl;
    // msg << "hash_map_border_key_: " << hash_map_border_key_;
    // MessageManager::AddMessage(msg, message_t::INFO);
    

    //TilemapManager::AddTilemap(name_, tile_size_, tile_scale_, emptyTile, 1024);
    //auto tiles = TilemapManager::GetTilemap(name_);
    //ResourceManager::CreateTexture(emptyTile.data(), sizeof(emptyTile.data()), "border");
    //tiles->AddTile("border", ResourceManager::GetTexture("border").ID);
    TilemapManager::AddTilemap(name_, tile_size_, tile_scale_, keyBorderPath.str().c_str());
    // ResourceManager::LoadTexture(keyBorderPath.str().c_str(), true, hash_map_border_key_.c_str());
    // GLuint texID = ResourceManager::GetTexture(hash_map_border_key_.c_str()).ID;
    // TilemapManager::GetTilemap(name_)->AddTile(hash_map_border_key_, texID);
    //tilemap_ = std::make_unique<Tilemap>(name_, tile_size_, tile_scale_, fileDefaultBorder.str().c_str());

    // ResourceManager::LoadTexture("data/assets/sprites/keen4_sprite_flame_0.png", GL_TRUE, "testing123");
    
    init();
}

WorldLayer::~WorldLayer()
{
    glDeleteVertexArrays(1, &quad_vao_);
    glDeleteBuffers(1, &quad_vbo_);
    glDeleteBuffers(1, &tile_trans_vbo_);
    glDeleteBuffers(1, &tile_id_vbo_);
}

GLvoid WorldLayer::AddSprite(GLuint mapID, const std::string key, GLuint texID)
{
    auto tiles = TilemapManager::GetTilemap(name_);
    auto tileHashes = tiles->GetHashs();
    //if (!tiles->HashExists(key))
    {
        tiles->AddTile(key, texID);
        //tile_id_max_ = tile_id_max_ + 1.0f;
        //tile_hash_id_map_.insert(std::make_pair(key.c_str(), tile_id_max_));
        tile_id_.at(mapID) = (GLfloat)tiles->GetTileID(key);

        /* std::stringstream msg;
        for (auto const& [hashKey, hashVal] : tile_hash_id_map_)
		{
            msg << hashKey << ":" << hashVal << "\t";
        }
        MessageManager::AddMessage(msg, message_t::INFO); */
    }
    //else
    {
        //tile_id_.at(mapID) = (GLfloat)tiles->GetTileID(key);
    }
    glBindBuffer(GL_ARRAY_BUFFER, tile_id_vbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tile_id_.size() * sizeof(GLfloat), tile_id_.data());
    
}

GLvoid WorldLayer::Draw(glm::mat4 projection, glm::mat4 view)
{
    ResourceManager::GetShader("llayer").Use();
    ResourceManager::GetShader("llayer").SetMatrix4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    ResourceManager::GetShader("llayer").SetMatrix4("view", view);
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //model = glm::translate(model, center_);
    ResourceManager::GetShader("llayer").SetMatrix4("model", model);
    //tilemap_->GetTexArray().Bind();
    //TilemapManager::GetTilemap(name_)->GetTexArray().Bind();
    ResourceManager::GetTextureArray(name_).Bind();
    //ResourceManager::GetTextureAtlas(name_).Bind();
    glBindVertexArray(quad_vao_);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, width_ * height_ * 2 * border_size_);

    // std::stringstream msg;
    // msg << "width: " << width_ << "\theight: " << height_;
    // MessageManager::AddMessage(msg, message_t::INFO);
}


// PRIVATE:
/////////////////////////////////////////////////////////////////////////////////////////////////////////
GLvoid WorldLayer::init()
{
    for (GLfloat y = -border_size_; y < (GLfloat)height_ + border_size_;)
    {
        for (GLfloat x = -border_size_; x < (GLfloat)width_ + border_size_;)
        {
            glm::vec2 translation;
            // -1.0f => To draw the border around the level
            translation.x = x;
            translation.y = y;
            //translation.z = 0.0f;
            //translation.w = 0.0f;
            translations_.push_back(translation);
            tile_id_.push_back(0.0f);
            x = x + 1.0f;
        }
        y = y + 1.0f;
    }

    glGenBuffers(1, &tile_trans_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, tile_trans_vbo_);
    glBufferData(GL_ARRAY_BUFFER, translations_.size() * sizeof(glm::vec2), translations_.data(), GL_STATIC_DRAW);
    

    glGenBuffers(1, &tile_id_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, tile_id_vbo_);
    glBufferData(GL_ARRAY_BUFFER, tile_id_.size() * sizeof(GLfloat), tile_id_.data(), GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
       // positions   // texCoords
        0.0f,  1.0f,  0.0f, 0.0f,
        0.0f,  0.0f,  0.0f, 1.0f,
        1.0f,  0.0f,  1.0f, 1.0f,

        0.0f,  1.0f,  0.0f, 0.0f,
        1.0f,  0.0f,  1.0f, 1.0f,
        1.0f,  1.0f,  1.0f, 0.0f
    };

    glGenVertexArrays(1, &quad_vao_);
    glGenBuffers(1, &quad_vbo_);
    glBindVertexArray(quad_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    // instance data
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, tile_trans_vbo_);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, tile_id_vbo_);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.

    draw_border();
}

GLvoid WorldLayer::draw_border()
{
    //ResourceManager::
    //tile_size_
    // std::stringstream key = "border";
    // draw left and right border
    for (size_t b = 0; b < (size_t)border_size_; b++)
    {
        for (size_t i = b; i < height_ + 2*(size_t)border_size_ - b; i++)
        {
            hash_map_.at(i).at(b) = hash_map_border_key_;
            hash_map_.at(i).at(width_ + 2*(size_t)border_size_ - b - 1) = hash_map_border_key_;
        }
        // draw upper and lower border
        for (size_t i = b; i < width_ + 2*(size_t)border_size_ - b; i++)
        {
            hash_map_.at(b).at(i) = hash_map_border_key_;
            hash_map_.at(height_ + 2*(size_t)border_size_ - b - 1).at(i) = hash_map_border_key_;
        }
    }

    tile_id_.clear();
    // std::string emptyHash = "empty";

    //GLuint counter = 0;
    for (size_t i = 0; i < height_ + 2*(size_t)border_size_; i++)
    {
        for (size_t j = 0; j < width_ + 2*(size_t)border_size_; j++)
        {
            if (hash_map_.at(i).at(j).compare("") != 0)
            {
                tile_id_.push_back(1.0);
            }
            else
            {
                tile_id_.push_back(0.0);
            }
        }
    }
    

    glBindBuffer(GL_ARRAY_BUFFER, tile_id_vbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tile_id_.size() * sizeof(GLfloat), tile_id_.data());
}
