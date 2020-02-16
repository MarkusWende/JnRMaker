#include "../include/scene.h"
#include "../include/resource_manager.h"

Scene::Scene(GLuint width, GLuint height)
{
  ResourceManager::LoadTexture("assets/tiles/keen4_tiles.png", sf::Color(186 ,254, 202, 255), "face");

  ResourceManager::CreateRenderTexture(800, 800, "viewport");
}

Scene::~Scene() { }
