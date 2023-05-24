#include "GraphicsManager.h"

void GraphicsManager::Load(const std::string& name)
{
    std::cout << "Loading resource: " << name << std::endl;
}

void GraphicsManager::Unload(const std::string& name)
{
    std::cout << "Unloading resource: " << name << std::endl;
}