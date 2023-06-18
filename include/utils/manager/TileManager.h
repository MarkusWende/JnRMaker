#pragma once

#include <string.h>
#include <iostream>
#include <memory>
#include <map>
#include <glm/glm.hpp>

#include "IManager.h"
#include "UILogger.h"
#include "Tilemap.h"

class TileManager : public IManager<Tilemap> {
public:
    TileManager(std::shared_ptr<ILogger> logger);
    void Load(const char* name, const char* tilemapFIle, glm::vec2 size, glm::vec2 scale);
    void Unload(const char* name) override;
    std::shared_ptr<Tilemap> Get(const char* name) override;
    std::shared_ptr<std::vector<std::string>> GetKeys();

private:
    std::shared_ptr<UILogger> ui_logger_;
    std::map<std::string, std::shared_ptr<Tilemap>>		tilemaps_;
    std::string active_tilemap_;
};