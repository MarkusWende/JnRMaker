#pragma once

#include <string>

template <typename T>
class IManager
{
public:
    virtual ~IManager() {}
    // Variadic template for Load with arbitrary number of parameters
    template<typename... Args>
    void Load(const char* name, Args&&... args)
    {
        // Implementation specific to the derived class
        // You can use the provided arguments (args) as needed
    }

    virtual void Unload(const char* name) = 0;

    virtual std::shared_ptr<T> Get(const char* name) = 0;
};