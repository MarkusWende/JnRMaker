#include "Injector.h"

std::shared_ptr<ILogger> Injector::GetLogger()
{
    if (!logger_)
    {
        logger_ = std::make_shared<UILogger>();
    }
    return logger_;
}

std::shared_ptr<IResourceManager> Injector::GetResourceManager()
{
    if (!resource_manager_)
    {
        resource_manager_ = std::make_shared<SimpleResourceManager>();
    }
    return resource_manager_;
}