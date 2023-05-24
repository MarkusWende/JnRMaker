#include <memory>
#include <iostream>
#include "UILogger.h"
#include "GraphicsManager.h"

class Injector {
public:
    template<typename T, typename... Args>
        std::shared_ptr<T> Create(Args&&... args)
        {
            return std::make_shared<T>(std::forward<Args>(args)...);
        };

    std::shared_ptr<ILogger>            GetLogger();
    std::shared_ptr<IResourceManager>   GetGraphicsManager();

private:
    std::shared_ptr<ILogger> logger_;
    std::shared_ptr<IResourceManager> graphics_manager_;
    std::shared_ptr<IResourceManager> audio_manager_;
};