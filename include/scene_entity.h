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
 * @file scene_entity.h
 * @brief This file contains the base class implementation of a scene entity.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include <SFML/System/Vector3.hpp>
#include <SFML/OpenGL.hpp>


/**
 * @brief Scene entity class represents a general entity in the scene. E.g. entities are camera objects, solid object or light object.
 */
class SceneEntity
{
public:
  SceneEntity() : position(sf::Vector3<GLfloat>(0.0f, 0.0f, 0.0f)) { };     //!< constructor
  ~SceneEntity() { };                                                       //!< destructor

protected:
  sf::Vector3<GLfloat> position;                                            /**< Position of the entity in a 3D coordinate system. */

private:

};

#endif
