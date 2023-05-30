#pragma once

#include "ShaderManager.h"
#include "UILogger.h"

class Resources
{
public:
    Resources(std::shared_ptr<ILogger> logger)
    {
        ui_logger_ = std::dynamic_pointer_cast<UILogger>(logger);
        shader_ = std::make_shared<ShaderManager>(logger);
        ui_logger_->Log("Loading shader..");
#ifdef __EMSCRIPTEN__
        shader_->Load("line", "resources/shaders/es/line.vert", "resources/shaders/es/line.frag");
        shader_->Load("llayer", "resources/shaders/es/level_layer.vert", "resources/shaders/es/level_layer.frag");
        shader_->Load("scene", "resources/shaders/es/scene.vert", "resources/shaders/es/scene.frag");
        shader_->Load("solid", "resources/shaders/es/solid.vert", "resources/shaders/es/solid.frag");
        shader_->Load("sprite", "resources/shaders/es/sprite.vert", "resources/shaders/es/sprite.frag");
#else
        shader_->Load("scene", "resources/shaders/scene.vert", "resources/shaders/scene.frag");
        shader_->Load("solid", "resources/shaders/solid.vert", "resources/shaders/solid.frag");
        shader_->Load("sprite", "resources/shaders/sprite.vert", "resources/shaders/sprite.frag");
        shader_->Load("line", "resources/shaders/line.vert", "resources/shaders/line.frag");
        shader_->Load("llayer", "resources/shaders/level_layer.vert", "resources/shaders/level_layer.frag");
        //shader_->Load("resources/shaders/tile.vert", "resources/shaders/tile.frag", nullptr, "tile");
#endif
    };
    ~Resources() {};

private:
    std::shared_ptr<ShaderManager> shader_;
    std::shared_ptr<UILogger> ui_logger_;

};