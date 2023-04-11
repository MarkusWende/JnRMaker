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
 * @file solid_sprite.h
 * @brief This file contains the solid sprite class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#ifndef SOLID_SPRITE_H
#define SOLID_SPRITE_H

#include "entity_solid.h"
#include "primitive_quad.h"

 /**
  * @brief The Primitive Cube class represents a cube.
  * @sa HPlane, Line, Prism, Sphere, VPlane and Primitive
  */
class Sprite : public Solid
{
public:
    Sprite(std::string meshName = "Sprite", GLboolean smooth = false, GLuint width = 16, GLuint height = 16);		/**< Constructor. Smooth shading by default. */
    ~Sprite() {}														    /**< Destructor. */

    GLvoid Draw(glm::mat4 projection, glm::mat4 view);

    GLvoid AssignTextureID(GLuint texID) { texture_id_ = texID; };

    GLuint GetTextureID() { return texture_id_; };

private:
    using Solid::DrawWireframe;						/**< Dont use this function. Therefor its private. */

    GLuint height_;
    GLuint width_;

    std::unique_ptr<Quad>	quad_;
    GLuint            texture_id_;

	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version)
	{

	}

};

CEREAL_CLASS_VERSION(Sprite, 1)

#endif