#include "Injector.h"

std::shared_ptr<ILogger> Injector::GetLogger()
{
    if (!logger_)
        logger_ = std::make_shared<UILogger>();
    return logger_;
}

std::shared_ptr<Resources> Injector::GetResources()
{
    if (!resources_)
        resources_ = std::make_shared<Resources>(GetLogger());
    return resources_;
}