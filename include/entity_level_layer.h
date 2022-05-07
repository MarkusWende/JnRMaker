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
 * @file entity_level_layer.h
 * @brief This file contains the solid entity class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#ifndef LEVEL_LAYER_H
#define LEVEL_LAYER_H

#include "scene_entity.h"
#include "tilemap_manager.h"

/**
 * @brief Solid class represents a solid object. Solids are visible object in the rendered scene.
 */
class LevelLayer : public SceneEntity
{
public:
	LevelLayer();
	LevelLayer(std::string name, GLuint width, GLuint height, glm::vec2 spriteSize);                            //!< constructor

    GLuint GetHeight() {return height_;};
    GLuint GetWidth() {return width_;};
	GLvoid AddSprite(GLuint mapID, const std::string key, GLuint texID);

    GLvoid Draw(glm::mat4 projection, glm::mat4 view);

    std::string GetHash(GLuint posX, GLuint posY) { return hash_map_.at(posY).at(posX); };

    ~LevelLayer();                                               //!< destructor

private:
    GLvoid init();
    GLvoid draw_border();

    std::string                             name_;
    std::vector<std::vector<std::string>>   hash_map_;
    GLuint                                  width_pixels_;
    GLuint                                  width_;
    GLuint                                  height_;
    GLuint                                  height_pixels_;
    glm::vec2                               tile_size_;
    glm::vec2                               tile_scale_;

    std::vector<glm::vec2>                  translations_;
    std::vector<GLfloat>                    tile_id_;
    GLuint                                  quad_vao_;
    GLuint                                  quad_vbo_;
    GLuint                                  tile_trans_vbo_;
    GLuint                                  tile_id_vbo_;
    //std::map<std::string, GLfloat> 			tile_hash_id_map_;
    //GLfloat                                 tile_id_max_;
    std::string                             hash_map_border_key_;
};

#ifdef __EMSCRIPTEN__

#else
CEREAL_CLASS_VERSION(LevelLayer, 1);
#endif

#endif
