#include "ShaderManager.h"

ShaderManager::ShaderManager(std::shared_ptr<ILogger> logger)
{
    ui_logger_ = std::dynamic_pointer_cast<UILogger>(logger);
}

void ShaderManager::Load(const std::string& name)
{
    std::cout << "Loading resource: " << name << std::endl;
}

void ShaderManager::Unload(const std::string& name)
{
    std::cout << "Unloading resource: " << name << std::endl;
}