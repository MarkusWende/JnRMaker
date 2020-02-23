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
 * @file resource_manager.cpp
 * @brief This file contains the singleton resource manager class implementation.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#include "../include/resource_manager.h"

std::map<std::string, sf::Texture> ResourceManager::Textures;
std::map<std::string, std::unique_ptr<sf::RenderTexture>> ResourceManager::RenderTextures;


GLvoid ResourceManager::LoadTexture(const GLchar* file, sf::Color maskColor, std::string name)
{
  sf::Image image;
  if (!image.loadFromFile(file))
  {
    std::cout << "Error!!" << std::endl;
    // error...
  }

  image.createMaskFromColor(maskColor);

  sf::Texture texture;
  if (!texture.loadFromImage(image))
  {
    std::cout << "Error!!" << std::endl;
    // error...
  }

	Textures[name] = texture;
}

GLvoid ResourceManager::CreateRenderTexture(GLuint width, GLuint height, std::string name)
{
  RenderTextures.insert(std::make_pair(name, std::unique_ptr<sf::RenderTexture>(new sf::RenderTexture)));
  sf::RenderTexture* rTexture = RenderTextures.find(name)->second.get();
  if(!rTexture->create(width, height))
  {
    std::cout << "Error!!" << std::endl;
    // error...
  }
}

GLvoid ResourceManager::UpdateRenderTexture(GLuint width, GLuint height, std::string name)
{
  RenderTextures.erase(name);
  RenderTextures.insert(std::make_pair(name, std::unique_ptr<sf::RenderTexture>(new sf::RenderTexture)));
  sf::RenderTexture* rTexture = RenderTextures.find(name)->second.get();
  if(!rTexture->create(width, height))
  {
    std::cout << "Error!!" << std::endl;
    // error..
  }
}

sf::Texture* ResourceManager::GetTexture(std::string name)
{
	return &Textures[name];
}

sf::RenderTexture* ResourceManager::GetRenderTexture(std::string name)
{
	return RenderTextures.find(name)->second.get();
}


// PRIVATE:
////////////////////////////////////////////////////////////////////////////
sf::Texture ResourceManager::loadTextureFromFile(const GLchar* file)
{
	// Create Texture object
	sf::Texture texture;

  if (!texture.loadFromFile(file))
  {
    std::cout << "Error!!" << std::endl;
    // error...
  }

	return texture;
}
