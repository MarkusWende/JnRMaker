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
 * @file tile_manager.cpp
 * @brief This file contains the singleton tile manager class implementation.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */

#include <sstream>

#include "../include/tilemap_manager.h"

std::map<std::string, Tilemap> TilemapManager::Tilemaps;

GLvoid TilemapManager::AddTilemap(std::string name, glm::vec2 size, glm::vec2 scale)
{
  Tilemaps.insert(std::make_pair(name, Tilemap(name, size, scale)));
}

GLvoid TilemapManager::AddTilemap(std::string name, glm::vec2 size, glm::vec2 scale, std::string file)
{
    Tilemaps.insert(std::make_pair(name, Tilemap(name, size, scale, file)));
}

Tilemap* TilemapManager::GetTilemap(std::string name)
{
	return &Tilemaps[name];
}
