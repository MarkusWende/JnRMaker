#include <string>

class IManager {
public:
    virtual ~IManager() {}
    // Variadic template for Load with arbitrary number of parameters
    template<typename... Args>
    void Load(const std::string& name, Args&&... args)
    {
        // Implementation specific to the derived class
        // You can use the provided arguments (args) as needed
    }

    virtual void Unload(const std::string& name) = 0;
};