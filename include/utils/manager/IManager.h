#include <string>

class IManager {
public:
    virtual ~IManager() {}
    virtual void Load(const std::string& name) = 0;
    virtual void Unload(const std::string& name) = 0;
};