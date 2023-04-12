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
* @file scene.h
* @brief This file contains the coordinate system solid class, which is used to create and draw coordinate systems.
*
* @author Markus Wende
* https://github.com/MarkusWende
*/


#ifndef SOLID_CS_H
#define SOLID_CS_H

#include "entity_solid.h"
#include "primitive_line.h"

  /**
   * @brief Todo:
   */
class CoordinateSystem : public Solid
{
public:
    CoordinateSystem(glm::vec3 offset = glm::vec3(0));				                                /**< Todo: */
	~CoordinateSystem() { };														/**< Destructor. */

	/**
	 * @brief Todo:
	 * @return Void.
	 */
	GLvoid Draw(glm::mat4 projection, glm::mat4 view);

	GLvoid SetResolution(glm::vec2 res)				{ resolution_ = res; };

private:
	using Solid::DrawWireframe;						/**< Dont use this function. Therefor its private. */

	std::map<std::string, std::unique_ptr<Line>>	axis_;
	glm::vec2										resolution_;
	//std::map<std::string, glm::vec4>				colors_;

};

#endif	/* SOLID_CS_H */
