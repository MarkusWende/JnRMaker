#pragma once

#include <string.h>
#include <iostream>
#include <memory>
#include <map>
#include <glm/glm.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/fetch.h>
#include <emscripten/websocket.h>
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#include <emscripten/val.h>
#endif

#include "IManager.h"
#include "UILogger.h"
#include "Tilemap.h"

#ifdef __EMSCRIPTEN__

#else
#include <nfd.h>
#endif

class TileManager : public IManager<Tilemap> {
public:
    TileManager(std::shared_ptr<ILogger> logger);
    void Load(const char* name, const char* tilemapFIle, glm::vec2 size, glm::vec2 scale);
#ifdef __EMSCRIPTEN__
    void LoadTilemap(std::string const& name, std::string const& type, emscripten::val data, int dataSize);
#else
    void LoadTilemap(std::string const& name, std::string const& type, const char* data, int dataSize);
#endif
    void Load();

    void Unload(const char* name) override;
    std::shared_ptr<Tilemap> Get(const char* name) override;
    std::shared_ptr<Tilemap> GetActive() { return tilemaps_[active_tilemap_]; };
    std::shared_ptr<std::vector<std::string>> GetKeys();

    void SetActive(const char* name) { active_tilemap_ = name; };

private:
    std::shared_ptr<UILogger> ui_logger_;
    std::map<std::string, std::shared_ptr<Tilemap>>		tilemaps_;
    std::string active_tilemap_;

};