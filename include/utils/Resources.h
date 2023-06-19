#pragma once

#include "ShaderManager.h"
#include "TileManager.h"
#include "UILogger.h"

class Resources
{
public:
    Resources(std::shared_ptr<ILogger> logger);
    ~Resources() {};

    std::shared_ptr<Shader> GetShader(const char* name) { return shader_manager_->Get(name); };
    std::shared_ptr<Tilemap> GetTilemap(const char* name) { return tile_manager_->Get(name); };
    std::shared_ptr<Tilemap> GetActiveTilemap() { return tile_manager_->GetActive(); };
    std::shared_ptr<std::vector<std::string>> GetTileManagerKeys() { return tile_manager_->GetKeys(); }

    void SetActiveTilemap(const char* name) { tile_manager_->SetActive(name); };

    void AddTilemap(const char* name, const char* tilemapFIle, glm::vec2 size, glm::vec2 scale) { tile_manager_->Load(name, tilemapFIle, size, scale); };
    void AddTilemap() { tile_manager_->Load(); };

private:
    std::shared_ptr<ShaderManager> shader_manager_;
    std::shared_ptr<TileManager> tile_manager_;
    std::shared_ptr<UILogger> ui_logger_;

};