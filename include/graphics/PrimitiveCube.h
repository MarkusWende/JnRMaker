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
 * @file PrimitiveCube.h
 * @author Kirsch Audio GmbH
 * @brief This file contains the Primitive Cube class, which is used to create and draw cubes.
 */


#ifndef PRIMITIVE_CUBE_H
#define PRIMITIVE_CUBE_H

#include "Primitive.h"
#include "ResourceManagerOld.h"

/**
 * @brief The Primitive Cube class represents a cube.
 * @sa HPlane, Line, Prism, Sphere, VPlane and Primitive
 */
class Cube : public Primitive
{
public:
	Cube(std::shared_ptr<ILogger> logger, std::shared_ptr<Resources> resources, std::string meshName = "Cube", GLboolean smooth=false);		/**< Constructor. Smooth shading by default. */
    //~Cube() {}														/**< Destructor. */

	/**
	 * @brief Set the position of one of the four vertical cube edges by id.
	 * @param pos New position of the edge. [x,y] -> [x,z] y of the new position is mapped to the z coordinate, because y is the vertical axis.
	 * @param id Id of the edge e.g. {0,1,2,3}.
	 * @return Void.
	 */
	GLvoid SetEdge(glm::vec2 pos, GLuint id);

	/**
	 * @brief Set the position of the top and bottom face of the cube, relative to the coordinate system.
	 * @param topHeight New top y-Value of the cube, relative to the coordinate system.
	 * @param bottomHeight New bottom y-Value of the cube, relative to the coordinate system.
	 * @return Void.
	 */
	GLvoid SetHeight(GLfloat topHeight, GLfloat bottomHeight);

private:
	/**
	 * @brief Build the cube with smooth shading.
	 * @return Void.
	 */
	GLvoid buildVerticesSmooth();

	/**
	 * @brief Build the cube with flat shading.
	 * @return Void.
	 */
	GLvoid buildVerticesFlat();
	
};

#endif
