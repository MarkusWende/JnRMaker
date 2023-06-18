#include "Resources.h"

Resources::Resources(std::shared_ptr<ILogger> logger)
{
    ui_logger_ = std::dynamic_pointer_cast<UILogger>(logger);
    shader_manager_ = std::make_shared<ShaderManager>(logger);
    tile_manager_ = std::make_shared<TileManager>(logger);

#ifdef __EMSCRIPTEN__
    shader_manager_->Load("line", "resources/shaders/es/line.vert", "resources/shaders/es/line.frag");
    shader_manager_->Load("llayer", "resources/shaders/es/level_layer.vert", "resources/shaders/es/level_layer.frag");
    shader_manager_->Load("scene", "resources/shaders/es/scene.vert", "resources/shaders/es/scene.frag");
    shader_manager_->Load("solid", "resources/shaders/es/solid.vert", "resources/shaders/es/solid.frag");
    shader_manager_->Load("sprite", "resources/shaders/es/sprite.vert", "resources/shaders/es/sprite.frag");
#else
    shader_manager_->Load("scene", "resources/shaders/scene.vert", "resources/shaders/scene.frag");
    shader_manager_->Load("solid", "resources/shaders/solid.vert", "resources/shaders/solid.frag");
    shader_manager_->Load("sprite", "resources/shaders/sprite.vert", "resources/shaders/sprite.frag");
    shader_manager_->Load("line", "resources/shaders/line.vert", "resources/shaders/line.frag");
    shader_manager_->Load("llayer", "resources/shaders/level_layer.vert", "resources/shaders/level_layer.frag");
    //shader_manager_->Load("resources/shaders/tile.vert", "resources/shaders/tile.frag", nullptr, "tile");
#endif
    tile_manager_->Load("resources/assets/tiles/game-tiles_cut.png", "resources/assets/tiles/game-tiles_cut.png", { 16, 16 }, { 1.0f, 1.0f });
}