/**
 * JnRMaker - A tile editor and jump and run game maker
 * See COPYRIGHT file at the top of the source tree.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the JnRMaker License Statement and
 * the GNU General Public License along with this program.
 *
 */

/**
 * @file scene.cpp
 * @brief This file contains the scene class implementation.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#include "Scene.h"

Scene::Scene(std::shared_ptr<ILogger> logger, std::shared_ptr<Resources> resources, GLuint width, GLuint height)
{
    ui_logger_ = std::dynamic_pointer_cast<UILogger>(logger);
	resources_ = resources;

    width_ = width;
    height_ = height;

    add_sprite_flag_ = false;
    mouse_over_scene_ = false;
    mouse_inside_level_ = false;
    map_is_null_ = true;
    map_height_ = 0;
    map_width_ = 0;
    sprite_size_ = glm::vec2(0);
    sprite_scale_ = glm::vec2(0);
    level_border_size_ = 4;
    current_tile_id_ = 0;
    active_layer_ = layer_t::BACK;

	/* std::stringstream msg;
	msg << "scene.cpp: " << fs::current_path().string().c_str();
	MessageManager::AddMessage(msg, message_t::INFO);
    */

    //chdir(fs::current_path().parent_path().string().c_str());
    //std::cout << fs::current_path().parent_path().string().c_str();

// #ifdef __EMSCRIPTEN__
//     ResourceManagerOld::LoadShader("resources/shaders/es/line.vert", "resources/shaders/es/line.frag",  nullptr, "line");
//     ResourceManagerOld::LoadShader("resources/shaders/es/level_layer.vert", "resources/shaders/es/level_layer.frag",  nullptr, "llayer");
//     ResourceManagerOld::LoadShader("resources/shaders/es/scene.vert", "resources/shaders/es/scene.frag", nullptr, "scene");
//     ResourceManagerOld::LoadShader("resources/shaders/es/solid.vert", "resources/shaders/es/solid.frag", nullptr, "solid");
//     ResourceManagerOld::LoadShader("resources/shaders/es/sprite.vert", "resources/shaders/es/sprite.frag", nullptr, "sprite");
// #else
//     ResourceManagerOld::LoadShader("resources/shaders/scene.vert", "resources/shaders/scene.frag", nullptr, "scene");
//     ResourceManagerOld::LoadShader("resources/shaders/solid.vert", "resources/shaders/solid.frag", nullptr, "solid");
//     ResourceManagerOld::LoadShader("resources/shaders/sprite.vert", "resources/shaders/sprite.frag", nullptr, "sprite");
//     ResourceManagerOld::LoadShader("resources/shaders/line.vert", "resources/shaders/line.frag",  nullptr, "line");
//     ResourceManagerOld::LoadShader("resources/shaders/level_layer.vert", "resources/shaders/level_layer.frag",  nullptr, "llayer");
//     //ResourceManagerOld::LoadShader("resources/shaders/tile.vert", "resources/shaders/tile.frag", nullptr, "tile");
// #endif
    // Testing
    //TilemapManager::Add("resources/assets/tiles/game-tiles_cut.png", { 16, 16 }, { 1.0f, 1.0f }, "resources/assets/tiles/game-tiles_cut.png");


    //ResourceManagerOld::CreateRenderTexture(width_, height_, "viewport");
    //ResourceManagerOld::CreateRenderTexture(width_, height_, "minimap");
    filter_.insert(std::make_pair("asdasd", new Entity(logger, resources)));

    e_solids_.insert(std::make_pair("TestCube", new Cube(logger, resources, "TestCube", false)));
    e_solids_.insert(std::make_pair("TestQuad", new Quad(logger, resources, "TestQuad")));
    //e_solids_.insert(std::make_pair("TestGrid", new Grid("TestGrid", true)));
    e_cs_.insert(std::make_pair("MainCS", new CoordinateSystem(logger, resources, glm::vec3(0))));

    e_cameras_.insert(std::make_pair("SceneCamera", new Camera(logger, resources)));
    //e_cameras_["SceneCamera"]->SetState(CameraState::ORTHOGRAPHIC);

    // Create framebuffers
    //ResourceManagerOld::CreateRenderTexture(width_, height_, "viewport");
    //generateGrid();
    ResourceManagerOld::CreateFramebuffer(logger, "scene", width_, height_, GL_TEXTURE_2D);
    //ResourceManagerOld::CreateFramebuffer("minimap", width_, height_);
    ResourceManagerOld::CreateFramebuffer(logger, "imguiScene", width_, height_, GL_TEXTURE_2D);

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
       // positions   // texCoords
       -1.0f,  1.0f,  0.0f, 1.0f,
       -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

       -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
    GLuint sceneVBO;
    // Configurate scene quad VAO
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &sceneVBO);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, sceneVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

Scene::~Scene() { }

GLvoid Scene::CreateLevel(GLuint width, GLuint height, glm::vec2 spriteSize, glm::vec2 spriteScale)
{
    map_is_null_ = true;
    // Delete grid if it exists and create new one
    if (!e_grids_.empty())
    {
        e_grids_.clear();
    }
    e_grids_.insert(std::make_pair("XYGrid", new Grid(ui_logger_, resources_, "XYGrid", true, width, height)));
    map_width_ = width;
    map_height_ = height;
    sprite_size_ = spriteSize;
    sprite_scale_ = spriteScale;

    // Move camera to the center of the grid
    e_cameras_["SceneCamera"]->Move(((GLfloat)width)/2.0f, ((GLfloat)height)/2.0f);

    // Load default sprites
    // std::stringstream keyEmpty, keyBorder;
    // keyEmpty << "default_empty_" << spriteSize.x << "x" << spriteSize.y;
    // keyBorder << "default_border_" << spriteSize.x << "x" << spriteSize.y;
    // std::stringstream fileDefaultEmpty;
    // std::stringstream fileDefaultBorder;
    // fileDefaultEmpty << "resources/assets/sprites/" << keyEmpty.str().c_str() << ".png";
    // fileDefaultBorder << "resources/assets/sprites/" << keyBorder.str().c_str() << ".png";
    // ResourceManagerOld::LoadTexture(fileDefaultEmpty.str().c_str(), GL_TRUE, keyEmpty.str().c_str());
    // ResourceManagerOld::LoadTexture(fileDefaultBorder.str().c_str(), GL_TRUE, keyBorder.str().c_str());

    // Create level
    if (!e_level_layers_.empty())
    {
        e_level_layers_.clear();
    }
    e_level_layers_.insert(std::make_pair("Tiles", new WorldLayer(ui_logger_, resources_, "Tiles", width, height, spriteSize, level_border_size_)));
    // active_tilemap_name_ = "Tiles";

    // Create default brush
    e_sprites_.clear();
    e_sprites_.insert(std::make_pair("brush", new Sprite(ui_logger_, resources_, "brush", false, (GLuint)spriteSize.x, (GLuint)spriteSize.y)));
    std::string keyBorderHash = ResourceManagerOld::GetNameHash("Tiles", "r0c1");
    e_sprites_.find("brush")->second->AssignTextureID(ResourceManagerOld::GetTexture(keyBorderHash.c_str()).ID);
    active_sprite_name_ = keyBorderHash.c_str();

    map_is_null_ = false;
}

GLvoid Scene::ResizeLevel(GLuint width, GLuint height)
{
    (void)width;
    (void)height;

    if (map_is_null_)
    {
        std::stringstream msg;
        msg << "There is no level to resize..";
        ui_logger_->Log(log_t::WARNING, "%s", msg.str().c_str());
    }
    else
    {
        /* code */
    }
    
}

GLvoid Scene::Update(GLuint width, GLuint height)
{
    //ResourceManagerOld::ResizeRenderTexture(width_, height_, "viewport");
    if ((width_ != width) || (height_ != height))
    {
        width_ = width;
        height_ = height;

        ResourceManagerOld::ResizeFramebuffer("scene", width_, height_);
        ResourceManagerOld::ResizeFramebuffer("imguiScene", width_, height_);

    }

    //e_cameras_["Editor"]->Center(width_ / 2, height_ / 2);
    //e_cameras_["Editor"]->Resize(width_, height_);
}

GLvoid Scene::Render()
{
    // Render everything to the scene framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, ResourceManagerOld::GetFramebuffer("scene").GetID());
    glViewport(0, 0, width_, height_); // Render on the whole framebuffer, complete from the lower left corner to the upper right

///////////////////////////////////////////////////////////////////////////////////////////////

    // Background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!


    // Create transformations
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = e_cameras_["SceneCamera"]->GetViewMatrix();
    //glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 3.0f) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Initialize projection matrix, wheter orthographic or perspective depending on the camera state
    if (e_cameras_["SceneCamera"]->GetState() == CameraState::PERSPECTIVE)
        projection = glm::perspective(glm::radians(e_cameras_["SceneCamera"]->GetZoom()), (float)width_ / (float)height_, 0.1f, 300.0f);
    if (e_cameras_["SceneCamera"]->GetState() == CameraState::ORTHOGRAPHIC)
    {
        projection = glm::ortho(
            -1.0f * (float)width_ / (float)height_ * e_cameras_["SceneCamera"]->GetZoom(),
            (float)width_ / (float)height_ * e_cameras_["SceneCamera"]->GetZoom(),
            -1.0f * e_cameras_["SceneCamera"]->GetZoom(),
            1.0f * e_cameras_["SceneCamera"]->GetZoom(),
            0.0f,
            300.0f
        );
    }

    // Get the mouse ray for selecting objects and interacting with them
    if (e_cameras_["SceneCamera"]->GetState() == CameraState::PERSPECTIVE)
    {
        mouse_ray_start_ = glm::unProject(glm::vec3(mouse_position_.x-3, mouse_position_.y-45, 0.1f), view, projection, glm::vec4(0, 0, width_, height_));
        mouse_ray_end_ = glm::unProject(glm::vec3(mouse_position_.x-3, mouse_position_.y-45, 300.0f), view, projection, glm::vec4(0, 0, width_, height_));
    }
    else if (e_cameras_["SceneCamera"]->GetState() == CameraState::ORTHOGRAPHIC)
    {
        mouse_ray_start_ = glm::unProject(glm::vec3(mouse_position_.x-3, mouse_position_.y-45, 0.0f), view, projection, glm::vec4(0, 0, width_, height_));
        mouse_ray_end_ = glm::unProject(glm::vec3(mouse_position_.x-3, mouse_position_.y-45, 300.0f), view, projection, glm::vec4(0, 0, width_, height_));
    }


    //std::stringstream msg;
    //msg << "x: " << mouse_ray_start_.x << "\ty: " << mouse_ray_start_.y << "\tz: " << mouse_ray_start_.z;
    //MessageManager::AddMessage(msg, message_t::INFO);

    // Grid
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    {
        if (e_grids_.find("XYGrid") != e_grids_.end())
        {
            e_grids_["XYGrid"]->Draw(projection, view);
        }
    }


    // Coordinate System
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    {
        if (e_cameras_["SceneCamera"]->GetState() == CameraState::ORTHOGRAPHIC)
            glDisable(GL_DEPTH_TEST);

        e_cs_["MainCS"]->Draw(projection, view);

        if (e_cameras_["SceneCamera"]->GetState() == CameraState::ORTHOGRAPHIC)
            glEnable(GL_DEPTH_TEST);
    }

    // Sprite tools
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Brush
    //std::string brushKey = ResourceManagerOld::getNameHash(active_tilemap_name_, active_sprite_name_);

    //Tilemap* tilemap = TilemapManager::GetTilemap(active_tilemap_name_);

    if (e_sprites_.find("brush") != e_sprites_.end())
    {
        int cubePosX = (int)mouse_ray_start_.x;
        int cubePosY = (int)mouse_ray_start_.y;
        float xSign = 0.0f;
        float ySign = 0.0f;
        if (mouse_ray_start_.x < 0.0f)
            xSign = -1.0f;
        if (mouse_ray_start_.y < 0.0f)
            ySign = -1.0f;

        glm::vec2 tileField = glm::vec2((float)cubePosX + xSign, (float)cubePosY + ySign);

        if (tileField.x >= 0 && tileField.y >= 0 && tileField.x < (map_width_) && tileField.y < (map_height_))
        {
            glm::vec3 brushPos = glm::vec3((float)cubePosX + xSign + 0.5f, (float)cubePosY + ySign + 0.5f, 0.0f);
            e_sprites_["brush"]->Move(brushPos);
            e_sprites_["brush"]->Draw(projection, view);

            current_tile_id_ = ((GLuint)(tileField.y) + level_border_size_) * (map_width_ + level_border_size_ * 2) + ((GLuint)(tileField.x) + level_border_size_);
            mouse_inside_level_ = true;
        }
        else
        {
            mouse_inside_level_ = false;
        }
    }
    // Draw level
    if (e_level_layers_.find("Tiles") != e_level_layers_.end())
    {
        e_level_layers_["Tiles"]->Draw(projection, view);
    }


    // DEVELOPEMENT BLOCK
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (true)
    {
        if (e_cameras_["SceneCamera"]->GetState() == CameraState::PERSPECTIVE)
        {
            e_cameras_["SceneCamera"]->DrawCenter(projection, view);
        }
        
        /* ResourceManagerOld::GetShader("floor").Use();
        ResourceManagerOld::GetShader("floor").SetMatrix4("projection", projection);
        ResourceManagerOld::GetShader("floor").SetMatrix4("view", view);

        ResourceManagerOld::GetShader("floor").SetVector3f("dirLight.direction", -0.3f, -1.0f, -0.1f);
        ResourceManagerOld::GetShader("floor").SetVector3f("dirLight.ambient", 0.5f, 0.5f, 0.5f);
        ResourceManagerOld::GetShader("floor").SetVector3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        ResourceManagerOld::GetShader("floor").SetVector3f("color", 0.5f, 0.5f, 0.5f);
        ResourceManagerOld::GetShader("floor").SetFloat("alpha", 1.0f); */

        //model = glm::translate(model, glm::vec3(0.0, 0.1f, 0.0));
        //model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)); // a smaller cube
        //ResourceManagerOld::GetShader("floor").SetMatrix4("model", model);

        //glm::vec3 pos(0.0f);
        //glm::vec3 norm(0.0f);
        //if (glm::intersectRaySphere(mouseRayEnd, glm::normalize(mouseRayStart - mouseRayEnd), glm::vec3(1.0f, 0.1f, 0.0f), 1.0f / M.MapModel.GetDistance(), pos, norm))
        //if (cube->Intersection(mouseRayEnd, glm::normalize(mouseRayStart - mouseRayEnd)))
        {
            //	ResourceManagerOld::GetShader("floor").SetVector3f("color", 0.0, 1.0, 0.0);
        }
        //glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f);
        //if (M.MapModel.Intersection(mouseRayEnd, glm::normalize(mouseRayStart - mouseRayEnd), offset))
        {
            //glm::vec3 center = entities_cameras_["SceneCamera"]->GetCenter();
            //std::stringstream ss;
            //ss << "Offset: (" << offset.x << "|" << offset.y << "|" << offset.z << ")\t\tCameraCenter: (" << center.x << "|" << center.y << "|" << center.z << ")";
            //MessageManager::AddMessage(ss , MESSAGE_TYPE_INFO);
        }
        
        //glm::vec4 col = { 1.0f, 0.2f, 0.2f, 1.0f };
        //int cubePosX = (int)mouse_ray_start_.x;
        //int cubePosY = (int)mouse_ray_start_.y;
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(5.0f, 5.0f, 0.5f));
        // //model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f)); // a smaller cube
        // ResourceManagerOld::GetShader("tile").Use();
        // ResourceManagerOld::GetShader("tile").SetMatrix4("projection", projection);
        // ResourceManagerOld::GetShader("tile").SetMatrix4("view", view);
        // ResourceManagerOld::GetShader("tile").SetMatrix4("model", model);
        // ResourceManagerOld::GetShader("tile").SetFloat("aLayer", 1.0f);
        // glBindTexture(GL_TEXTURE_2D_ARRAY, ResourceManagerOld::GetTextureAtlas("default").ID);
        //e_solids_["TestQuad"]->Draw();

        
        
        //glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f);
        //M.MapModel.Intersection({ 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, offset);

        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // a smaller cube
        //ResourceManagerOld::GetShader("floor").SetMatrix4("model", model);
        //entities_solids_["TestCube2"]->DrawWireframe();
        //cube->DrawWithLines(ResourceManagerOld::GetShader("floor"), col);

        //entities_solids_["test"]->Draw();
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)); // a smaller cube


        //e_solids_["TestCube"]->DrawWireframe();
        /*
        const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "void main()\n"
            "{\n"
            "   //gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "   gl_Position = vec4(aPos, 1.0);\n"
            "}\0";
        const char* fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\n\0";

        // build and compile our shader program
    // ------------------------------------
    // vertex shader
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // link shaders
        int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            -0.1f, -0.1f, 0.0f, // left
             0.1f, -0.1f, 0.0f, // right
             0.0f,  0.1f, 0.0f  // top
        };

        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        // render
        // ------
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
         */
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    /*
    if (false)
    {
        sf::RenderTexture* texViewport = ResourceManagerOld::GetRenderTexture("viewport");
        sf::RenderTexture* texMinimap = ResourceManagerOld::GetRenderTexture("minimap");
        texViewport->clear(sf::Color::Black);
        texMinimap->clear(sf::Color::Black);

        glm::vec2 sprSize = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteSize();
        glm::vec2 sprScale = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteScale();

        sf::Sprite* spr = TilemapManager::GetTilemap(active_tilemap_name_)->GetSprite(active_sprite_name_);
        sf::RectangleShape rectangle(sf::Vector2f(32, 32));
        rectangle.setFillColor(sf::Color(0, 0, 0, 0));
        rectangle.setOutlineColor(sf::Color::Green);
        rectangle.setOutlineThickness(2);

        if (mouse_over_scene_)
        {
            //sf::Vector2f worldPos = texViewport->mapPixelToCoords({ mouse_position_.x, mouse_position_.y - 48 });
            sf::Vector2f worldPos = {0,0};
            sf::Vector2i sprPos(worldPos);

            GLuint row = sprPos.y / (sprSize.y * sprScale.y);
            GLuint col = sprPos.x / (sprSize.x * sprScale.x);

            GLuint y = row * (sprSize.y * sprScale.y);
            GLuint x = col * (sprSize.x * sprScale.x);

            spr->setPosition(sf::Vector2f(e_solids_.size() * (sprSize.y * sprScale.y), 0));

            if (add_sprite_flag_)
            {
                std::string hashSolid = getNameHash(active_tilemap_name_, active_sprite_name_);
                std::map<std::string, std::unique_ptr<Solid>>::iterator itSolid;
                itSolid = e_solids_.find(hashSolid);
                GLuint idSolid = 0;

                if (itSolid != e_solids_.end())
                {
                    idSolid = e_solids_.find(hashSolid)->second.get()->GetId();
                }
                else
                {
                    idSolid = e_solids_.size() + 1;
                    e_solids_.insert(std::make_pair(hashSolid, new Solid(idSolid, hashSolid, active_layer_)));
                    sf::RenderTexture* renderTex = ResourceManagerOld::GetRenderTexture("tex_used_tiles");
                    sf::Texture tex = renderTex->getTexture();
                    sf::Sprite sprTexOld;
                    sprTexOld.setOrigin(0, 32);
                    sprTexOld.setScale(1.0f, -1.0f);
                    sprTexOld.setTexture(tex);
                    ResourceManagerOld::ResizeRenderTexture((idSolid) * (sprSize.x * sprScale.x), (sprSize.y * sprScale.y), "tex_used_tiles");
                    sf::RenderTexture* newRenderTex = ResourceManagerOld::GetRenderTexture("tex_used_tiles");
                    //newRenderTex->clear(sf::Color::Black);
                    newRenderTex->draw(sprTexOld);
                    newRenderTex->draw(*spr);

                    //std::stringstream msg;
                    //msg << "hash: " << hashSolid << "id: " << idSolid;
                    //MessageManager::AddMessage(msg, message_t::INFO);
                }

                if ((active_layer_ == layer_t::FORE) && (row < map_fg_.size()) && (col < map_fg_.at(0).size()))
                {
                    map_fg_.at(row).at(col) = idSolid;
                }
                else if ((active_layer_ == layer_t::PLAYER) && (row < map_pg_.size()) && (col < map_pg_.at(0).size()))
                {
                    map_pg_.at(row).at(col) = idSolid;
                }
                else if ((active_layer_ == layer_t::BACK) && (row < map_bg_.size()) && (col < map_bg_.at(0).size()))
                {
                    map_bg_.at(row).at(col) = idSolid;
                }

                updateLayerVAO(active_layer_);

                add_sprite_flag_ = false;
            }

            spr->setPosition(sf::Vector2f(x, y));
            rectangle.setPosition(sf::Vector2f(x, y));
        }
        else
        {
            spr->setPosition(sf::Vector2f(0, 0));
        }

        // Draw background
        sf::RenderStates states;
        states.transform = sf::Transform::Identity;
        states.blendMode = sf::BlendAlpha;
        states.texture = &ResourceManagerOld::GetRenderTexture("tex_used_tiles")->getTexture();
        texViewport->draw(map_bg_vao_, states);
        texViewport->draw(map_pg_vao_, states);
        texViewport->draw(map_fg_vao_, states);

        texViewport->draw(*spr);
        //texViewport->draw(&grid_[0], grid_.size(), sf::Lines);

        texMinimap->draw(map_bg_vao_, states);
        texMinimap->draw(map_pg_vao_, states);
        texMinimap->draw(map_fg_vao_, states);
        //texMinimap->draw(*spr);

        texViewport->display();
    }
    */


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Render to our imgui framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, ResourceManagerOld::GetFramebuffer("imguiScene").GetID());
    glViewport(0, 0, width_, height_); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    //glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    //glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

    //ResourceManagerOld::GetShader("scene").Use();
    resources_->GetShader("scene")->Use();
    glBindVertexArray(vao_);
    glBindTexture(GL_TEXTURE_2D, ResourceManagerOld::GetFramebuffer("scene").GetTextureID());
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLvoid Scene::PlaceTile()
{
    if (!map_is_null_ && mouse_inside_level_)
    {
        if (active_layer_ != layer_t::FORE)
        {
            /* std::string key = ResourceManagerOld::getNameHash(active_tilemap_name_, active_sprite_name_); */
            /* std::stringstream msg;
            msg << "key: " << active_sprite_name_;
            MessageManager::AddMessage(msg, message_t::INFO); */

            e_level_layers_["Tiles"]->AddSprite(current_tile_id_, active_sprite_name_.c_str(), e_sprites_["brush"]->GetTextureID());

            //TilemapManager::GetTilemap("Testing")->AddTile(active_sprite_name_.c_str(), e_sprites_["brush"]->GetTexture()->ID);
        }
    }
}

GLvoid Scene::RemoveTile()
{
    if (!map_is_null_ && mouse_inside_level_)
    {
        if (active_layer_ != layer_t::FORE)
        {
            /* std::string key = ResourceManagerOld::getNameHash(active_tilemap_name_, active_sprite_name_); */
            /* std::stringstream msg;
            msg << "key: " << active_sprite_name_;
            MessageManager::AddMessage(msg, message_t::INFO); */
            std::string currentActiveSprite = active_sprite_name_;
            std::string keyEmptyHash = ResourceManagerOld::GetNameHash("Tiles", "r0c0");
            e_sprites_.find("brush")->second->AssignTextureID(ResourceManagerOld::GetTexture(keyEmptyHash.c_str()).ID);
            active_sprite_name_ = keyEmptyHash.c_str();
            e_level_layers_["Tiles"]->AddSprite(current_tile_id_, active_sprite_name_.c_str(), e_sprites_["brush"]->GetTextureID());
            e_sprites_.find("brush")->second->AssignTextureID(ResourceManagerOld::GetTexture(currentActiveSprite.c_str()).ID);
            active_sprite_name_ = currentActiveSprite.c_str();
            //TilemapManager::GetTilemap("Testing")->AddTile(active_sprite_name_.c_str(), e_sprites_["brush"]->GetTexture()->ID);
        }
    }
}

// PRIVATE
/*
GLvoid Scene::generateGrid()
{
    grid_.clear();

    glm::vec2 sprSize = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteSize();
    glm::vec2 sprScale = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteScale();

    // Draw a grid
    {
        sf::Color gridColor = {255, 255, 255, 50};
        // Push grid rows to vertex vector
        GLuint numRows = map_height_;
        for (size_t row = 0; row <= numRows; row++)
        {
            GLuint rowPixel = row * (sprSize.y * sprScale.y);
            grid_.push_back(sf::Vertex(sf::Vector2f(0, rowPixel), gridColor));
            grid_.push_back(sf::Vertex(sf::Vector2f(map_width_ * (sprSize.y * sprScale.y), rowPixel), gridColor));
        }
        // Push grid columns to vertex vector
        GLuint numCols = map_width_;
        for (size_t col = 0; col <= numCols; col++)
        {
            GLuint colPixel = col * (sprSize.x * sprScale.x);
            grid_.push_back(sf::Vertex(sf::Vector2f(colPixel, 0), gridColor));
            grid_.push_back(sf::Vertex(sf::Vector2f(colPixel, map_height_ * (sprSize.x * sprScale.x)), gridColor));
        }
    }
}
*/
