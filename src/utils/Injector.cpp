#include "Injector.h"

std::shared_ptr<ILogger> Injector::GetLogger()
{
    if (!logger_)
        logger_ = std::make_shared<UILogger>();
    return logger_;
}

std::shared_ptr<IManager> Injector::GetShaderManager()
{
    if (!shader_manager_)
        shader_manager_ = std::make_shared<ShaderManager>(GetLogger());
    return shader_manager_;
}