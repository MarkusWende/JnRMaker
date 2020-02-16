#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/OpenGL.hpp>

class ResourceManager
{
public:

  static std::map<std::string, sf::Texture> Textures;
  static std::map<std::string, sf::RenderTexture*> RenderTextures;

  static void LoadTexture(const GLchar* file, sf::Color maskColor, std::string name);
  static void CreateRenderTexture(GLuint width, GLuint height, std::string name);
  static sf::Texture* GetTexture(std::string name);

private:
  // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
  ResourceManager() { }
  ~ResourceManager() { }

  static sf::Texture loadTextureFromFile(const GLchar* file);
};

#endif
