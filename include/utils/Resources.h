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
    std::shared_ptr<std::vector<std::string>> GetTileManagerKeys() { return tile_manager_->GetKeys(); }

private:
    std::shared_ptr<ShaderManager> shader_manager_;
    std::shared_ptr<TileManager> tile_manager_;
    std::shared_ptr<UILogger> ui_logger_;

};