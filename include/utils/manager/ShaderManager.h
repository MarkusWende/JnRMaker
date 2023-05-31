#pragma once

#include <string.h>
#include <iostream>
#include <memory>
#include <map>
#include <fstream>
#include <sstream>

#include "IManager.h"
#include "UILogger.h"
#include "Shader.h"

class ShaderManager : public IManager<Shader>
{
public:
    ShaderManager(std::shared_ptr<ILogger> logger);
    void Load(const char* name, const char* vertexShaderFile, const char* fragmentShaderFile);
    void Load(const char* name, const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile);
    void Unload(const char* name) override;
    std::shared_ptr<Shader> Get(const char* name) override;

private:
    std::shared_ptr<Shader> loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile);

    std::shared_ptr<UILogger>                           ui_logger_;
    std::map<std::string, std::shared_ptr<Shader>>		shaders_;							/**< Shader map. Stores every shader used in the application. */
};