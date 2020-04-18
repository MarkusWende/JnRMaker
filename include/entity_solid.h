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

#include "project_manager.h"

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
class Solid
{
public:
    Solid()
    {
        Solid(0, "NULL", layer_t::FORE);
    };

    Solid(GLuint id, std::string name, layer_t layer)                            //!< constructor
    {
        layer_ = layer;
        name_ = name;
        id_ = id;
    };

    ~Solid() { };                                               //!< destructor

    GLuint	GetId() { return id_; };

private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& ar, std::uint32_t const version)
    {
        //ar(cereal::base_class<SceneEntity>(this));
        //ar(CEREAL_NVP(layer_));
        ar(CEREAL_NVP(name_));
        ar(CEREAL_NVP(id_));
    }

    layer_t                 layer_;                               /**< foreground layer. */
    std::string             name_;
    GLuint                  id_;
};
CEREAL_CLASS_VERSION(Solid, 1);

#endif
