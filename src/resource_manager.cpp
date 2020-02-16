#include <iostream>
#include <SFML/Graphics/Image.hpp>

#include "../include/resource_manager.h"

std::map<std::string, sf::Texture> ResourceManager::Textures;
std::map<std::string, sf::RenderTexture*> ResourceManager::RenderTextures;


void ResourceManager::LoadTexture(const GLchar* file, sf::Color maskColor, std::string name)
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

void ResourceManager::CreateRenderTexture(GLuint width, GLuint height, std::string name)
{
  sf::RenderTexture rTexture;
  if(!rTexture.create(width, height))
  {
    std::cout << "Error!!" << std::endl;
    // error...
  }

  RenderTextures[name] = &rTexture;
}

sf::Texture* ResourceManager::GetTexture(std::string name)
{
	return &Textures[name];
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
