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
 * @brief This file contains the scene class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */

#ifndef SCENE_H
#define SCENE_H

#include <sstream>
#include <map>
#include <memory>

#include <SFML/OpenGL.hpp>
#include <SFML/System/Vector2.hpp>

#include "entity_camera.h"
#include "entity_solid.h"
#include "resource_manager.h"
#include "message_manager.h"
#include "tilemap_manager.h"
#include "scene_entity.h"


/**
 * @brief Scene class represents the rendered the, which is stored in a texture and displayed in ImGui.
 */
class Scene
{
public:
	Scene(GLuint width, GLuint height);														//!< constructor
	~Scene();																											//!< destructor

	/**
	 * @brief Update the scene, when the window size changes.
	 * @return GLvoid.
	 */
	GLvoid Update();

	/**
	 * @brief Render all entities that are displayed to the scene texture.
	 * @return GLvoid.
	 */
	GLvoid Render();

	// Setters
	/**
	 * @brief Set the width of the scene.
	 * @param width New width of the scene.
	 * @return GLvoid.
	 */
	GLvoid SetSizeWidth(GLuint width) { width_ = width; };

	/**
	 * @brief Set the height of the scene.
	 * @param height New height of the scene.
	 * @return GLvoid.
	 */
	GLvoid SetSizeHeight(GLuint height) { height_ = height; };

	//std::map<std::string, std::unique_ptr<sf::Sprite>> 	tiles_;	/**< All solid entities are stored in this map. */

private:
	std::map<std::string, std::unique_ptr<Camera>> 			e_cameras_;	/**< All camera entities are stored in this map. */
	std::map<std::string, std::unique_ptr<Solid>> 			e_solids_;	/**< All solid entities are stored in this map. */

	GLuint																					width_;			/**< Width of the rendered scene. */
	GLuint																					height_;		/**< Height of the rendered scene. */

};

#endif
