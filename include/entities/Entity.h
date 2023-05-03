/****************************************************************************
 * Copyright (C) 2019 Kirsch-Audio GmbH <info@kirsch-audio.com>				*
 * All rights reserved.														*
 *																			*
 * This file is part of SoundIMP											*
 *																			*
 *	SoundIMP is a software for creating sound immission predictions.		*
 *																			*
 *	Unauthorized copying of this file, via any medium is strictly			*
 *	prohibited																*
 *																			*
 *	Proprietary and confidential.											*
 ****************************************************************************/

 /**
  * @file Entity.h
  * @author Kirsch Audio GmbH
  * @brief 
  */


#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#ifdef __EMSCRIPTEN__
#define GLM_FORCE_PURE
#else

#endif

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/intersect.hpp>

#include "ResourceManager.h"
#include "MessageManager.h"
#include "ProjectManager.h"

enum class EntityType {
	LIGHT_ENTITY,
	SOLID_ENTITY,
	CAMERA_ENTITY,
	DEFAULT_ENTITY
};

/**
 * @brief Defines several possible options for entity movements. Used as abstraction to stay away from window-system specific input methods.
 */
enum class EntityMovementDirection {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

/**
* @brief 
*/
class SceneEntity
{
public:
																		/**< Constructor. */
	explicit SceneEntity() : entity_type_(EntityType::DEFAULT_ENTITY),
		center_(glm::vec3(0.0f, 0.0f, 0.0f))
	{ };													
	
	~SceneEntity() { };									/**< Destructor. */

	GLvoid Move(glm::vec3 pos) { center_ = pos; };

	glm::vec3	GetCenter() { return center_; }													/**< @brief Get the center of the solid. @return glm::vec3 The center as a 3d vector. */

protected:
	EntityType	entity_type_;
	//glm::vec3	position;						/**< Entity position in a cartesian coordinate system. */
	glm::vec3   center_;							/**< @brief The center of the solid. */
	
};

#endif
