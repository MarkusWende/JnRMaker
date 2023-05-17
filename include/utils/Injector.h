#include <memory>
#include <iostream>
#include "UILogger.h"
#include "SimpleResourceManager.h"

class Injector {
public:
    template<typename T, typename... Args>
        std::shared_ptr<T> Create(std::shared_ptr<ILogger> logger, Args&&... args)
        {
            return std::make_shared<T>(logger, std::forward<Args>(args)...);
        };

    std::shared_ptr<ILogger>            GetLogger();
    std::shared_ptr<IResourceManager>   GetResourceManager();

private:
    std::shared_ptr<ILogger> logger_;
    std::shared_ptr<IResourceManager> resource_manager_;
};