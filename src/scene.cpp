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
  ResourceManager::LoadTexture("assets/tiles/keen4_tiles.png", sf::Color(186 ,254, 202, 255), "tiles_keen4");
  TilemapManager::AddTilemap("tiles_keen4", 16, 16, { 2.0f, 2.0f });

  // Create framebuffers
  ResourceManager::CreateRenderTexture(width_, height_, "viewport");
}

Scene::~Scene() { }

GLvoid Scene::Update()
{
  ResourceManager::UpdateRenderTexture(width_, height_, "viewport");
}

GLvoid Scene::Render()
{
  sf::RenderTexture* texViewport = ResourceManager::GetRenderTexture("viewport");
  texViewport->clear(sf::Color::Black);
}
