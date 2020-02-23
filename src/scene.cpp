#include "../include/scene.h"

Scene::Scene(GLuint width, GLuint height)
{
  width_ = width;
  height_ = height;

  // Load textures
  ResourceManager::LoadTexture("assets/tiles/keen4_tiles.png", sf::Color(186 ,254, 202, 255), "tiles_keen4");

  // Create framebuffers
  ResourceManager::CreateRenderTexture(width_, height_, "viewport");

  sf::Texture& tex = *ResourceManager::GetTexture("tiles_keen4");
  std::stringstream ss;
  ss << "Size: (" << tex.getSize().x << "|" << tex.getSize().y << ")";
  MessageManager::AddMessage(ss, message_t::INFO);
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
