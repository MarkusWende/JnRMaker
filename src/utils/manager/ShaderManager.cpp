#include "ShaderManager.h"

ShaderManager::ShaderManager(std::shared_ptr<ILogger> logger)
{
    ui_logger_ = std::dynamic_pointer_cast<UILogger>(logger);
}

void
ShaderManager::Load(const char* name, const char* vShaderFile, const char* fShaderFile)
{
    Load(name, vShaderFile, fShaderFile, nullptr);
}

void
ShaderManager::Load(const char* name, const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    ui_logger_->Log(log_t::DEBUG, "Loading shader: %s", name);
    shaders_[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
}

void
ShaderManager::Unload(const char* name)
{
    ui_logger_->Log(log_t::DEBUG, "Unloading shader: %s", name);
}

std::shared_ptr<Shader>
ShaderManager::Get(const char* name)
{
    // Implementation to retrieve the shader by name
    auto it = shaders_.find(name);
    if (it != shaders_.end()) {
        return it->second;
    }
    return nullptr; // or handle the case when shader is not found
}


/// PRIVATE
std::shared_ptr<Shader>
ShaderManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (...)
    {
        FILE *stream;
#if _WIN32
        freopen_s(&stream, "log.txt", "w", stdout);
        fprintf(stream, "\tFailed to read shader files\n");
        fclose(stream);
#elif __linux__
        stream = fopen("./log.txt", "w");
        fprintf(stream, "\tFailed to read shader files\n");
        fclose(stream);
#else
        (void)stream;
#endif
        delete stream;
    }
    auto vShaderCode = vertexCode.c_str();
    auto fShaderCode = fragmentCode.c_str();
    auto gShaderCode = geometryCode.c_str();
    // 2. Now create shader object from source code
    std::shared_ptr<Shader> shader = std::make_shared<Shader>(ui_logger_);
    shader->Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}