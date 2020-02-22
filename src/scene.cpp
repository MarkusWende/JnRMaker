#include "../include/scene.h"

Scene::Scene(GLuint width, GLuint height)
{
  ResourceManager::LoadTexture("assets/tiles/keen4_tiles.png", sf::Color(186 ,254, 202, 255), "tiles_keen4");

  ResourceManager::CreateRenderTexture(800, 800, "viewport");

  sf::Texture& tex = *ResourceManager::GetTexture("tiles_keen4");
  std::stringstream ss;
  ss << "Size: (" << tex.getSize().x << "|" << tex.getSize().y << ")";
  MessageManager::AddMessage(ss, message_t::INFO);
  ss << "Size: (" << tex.getSize().x << "|" << tex.getSize().y << ")";
  MessageManager::AddMessage(ss, message_t::ERROR);
  ss << "Size: (" << tex.getSize().x << "|" << tex.getSize().y << ")";
  MessageManager::AddMessage(ss, message_t::WARNING);
}

Scene::~Scene() { }
