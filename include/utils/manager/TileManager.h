#pragma once

#include <string.h>
#include <iostream>
#include <memory>
#include <map>

#include "IManager.h"
#include "UILogger.h"
#include "Tilemap.h"

class TileManager : public IManager<Tilemap> {
public:
    TileManager(std::shared_ptr<ILogger> logger);
    void Load(const char* name, const char* tilemapFIle);
    void Unload(const char* name) override;
    std::shared_ptr<Tilemap> Get(const char* name) override;

private:
    std::shared_ptr<UILogger> ui_logger_;
    std::map<std::string, std::shared_ptr<Tilemap>>		tilemap_;							/**< Shader vector. Stores every shader used in the application. */
};