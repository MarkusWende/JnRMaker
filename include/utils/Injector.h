#include <memory>
#include <iostream>
#include "UILogger.h"
#include "ShaderManager.h"

class Injector {
public:
    template<typename T, typename... Args>
        std::shared_ptr<T> Create(Args&&... args)
        {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }

    std::shared_ptr<ILogger>            GetLogger();
    std::shared_ptr<IManager>           GetShaderManager();
    //std::shared_ptr<IManager>           GetResources();

private:
    std::shared_ptr<ILogger> logger_;
    std::shared_ptr<IManager> shader_manager_;
    //std::shared_ptr<IManager<Audio>> audio_manager_;
    //std::shared_ptr<ResourceManagerOld> ResourceManagerOld_;
};