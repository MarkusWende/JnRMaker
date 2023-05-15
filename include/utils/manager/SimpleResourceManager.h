#include <string.h>
#include <iostream>
#include "IResourceManager.h"

class SimpleResourceManager : public IResourceManager {
public:
    void Load(const std::string& name) override {
        std::cout << "Loading resource: " << name << std::endl;
    }

    void Unload(const std::string& name) override {
        std::cout << "Unloading resource: " << name << std::endl;
    }
};