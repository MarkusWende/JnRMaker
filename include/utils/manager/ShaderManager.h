#pragma once

#include <string.h>
#include <iostream>
#include <memory>
#include <map>

#include "IManager.h"
#include "UILogger.h"
#include "Shader.h"

class ShaderManager : public IManager {
public:
    ShaderManager(std::shared_ptr<ILogger> logger);
    void Load(const std::string& name) override;
    void Unload(const std::string& name) override;

private:
    std::shared_ptr<UILogger> ui_logger_;
    std::map<std::string, std::shared_ptr<Shader>>		shaders_;							/**< Shader vector. Stores every shader used in the application. */
};