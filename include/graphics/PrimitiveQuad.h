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


#ifndef PRIMITIVE_QUAD_H
#define PRIMITIVE_QUAD_H

#include "Primitive.h"
#include "ResourceManagerOld.h"

/**
 * @brief The Primitive Quad class represents a quad.
 * @sa Cube, Line, Prism, Sphere, VPlane and Primitive
 */
class Quad : public Primitive
{
public:
	Quad(std::shared_ptr<ILogger> logger, std::shared_ptr<Resources> resources, std::string meshName = "Quad");		/**< Constructor. Smooth shading by default. */
    ~Quad() {}														/**< Destructor. */

private:
	/**
	 * @brief Build the cube with smooth shading.
	 * @return Void.
	 */
	GLvoid build();
	
};

#endif	/* PRIMITIVE_QUAD_H */
