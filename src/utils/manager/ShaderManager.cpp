#include "ShaderManager.h"

ShaderManager::ShaderManager(std::shared_ptr<ILogger> logger)
{
    ui_logger_ = std::dynamic_pointer_cast<UILogger>(logger);
}

void
ShaderManager::Load(const std::string& name)
{
    Load(name, "", "", "");
}

void
ShaderManager::Load(const std::string& name, const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
    Load(name, vertexShaderFile, fragmentShaderFile, "");
}

void
ShaderManager::Load(const std::string& name, const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const std::string& geometryShaderFile)
{
    ui_logger_->Log("Loading resource: %s", name.c_str());
}

void ShaderManager::Unload(const std::string& name)
{
    ui_logger_->Log("Unloading resource: %s", name.c_str());
}