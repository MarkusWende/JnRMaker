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

#include "../include/entity_level_layer.h"


// PUBLIC:
/////////////////////////////////////////////////////////////////////////////////////////////////////////
LevelLayer::LevelLayer()
{

};

LevelLayer::LevelLayer(std::string name, GLuint width, GLuint height, glm::vec2 spriteSize)
{
    name_ = name;
    width_ = width;
    height_ = height;
    width_pixels_ = width * spriteSize.x;
    height_pixels_ = height * spriteSize.y;
    tile_scale_ = { 1.0f, 1.0f };
    tile_size_ = spriteSize;
    //tile_id_max_ = 0.0f;
    hash_map_.resize(height, std::vector<std::string>(width, "empty"));

    std::stringstream keyBorder;
    keyBorder << "default_border_" << spriteSize.x << "x" << spriteSize.y;
    std::stringstream fileDefaultBorder;
    fileDefaultBorder << "data/assets/sprites/" << keyBorder.str().c_str() << ".png";
    //tile_hash_id_map_.insert(std::make_pair("border", 0.0f));

    TilemapManager::AddTilemap(name_, tile_size_, tile_scale_, fileDefaultBorder.str().c_str());
    //tilemap_ = std::make_unique<Tilemap>(name_, tile_size_, tile_scale_, fileDefaultBorder.str().c_str());
    
    init();
};

LevelLayer::~LevelLayer()
{
    glDeleteVertexArrays(1, &quad_vao_);
    glDeleteBuffers(1, &quad_vbo_);
    glDeleteBuffers(1, &tile_trans_vbo_);
    glDeleteBuffers(1, &tile_id_vbo_);
};

GLvoid LevelLayer::AddSprite(GLfloat mapID, const std::string key, GLuint texID)
{
    Tilemap* tiles = TilemapManager::GetTilemap(name_);
    std::vector<std::string> tileHashes = tiles->GetHashs();
    if (!tiles->HashExists(key))
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
    else
    {
        tile_id_.at(mapID) = (GLfloat)tiles->GetTileID(key);
    }
    glBindBuffer(GL_ARRAY_BUFFER, tile_id_vbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tile_id_.size() * sizeof(tile_id_.data()), tile_id_.data());
    
}

GLvoid LevelLayer::Draw(glm::mat4 projection, glm::mat4 view)
{
    ResourceManager::GetShader("llayer").Use();
    ResourceManager::GetShader("llayer").SetMatrix4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    ResourceManager::GetShader("llayer").SetMatrix4("view", view);
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //model = glm::translate(model, center_);
    ResourceManager::GetShader("llayer").SetMatrix4("model", model);
    //tilemap_->GetTexArray().Bind();
    TilemapManager::GetTilemap(name_)->GetTexArray().Bind();
    //ResourceManager::GetTextureAtlas(name_).Bind();
    glBindVertexArray(quad_vao_);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, width_ * height_);
}


// PRIVATE:
/////////////////////////////////////////////////////////////////////////////////////////////////////////
GLvoid LevelLayer::init()
{
    for (size_t y = 0; y < height_; y++)
    {
        for (size_t x = 0; x < width_; x++)
        {
            glm::vec2 translation;
            translation.x = (float)x + 0.5f;
            translation.y = (float)y + 0.5f;
            //translation.z = 0.0f;
            //translation.w = 0.0f;
            translations_.push_back(translation);
            tile_id_.push_back(0.0f);
        }
    }

    glGenBuffers(1, &tile_trans_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, tile_trans_vbo_);
    glBufferData(GL_ARRAY_BUFFER, translations_.size() * sizeof(translations_.data()), translations_.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &tile_id_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, tile_id_vbo_);
    glBufferData(GL_ARRAY_BUFFER, tile_id_.size() * sizeof(tile_id_.data()), tile_id_.data(), GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
       // positions   // texCoords
       -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  1.0f, 1.0f,

       -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  1.0f, 0.0f
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

GLvoid LevelLayer::draw_border()
{
    //ResourceManager::
    //tile_size_
    std::string key = "border";
    // draw left and right border
    for (size_t i = 0; i < height_; i++)
    {
        hash_map_.at(i).at(0) = key;
        hash_map_.at(i).at(width_ - 1) = key;
    }
    // draw upper and lower border
    for (size_t i = 1; i < width_ - 1; i++)
    {
        hash_map_.at(0).at(i) = key;
        hash_map_.at(height_ - 1).at(i) = key;
    }

    tile_id_.clear();
    std::string emptyHash = "empty";

    //GLuint counter = 0;
    for (size_t i = 0; i < height_; i++)
    {
        for (size_t j = 0; j < width_; j++)
        {
            if (hash_map_.at(i).at(j).compare(emptyHash) != 0)
            {
                tile_id_.push_back(1.0f);
            }
            else
            {
                tile_id_.push_back(0.0f);
            }
        }
    }

    // std::stringstream msg;
    // msg << "height_: " << height_ << std::endl;
    // msg << "width_: " << width_ << std::endl;
    // for (auto id : tile_id_)
    // {
    //     msg << id << " ";
    // }
    // MessageManager::AddMessage(msg, message_t::INFO);
    

    glBindBuffer(GL_ARRAY_BUFFER, tile_id_vbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tile_id_.size() * sizeof(tile_id_.data()), tile_id_.data());
}
