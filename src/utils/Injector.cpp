#include "Injector.h"

std::shared_ptr<ILogger> Injector::GetLogger()
{
    if (!logger_)
    {
        logger_ = std::make_shared<UILogger>();
    }
    return logger_;
}

std::shared_ptr<IResourceManager> Injector::GetGraphicsManager()
{
    if (!graphics_manager_)
    {
        graphics_manager_ = std::make_shared<GraphicsManager>();
    }
    return graphics_manager_;
}