#include "TileManager.h"

TileManager::TileManager(std::shared_ptr<ILogger> logger)
{
    ui_logger_ = std::dynamic_pointer_cast<UILogger>(logger);
}

void
TileManager::Load(const char* name, const char* tilemapFIle)
{
    ui_logger_->Log(log_t::DEBUG, "Loading tilemap: %s", name);
}

void
TileManager::Unload(const char* name)
{
    ui_logger_->Log(log_t::DEBUG, "Unloading tilemap: %s", name);
}

std::shared_ptr<Tilemap>
TileManager::Get(const char* name)
{
    // Implementation to retrieve the tilemap_ by name
    auto it = tilemap_.find(name);
    if (it != tilemap_.end()) {
        return it->second;
    }
    return nullptr; // or handle the case when tilemap_ is not found
}