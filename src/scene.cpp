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
  active_tilemap_name_ = "tiles_keen4";
  active_sprite_name_ = "r4c3";
  ResourceManager::LoadTexture("assets/tiles/keen4_tiles_red.png", sf::Color(186 ,254, 202, 255), active_tilemap_name_);
  TilemapManager::AddTilemap(active_tilemap_name_, 16, 16, { 2.0f, 2.0f });

  // Create framebuffers
  //ResourceManager::CreateRenderTexture(width_, height_, "viewport");
  ResourceManager::CreateRenderTexture(width_, height_, "viewport");
  ResourceManager::CreateRenderTexture(width_, height_, "minimap");
  ResourceManager::CreateRenderTexture(0, 0, "tex_background");

  //generateGrid();
}

Scene::~Scene() { }

GLvoid Scene::CreateMap(GLuint width, GLuint height, sf::Vector2u spriteSize, sf::Vector2f spriteScale)
{
    map_is_null_ = false;
    map_width_ = width;
    map_height_ = height;
    generateGrid();
    width_ = map_width_ * spriteSize.x * spriteScale.x;
    height_ = map_height_ * spriteSize.y * spriteScale.y;
    Update();
}

GLvoid Scene::Update()
{
    ResourceManager::UpdateRenderTexture(width_, height_, "viewport");
    ResourceManager::UpdateRenderTexture(width_, height_, "minimap");
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

        texViewport->draw(&grid_[0], grid_.size(), sf::Lines);

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

            if (add_sprite_flag_)
            {
                std::string hash = getNameHash(active_tilemap_name_, active_sprite_name_);
                spr->setPosition(sf::Vector2f(0, e_solids_.size() * 32));
                e_solids_.insert(std::make_pair(active_sprite_name_, new Solid(active_sprite_name_, *spr, layer_t::BACK)));

                std::stringstream msg;
                msg << "hash: " << hash;
                MessageManager::AddMessage(msg, message_t::INFO);

                ResourceManager::UpdateRenderTexture(32, e_solids_.size() * 32, "tex_background");
                sf::RenderTexture* texBackground = ResourceManager::GetRenderTexture("tex_background");
                texBackground->clear(sf::Color::Black);
                for (auto const& it : e_solids_)
                {
                    texBackground->draw(*it.second->GetSprite());
                }

                //texViewport->draw(*spr);

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

        for (auto const& it : e_solids_)
        {
            //texViewport->draw(*it.second->GetSprite());
            //texMinimap->draw(*it.second->GetSprite());
        }
     
        texViewport->display();
    }
}

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

    hash << tilesetName << ":" << tileName;
    return hash.str();
}
