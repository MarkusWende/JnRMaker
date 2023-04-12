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
 * @file primitive_line.h
 * @author Kirsch Audio GmbH
 * @brief This file contains the Primitive Line class, which is used to create vectors (lines) in a 3d coordinate system.
 */


#ifndef PRIMITIVE_LINE_H
#define PRIMITIVE_LINE_H

#include "primitive.h"

/**
 * @brief The Primitive Line class represents a vector (line) in 3d space.
 * @sa Cube, HPlane, Prism, Sphere, VPlane and Primitive
 */
class Line : public Primitive
{
public:
	Line(std::string meshName = "line", glm::vec3 begin = { 0.0f, 0.0f, 0.0f }, glm::vec3 end = { 0.0f, 0.0f, 0.0f });	/**< Constructor. A vector with start and end coordinate {0,0,0} by default. */
	~Line() { };															/**< Destructor. */

	/**
	 * @brief Draw the vertex array object of the vector (line).
	 * @sa Primitive::Draw()
	 * @return Void.
	 */
	GLvoid Draw();

	/**
	 * @brief Calculate if there is an intersection with the line. This is done by using glm::intersectRayTriangle and two vertices of the triangle are the same.
	 * @sa Primitive::Intersection()
	 * @param orig The origin of the ray.
	 * @param dir The direction of the ray.
	 * @return Bool Return true if an intersection with the line exists.
	 */
	//bool Intersection(glm::vec3 orig, glm::vec3 dir) override;

	/**
	 * @brief Set the start position of the vector.
	 * @param p Position of the start point {x,y,z}.
	 * @return Void.
	 */
	GLvoid SetStartPosition(glm::vec3 p);

	/**
	 * @brief Set the end position of the vector.
	 * @param p Position of the end point {x,y,z}.
	 * @return Void.
	 */
	GLvoid SetEndPosition(glm::vec3 p);

	/**
	 * @brief Get the end position of the vector.
	 * @return glm::vex3 The end point of the vector {x,y,z}.
	 */
	glm::vec3 GetLineEndPosition();

	/**
	 * @brief Get the start position of the vector.
	 * @return glm::vex3 The start point of the vector {x,y,z}.
	 */
	glm::vec3 GetLineStartPosition();

private:
	using Solid::DrawWireframe;						/**< Dont use this function. Therefor its private. */
	
};

#endif	/* PRIMITIVE_LINE_H */
