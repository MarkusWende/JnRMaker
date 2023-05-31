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
    void Load(const std::string& name, const std::string& tilemapFIle);
    void Unload(const std::string& name) override;
    std::shared_ptr<Tilemap> Get(const std::string& name) override;

private:
    std::shared_ptr<UILogger> ui_logger_;
    std::map<std::string, std::shared_ptr<Tilemap>>		tilemap_;							/**< Shader vector. Stores every shader used in the application. */
};