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

  // Load tilemap
  active_tilemap_name_ = "tiles_keen4";
  active_sprite_name_ = "r4c3";
  ResourceManager::LoadTexture("assets/tiles/keen4_tiles.png", sf::Color(186 ,254, 202, 255), active_tilemap_name_);
  TilemapManager::AddTilemap(active_tilemap_name_, 16, 16, { 2.0f, 2.0f });

  // Create framebuffers
  //ResourceManager::CreateRenderTexture(width_, height_, "viewport");
  ResourceManager::CreateRenderTexture(1000, 650, "viewport");

  generateGrid();
}

Scene::~Scene() { }

GLvoid Scene::Update()
{
  //ResourceManager::UpdateRenderTexture(width_, height_, "viewport");
  ResourceManager::UpdateRenderTexture(1000, 650, "viewport");
  generateGrid();
}

GLvoid Scene::Render(sf::Vector2i posMouse)
{
  sf::RenderTexture* texViewport = ResourceManager::GetRenderTexture("viewport");
  texViewport->clear(sf::Color::Black);

  glm::vec2 sprSize = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteSize();
  glm::vec2 sprScale = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteScale();

  texViewport->draw(&grid_[0], grid_.size(), sf::Lines);


  sf::Sprite* spr = TilemapManager::GetTilemap(active_tilemap_name_)->GetSprite(active_sprite_name_);
  if (mouse_over_scene_)
  {
      if (add_sprite_flag_)
      {
          sf::Vector2i sprPos(posMouse.x - 30, posMouse.y - 86);
          sf::Vector2i clippedPos;
          clippedPos.x = (int)(sprPos.x / 32) * 32 + 8;
          clippedPos.y = (int)(sprPos.y / 32) * 32 + 10;
          spr->setPosition(sf::Vector2f(clippedPos.x, clippedPos.y));

          add_sprite_flag_ = false;
      }
  }
  else
  {
      spr->setPosition(sf::Vector2f(0, 0));
  }
  texViewport->draw(*spr);


  texViewport->display();
}

GLvoid Scene::generateGrid()
{
  grid_.clear();

  glm::vec2 sprSize = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteSize();
  glm::vec2 sprScale = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteScale();

  // Draw a grid
  {
    sf::Color gridColor = {10, 10, 10, 255};
    // Push grid rows to vertex vector
    for (size_t row = 650 / (sprSize.x * sprScale.x); row != 0;)
    {
      //GLuint rowY = row * (sprSize.x * sprScale.x) - height_ % (GLuint)(sprSize.x * sprScale.x) + 1;
      GLuint rowY = row * (sprSize.x * sprScale.x) - ((sprSize.x * sprScale.x) -  (650 - ((sprSize.x * sprScale.x) * (int)(650 / (sprSize.x * sprScale.x)))));
      grid_.push_back(sf::Vertex(sf::Vector2f(0, rowY), gridColor));
      grid_.push_back(sf::Vertex(sf::Vector2f(1000, rowY), gridColor));

      row--;
    }
    // Push grid columns to vertex vector
    for (size_t col = 0; col < 1000 / (sprSize.y * sprScale.y); col++)
    {
      GLuint colX = col * (sprSize.y * sprScale.y) - ((sprSize.y * sprScale.y) - (1000 - ((sprSize.y * sprScale.y) * (int)(1000 / (sprSize.y * sprScale.y))))) - 1;
      grid_.push_back(sf::Vertex(sf::Vector2f(colX, 0), gridColor));
      grid_.push_back(sf::Vertex(sf::Vector2f(colX, 650), gridColor));
    }
  }
}
