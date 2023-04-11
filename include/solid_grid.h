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
 * @file primitive_cube.h
 * @author Kirsch Audio GmbH
 * @brief This file contains the Primitive Cube class, which is used to create and draw cubes.
 */


#ifndef SOLID_GRID_H
#define SOLID_GRID_H

#include "entity_solid.h"
#include "primitive_line.h"

/**
 * @brief The Primitive Cube class represents a cube.
 * @sa HPlane, Line, Prism, Sphere, VPlane and Primitive
 */
class Grid : public Solid
{
public:
    Grid(std::string meshName = "Grid", GLboolean smooth = false, GLuint width = 1, GLuint height = 1);		/**< Constructor. Smooth shading by default. */
    ~Grid() {}														    /**< Destructor. */

    GLvoid Draw(glm::mat4 projection, glm::mat4 view);

private:
    using Solid::DrawWireframe;						/**< Dont use this function. Therefor its private. */

	/**
	 * @brief Build the grid.
	 * @return Void.
	 */
	GLvoid build();

    std::map<std::string, std::unique_ptr<Line>>	axis_h_;
    std::map<std::string, std::unique_ptr<Line>>	axis_v_;

    GLfloat line_width_;
    GLuint height_;
    GLuint width_;

	friend class cereal::access;
	template <class Archive>
	void save(Archive& ar, std::uint32_t const version)
	{

	}

	template <class Archive>
	void load(Archive& ar, std::uint32_t const version)
	{

	}

};

CEREAL_CLASS_VERSION(Grid, 1)

#endif
