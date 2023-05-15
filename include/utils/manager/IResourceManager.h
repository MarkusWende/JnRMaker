#include <string>

class IResourceManager {
public:
    virtual ~IResourceManager() {}
    virtual void Load(const std::string& name) = 0;
    virtual void Unload(const std::string& name) = 0;
};