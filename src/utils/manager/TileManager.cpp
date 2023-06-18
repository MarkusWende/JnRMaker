#include "TileManager.h"

TileManager::TileManager(std::shared_ptr<ILogger> logger)
{
    ui_logger_ = std::dynamic_pointer_cast<UILogger>(logger);
}

void
TileManager::Load(const char* name, const char* tilemapFIle, glm::vec2 size, glm::vec2 scale)
{
    ui_logger_->Log(log_t::DEBUG, "Loading tilemap: %s", name);
    if (tilemaps_.find(name) == tilemaps_.end())
    {
        tilemaps_[name] = std::make_shared<Tilemap>(name, size, scale, tilemapFIle);
    }
    else
    {
        std::stringstream msg;
        msg << "Tilemap with this name: " << name << " already exist.";
        ui_logger_->Log(log_t::WARNING, "%s", msg.str().c_str());
    }
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
    auto it = tilemaps_.find(name);
    if (it != tilemaps_.end()) {
        return it->second;
    }
    return nullptr; // or handle the case when tilemap_ is not found
}

std::shared_ptr<std::vector<std::string>>
TileManager::GetKeys()
{
    std::vector<std::string> keys;
    for (const auto& pair : tilemaps_)
    {
        keys.push_back(pair.first);
    }
    return std::make_shared<std::vector<std::string>>(keys);
}