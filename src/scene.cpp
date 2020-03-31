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

  // Load tilemap
  current_tilemap_name_ = "tiles_keen4";
  ResourceManager::LoadTexture("assets/tiles/keen4_tiles.png", sf::Color(186 ,254, 202, 255), current_tilemap_name_);
  TilemapManager::AddTilemap(current_tilemap_name_, 16, 16, { 2.0f, 2.0f });

  // Create framebuffers
  ResourceManager::CreateRenderTexture(width_, height_, "viewport");

  generateGrid();
}

Scene::~Scene() { }

GLvoid Scene::Update()
{
  ResourceManager::UpdateRenderTexture(width_, height_, "viewport");
  generateGrid();
}

GLvoid Scene::Render()
{
  sf::RenderTexture* texViewport = ResourceManager::GetRenderTexture("viewport");
  texViewport->clear(sf::Color::Black);

  glm::vec2 sprSize = TilemapManager::GetTilemap(current_tilemap_name_)->GetSpriteSize();
  glm::vec2 sprScale = TilemapManager::GetTilemap(current_tilemap_name_)->GetSpriteScale();

  texViewport->draw(&grid_[0], grid_.size(), sf::Lines);

  for (GLuint i = 0; i < 12; i++)
  {
    sf::Sprite* spr = TilemapManager::GetTilemap(current_tilemap_name_)->GetSprite(current_sprite_name_);
    spr->setPosition(sf::Vector2f(i * (sprSize.x * sprScale.x), height_ - (sprSize.y * sprScale.y) - 48));
    texViewport->draw(*spr);
  }

  texViewport->display();
}

GLvoid Scene::generateGrid()
{
  grid_.clear();

  glm::vec2 sprSize = TilemapManager::GetTilemap(current_tilemap_name_)->GetSpriteSize();
  glm::vec2 sprScale = TilemapManager::GetTilemap(current_tilemap_name_)->GetSpriteScale();

  std::stringstream msg;
  msg << "height mod row: " <<  height_ % (GLuint)(sprSize.x * sprScale.x);
  MessageManager::AddMessage(msg, message_t::INFO);

  // Draw a grid
  {
    sf::Color gridColor = {10, 10, 10, 255};
    // Push grid rows to vertex vector
    for (size_t row = 0; row < height_ / (sprSize.x * sprScale.x); row++)
    {
      GLuint rowY = row * (sprSize.x * sprScale.x) - height_ % (GLuint)(sprSize.x * sprScale.x) + 1;
      grid_.push_back(sf::Vertex(sf::Vector2f(0, rowY), gridColor));
      grid_.push_back(sf::Vertex(sf::Vector2f(width_, rowY), gridColor));
    }
    // Push grid columns to vertex vector
    for (size_t col = 0; col < width_ / (sprSize.y * sprScale.y); col++)
    {
      GLuint colX = col * (sprSize.y * sprScale.y) + 1;
      grid_.push_back(sf::Vertex(sf::Vector2f(colX, 0), gridColor));
      grid_.push_back(sf::Vertex(sf::Vector2f(colX, height_), gridColor));
    }
  }
}
