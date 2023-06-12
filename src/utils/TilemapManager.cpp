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

#include "TilemapManager.h"

std::map<std::string, Tilemap> TilemapManager::Tilemaps;

GLvoid TilemapManager::Add(std::string name, glm::vec2 size, glm::vec2 scale, std::string file)
{
    if (Tilemaps.find(name) == Tilemaps.end())
    {
        Tilemaps.insert(std::make_pair(name, Tilemap(name, size, scale, file)));
    }
    else
    {
        std::stringstream msg;
        msg << "Tilemap with this name: " << name << " already exist.";
        //MessageManager::AddMessage(msg, log_t::WARNING);
        //std::dynamic_pointer_cast<UILogger>(logger)->Popup(log_t::WARNING, "%s", msg.str().c_str());
    }
}

GLvoid TilemapManager::Add(std::string name, glm::vec2 size, glm::vec2 scale, const unsigned char *data, int dataSize)
{
    if (Tilemaps.find(name) == Tilemaps.end())
    {
        Tilemaps.insert(std::make_pair(name, Tilemap(name, size, scale, data, dataSize)));
    }
    else
    {
        std::stringstream msg;
        msg << "Tilemap with this name: " << name << " already exist.";
        //MessageManager::AddMessage(msg, log_t_t::WARNING, true);
        //std::dynamic_pointer_cast<UILogger>(logger)->Popup(log_t::WARNING, "%s", msg.str().c_str());
    }
}

Tilemap* TilemapManager::Get(std::string name)
{
	return &Tilemaps[name];
}

GLvoid TilemapManager::Delete(std::string name)
{
	Tilemaps.erase(name);
}