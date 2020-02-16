#include <iostream>

#include "../include/resource_manager.h"

std::map<std::string, sf::Texture> ResourceManager::Textures;


void ResourceManager::LoadTexture(const GLchar* file, std::string name)
{
  sf::Texture texture;

  if (!texture.loadFromFile(file))
  {
    std::cout << "Error!!" << std::endl;
    // error...
  }
	Textures[name] = texture;
}

sf::Texture* ResourceManager::GetTexture(std::string name)
{
	return &Textures[name];
}



// PRIVATE:
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
