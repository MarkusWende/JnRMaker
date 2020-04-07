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
 * @file entity_solid.h
 * @brief This file contains the solid entity class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#ifndef SOLID_H
#define SOLID_H

#include <SFML/Graphics/Sprite.hpp>

#include "scene_entity.h"

 /**
  * @brief Enumeration for layer types.
  */
enum class layer_t {
    FORE,                            /**< Background layer. */
    PLAYER,                          /**< player ground layer. */
    BACK,                            /**< foreground layer. */
};

/**
 * @brief Solid class represents a solid object. Solids are visible object in the rendered scene.
 */
class Solid : public SceneEntity
{
public:
    Solid()
    {
        id_ = "NULL";
        spr_ = sf::Sprite();
        layer_ = layer_t::FORE;
    };

    Solid(std::string id, sf::Sprite spr, layer_t layer)                            //!< constructor
    {
        layer_ = layer;
        id_ = id;
        spr_ = spr;
    };

    ~Solid() { };                                               //!< destructor

    sf::Sprite* GetSprite() { return &spr_; };

private:
    layer_t                 layer_;                               /**< foreground layer. */
    sf::Sprite              spr_;
    std::string             id_;



};
#endif
