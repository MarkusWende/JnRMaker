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
 * @file scene.cpp
 * @brief This file contains the scene class implementation.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#include "../include/scene.h"

Scene::Scene(GLuint width, GLuint height)
{
  width_ = width;
  height_ = height;

  add_sprite_flag_ = false;
  mouse_over_scene_ = false;
  map_is_null_ = true;
  map_height_ = 0;
  map_width_ = 0;

  // Load tilemap
  active_tilemap_name_ = "";
  active_sprite_name_ = "";

  // Create framebuffers
  //ResourceManager::CreateRenderTexture(width_, height_, "viewport");
  //generateGrid();
}

Scene::~Scene() { }

GLvoid Scene::CreateMap(GLuint width, GLuint height, sf::Vector2u spriteSize, sf::Vector2f spriteScale)
{
    ResourceManager::CreateRenderTexture(width_, height_, "viewport");
    ResourceManager::CreateRenderTexture(width_, height_, "minimap");
    ResourceManager::CreateRenderTexture(32, 32, "tex_background");

    map_is_null_ = false;
    map_width_ = width;
    map_height_ = height;
    generateGrid();
    width_ = map_width_ * spriteSize.x * spriteScale.x;
    height_ = map_height_ * spriteSize.y * spriteScale.y;

    map_bg_ = std::vector<std::vector<GLuint>>(height, std::vector<GLuint>(width, 0));

    map_bg_vao_.clear();
    map_bg_vao_.setPrimitiveType(sf::Quads);
    map_bg_vao_.resize(map_width_ * map_height_* 4);

    Update();
}

GLvoid Scene::Update()
{
    ResourceManager::ResizeRenderTexture(width_, height_, "viewport");
    ResourceManager::ResizeRenderTexture(width_, height_, "minimap");
}

GLvoid Scene::Render(sf::Vector2i posMouse)
{
    if (!map_is_null_)
    {
        sf::RenderTexture* texViewport = ResourceManager::GetRenderTexture("viewport");
        sf::RenderTexture* texMinimap = ResourceManager::GetRenderTexture("minimap");
        texViewport->clear(sf::Color::Black);
        texMinimap->clear(sf::Color::Black);

        glm::vec2 sprSize = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteSize();
        glm::vec2 sprScale = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteScale();

        sf::Sprite* spr = TilemapManager::GetTilemap(active_tilemap_name_)->GetSprite(active_sprite_name_);
        sf::RectangleShape rectangle(sf::Vector2f(32, 32));
        rectangle.setFillColor(sf::Color(0, 0, 0, 0));
        rectangle.setOutlineColor(sf::Color::Green);
        rectangle.setOutlineThickness(2);

        if (mouse_over_scene_)
        {
            sf::Vector2i sprPos(posMouse.x, posMouse.y - 48);

            //std::stringstream msg;
            //msg << "x: " << (width_ % 32) << "\ty: " << (height_ % 32);
            //MessageManager::AddMessage(msg, message_t::INFO);

            GLuint row = sprPos.y / (sprSize.y * sprScale.y);
            GLuint col = sprPos.x / (sprSize.x * sprScale.x);

            GLuint y = row * (sprSize.y * sprScale.y);
            GLuint x = col * (sprSize.x * sprScale.x);

            spr->setPosition(sf::Vector2f(e_solids_.size() * (sprSize.y * sprScale.y), 0));

            if (add_sprite_flag_)
            {
                std::string hashSolid = getNameHash(active_tilemap_name_, active_sprite_name_);
                std::map<std::string, std::unique_ptr<Solid>>::iterator itSolid;
                itSolid = e_solids_.find(hashSolid);
                GLuint idSolid = 0;

                if (itSolid != e_solids_.end())
                {
                    idSolid = e_solids_.find(hashSolid)->second.get()->GetId();
                }
                else
                {
                    idSolid = e_solids_.size() + 1;
                    e_solids_.insert(std::make_pair(hashSolid, new Solid(idSolid, hashSolid, layer_t::BACK)));
                    sf::RenderTexture* renderTexBg = ResourceManager::GetRenderTexture("tex_background");
                    sf::Texture texBg = renderTexBg->getTexture();
                    sf::Sprite sprBgOld;
                    sprBgOld.setOrigin(0, 32);
                    sprBgOld.setScale(1.0f, -1.0f);
                    sprBgOld.setTexture(texBg);
                    ResourceManager::ResizeRenderTexture((idSolid) * (sprSize.x * sprScale.x), (sprSize.y * sprScale.y), "tex_background");
                    sf::RenderTexture* newRenderTexBg = ResourceManager::GetRenderTexture("tex_background");
                    newRenderTexBg->clear(sf::Color::Black);
                    newRenderTexBg->draw(sprBgOld);
                    newRenderTexBg->draw(*spr);

                    std::stringstream msg;
                    msg << "hash: " << hashSolid << "id: " << idSolid;
                    MessageManager::AddMessage(msg, message_t::INFO);
                }

                map_bg_.at(row).at(col) = idSolid;

                updateLayerVAO(layer_t::BACK);

                add_sprite_flag_ = false;
            }

            spr->setPosition(sf::Vector2f(x, y));
            rectangle.setPosition(sf::Vector2f(x, y));
        }
        else
        {
            spr->setPosition(sf::Vector2f(0, 0));
        }

        texViewport->draw(*spr);
        //texViewport->draw(rectangle);
        texMinimap->draw(*spr);
        //texMinimap->draw(rectangle);

        // Draw background
        sf::RenderStates states;
        states.transform = sf::Transform::Identity;
        states.texture = &ResourceManager::GetRenderTexture("tex_background")->getTexture();
        texViewport->draw(map_bg_vao_, states);
        texViewport->draw(&grid_[0], grid_.size(), sf::Lines);
        texMinimap->draw(map_bg_vao_, states);

        texViewport->display();
    }
}

GLvoid Scene::updateLayerVAO(layer_t layer)
{
    glm::vec2 sprSize = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteSize();
    glm::vec2 sprScale = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteScale();

    for (size_t row = 0; row < map_bg_.size(); row++)
    {
        for (size_t col = 0; col < map_bg_.at(0).size(); col++)
        {
            GLuint y = row * (sprSize.y * sprScale.y);
            GLuint x = col * (sprSize.x * sprScale.x);
            
            sf::Vertex* quad;
            GLuint tu, tv;
            GLuint id = 0;
            
            if (layer == layer_t::BACK)
            {
                id = map_bg_.at(row).at(col);
                quad = &map_bg_vao_[(col + row * map_width_) * 4];
                sf::RenderTexture* texBackground = ResourceManager::GetRenderTexture("tex_background");
                tu = (id-1) % (GLuint)(texBackground->getSize().x / (sprSize.x * sprScale.x));
                tv = (id-1) / (texBackground->getSize().x / (sprSize.x * sprScale.x));
            }
            
            if (id != 0)
            {
                quad[0].position = sf::Vector2f(x, y);
                quad[1].position = sf::Vector2f(x + (sprSize.x * sprScale.x), y);
                quad[2].position = sf::Vector2f(x + (sprSize.x * sprScale.x), y + (sprSize.y * sprScale.y));
                quad[3].position = sf::Vector2f(x, y + (sprSize.y * sprScale.y));

                quad[3].texCoords = sf::Vector2f(tu * (sprSize.x * sprScale.x), tv * (sprSize.y * sprScale.y));
                quad[2].texCoords = sf::Vector2f((tu + 1) * (sprSize.x * sprScale.x), tv * (sprSize.y * sprScale.y));
                quad[1].texCoords = sf::Vector2f((tu + 1) * (sprSize.x * sprScale.x), (tv + 1) * (sprSize.y * sprScale.y));
                quad[0].texCoords = sf::Vector2f(tu * (sprSize.x * sprScale.x), (tv + 1) * (sprSize.y * sprScale.y));
            }
        }
    }
}

// PRIVATE
GLvoid Scene::generateGrid()
{
  grid_.clear();

  glm::vec2 sprSize = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteSize();
  glm::vec2 sprScale = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteScale();

  // Draw a grid
  {
    sf::Color gridColor = {255, 255, 255, 50};
    // Push grid rows to vertex vector
    GLuint numRows = map_height_;
    for (size_t row = 0; row < numRows; row++)
    {
      GLuint rowPixel = row * (sprSize.y * sprScale.y);
      grid_.push_back(sf::Vertex(sf::Vector2f(0, rowPixel), gridColor));
      grid_.push_back(sf::Vertex(sf::Vector2f(map_width_ * (sprSize.y * sprScale.y), rowPixel), gridColor));
    }
    // Push grid columns to vertex vector
    GLuint numCols = map_width_;
    for (size_t col = 0; col < numCols; col++)
    {
      GLuint colPixel = col * (sprSize.x * sprScale.x);
      grid_.push_back(sf::Vertex(sf::Vector2f(colPixel, 0), gridColor));
      grid_.push_back(sf::Vertex(sf::Vector2f(colPixel, map_height_ * (sprSize.x * sprScale.x)), gridColor));
    }
  }
}

std::string Scene::getNameHash(std::string tilesetName, std::string tileName)
{
    std::stringstream hash;
    std::hash<std::string> hash_fn;
    size_t str_hash = hash_fn(tilesetName);
    hash << str_hash << ":" << tileName;
    return hash.str();
}
