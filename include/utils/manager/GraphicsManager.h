#pragma once

#include <string.h>
#include <iostream>
#include "IResourceManager.h"

class GraphicsManager : public IResourceManager {
public:
    void Load(const std::string& name) override;

    void Unload(const std::string& name) override;
};