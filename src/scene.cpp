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


#include "../include/scene.h"

Scene::Scene(GLuint width, GLuint height)
{
  width_ = width;
  height_ = height;

  add_sprite_flag_ = false;
  mouse_over_scene_ = false;
  map_is_null_ = true;
  map_height_ = 0;
  map_width_ = 0;

  // Load tilemap
  active_tilemap_name_ = "";
  active_sprite_name_ = "";
  active_layer_ = layer_t::BACK;

  ResourceManager::CreateRenderTexture(width_, height_, "viewport");
  ResourceManager::CreateRenderTexture(width_, height_, "minimap");

  e_cameras_.insert(std::make_pair("Editor", new Camera(width_, height_)));

  // Create framebuffers
  //ResourceManager::CreateRenderTexture(width_, height_, "viewport");
  //generateGrid();
}

Scene::~Scene() { }

GLvoid Scene::CreateMap(GLuint width, GLuint height, sf::Vector2u spriteSize, sf::Vector2f spriteScale)
{
    map_is_null_ = false;
    map_width_ = width;
    map_height_ = height;
    generateGrid();
    map_pixel_width_ = map_width_ * spriteSize.x * spriteScale.x;
    map_pixel_height_ = map_height_ * spriteSize.y * spriteScale.y;

    ResourceManager::CreateRenderTexture(32, 32, "tex_used_tiles");
    ResourceManager::ResizeRenderTexture(map_pixel_width_, map_pixel_height_, "minimap");

    //e_cameras_["Editor"]->Size(width_, height_);
    //e_cameras_["Editor"]->Center(width_ / 2, height_ / 2);

    map_bg_ = std::vector<std::vector<GLuint>>(height, std::vector<GLuint>(width, 0));
    map_pg_ = std::vector<std::vector<GLuint>>(height, std::vector<GLuint>(width, 0));
    map_fg_ = std::vector<std::vector<GLuint>>(height, std::vector<GLuint>(width, 0));

    map_bg_vao_.clear();
    map_bg_vao_.setPrimitiveType(sf::Quads);
    map_bg_vao_.resize(map_width_ * map_height_* 4);

    map_pg_vao_.clear();
    map_pg_vao_.setPrimitiveType(sf::Quads);
    map_pg_vao_.resize(map_width_ * map_height_ * 4);

    map_fg_vao_.clear();
    map_fg_vao_.setPrimitiveType(sf::Quads);
    map_fg_vao_.resize(map_width_ * map_height_ * 4);
}

GLvoid Scene::Update(GLuint width, GLuint height)
{
    width_ = width;
    height_ = height-24;

    ResourceManager::ResizeRenderTexture(width_, height_, "viewport");
    //e_cameras_["Editor"]->Center(width_ / 2, height_ / 2);
    e_cameras_["Editor"]->Resize(width_, height_);
}

GLvoid Scene::Render(sf::Vector2i posMouse)
{
    if (!map_is_null_)
    {
        sf::RenderTexture* texViewport = ResourceManager::GetRenderTexture("viewport");
        sf::RenderTexture* texMinimap = ResourceManager::GetRenderTexture("minimap");
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
            sf::Vector2f worldPos = texViewport->mapPixelToCoords({ posMouse.x, posMouse.y - 48 });
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
                    sf::RenderTexture* renderTex = ResourceManager::GetRenderTexture("tex_used_tiles");
                    sf::Texture tex = renderTex->getTexture();
                    sf::Sprite sprTexOld;
                    sprTexOld.setOrigin(0, 32);
                    sprTexOld.setScale(1.0f, -1.0f);
                    sprTexOld.setTexture(tex);
                    ResourceManager::ResizeRenderTexture((idSolid) * (sprSize.x * sprScale.x), (sprSize.y * sprScale.y), "tex_used_tiles");
                    sf::RenderTexture* newRenderTex = ResourceManager::GetRenderTexture("tex_used_tiles");
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
        states.texture = &ResourceManager::GetRenderTexture("tex_used_tiles")->getTexture();
        texViewport->draw(map_bg_vao_, states);
        texViewport->draw(map_pg_vao_, states);
        texViewport->draw(map_fg_vao_, states);

        texViewport->draw(*spr);
        texViewport->draw(&grid_[0], grid_.size(), sf::Lines);

        texMinimap->draw(map_bg_vao_, states);
        texMinimap->draw(map_pg_vao_, states);
        texMinimap->draw(map_fg_vao_, states);
        //texMinimap->draw(*spr);

        texViewport->display();
    }
}

GLvoid Scene::updateLayerVAO(layer_t layer)
{
    glm::vec2 sprSize = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteSize();
    glm::vec2 sprScale = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteScale();

    for (size_t row = 0; row < map_height_; row++)
    {
        for (size_t col = 0; col < map_width_; col++)
        {
            GLuint y = row * (sprSize.y * sprScale.y);
            GLuint x = col * (sprSize.x * sprScale.x);
            
            sf::Vertex* quad;
            GLuint id = 0;
            if (layer == layer_t::FORE)
            {
                id = map_fg_.at(row).at(col);
                quad = &map_fg_vao_[(col + row * map_width_) * 4];
            }
            else if (layer == layer_t::PLAYER)
            {
                id = map_pg_.at(row).at(col);
                quad = &map_pg_vao_[(col + row * map_width_) * 4];
            }
            else
            {
                id = map_bg_.at(row).at(col);
                quad = &map_bg_vao_[(col + row * map_width_) * 4];
            }

            sf::RenderTexture* renderTex = ResourceManager::GetRenderTexture("tex_used_tiles");
            GLuint tu = (id - 1) % (GLuint)(renderTex->getSize().x / (sprSize.x * sprScale.x));
            GLuint tv = (id - 1) / (renderTex->getSize().x / (sprSize.x * sprScale.x));
            
            if (id != 0)
            {
                quad[0].position = sf::Vector2f(x, y);
                quad[1].position = sf::Vector2f(x + (sprSize.x * sprScale.x), y);
                quad[2].position = sf::Vector2f(x + (sprSize.x * sprScale.x), y + (sprSize.y * sprScale.y));
                quad[3].position = sf::Vector2f(x, y + (sprSize.y * sprScale.y));

                quad[3].texCoords = sf::Vector2f(tu * (sprSize.x * sprScale.x), tv * (sprSize.y * sprScale.y));
                quad[2].texCoords = sf::Vector2f((tu + 1) * (sprSize.x * sprScale.x), tv * (sprSize.y * sprScale.y));
                quad[1].texCoords = sf::Vector2f((tu + 1) * (sprSize.x * sprScale.x), (tv + 1) * (sprSize.y * sprScale.y));
                quad[0].texCoords = sf::Vector2f(tu * (sprSize.x * sprScale.x), (tv + 1) * (sprSize.y * sprScale.y));
            }
        }
    }
}

// PRIVATE
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

std::string Scene::getNameHash(std::string tilesetName, std::string tileName)
{
    std::stringstream hash;
    std::hash<std::string> hash_fn;
    size_t str_hash = hash_fn(tilesetName);
    hash << str_hash << ":" << tileName;
    return hash.str();
}
