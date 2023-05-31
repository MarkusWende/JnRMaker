#include "ShaderManager.h"

ShaderManager::ShaderManager(std::shared_ptr<ILogger> logger)
{
    ui_logger_ = std::dynamic_pointer_cast<UILogger>(logger);
}

void
ShaderManager::Load(const std::string& name, const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
    Load(name, vertexShaderFile, fragmentShaderFile, "");
}

void
ShaderManager::Load(const std::string& name, const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const std::string& geometryShaderFile)
{
    ui_logger_->Log(log_t::DEBUG, "Loading shader: %s", name.c_str());
}

void
ShaderManager::Unload(const std::string& name)
{
    ui_logger_->Log(log_t::DEBUG, "Unloading shader: %s", name.c_str());
}

std::shared_ptr<Shader>
ShaderManager::Get(const std::string& name)
{
    // Implementation to retrieve the shader by name
    auto it = shaders_.find(name);
    if (it != shaders_.end()) {
        return it->second;
    }
    return nullptr; // or handle the case when shader is not found
}