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
 * @file primitive.h
 * @author Kirsch Audio GmbH
 * @brief This file contains the Primitive class, which is used as a parent class to create and draw simple 3d objects.
 */


#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "entity_solid.h"

#include <glm/gtx/intersect.hpp>

 /**
  * @brief The Primitive class is a parent class to create and draw simple 3d objects.
  * @sa Cube, HPlane, Line, Prism, Sphere and VPlane
  */
class Primitive : public Solid
{
public:
	Primitive(std::string name) : Solid(0,  name, layer_t::FORE) {};										/**< Constructor. */
	//~Primitive() { };													/**< Destructor. */

	/**
	 * @brief Draw the vertex array object. This method is virtual.
	 * @return Void.
	 */
	//void Draw();

	/**
	 * @brief Draw the vertex array object in wireframe mode. This method is virtual.
	 * @param shader The shader, which should be used.
	 * @param color A glm::vec4 color (RGBA).
	 * @return Void.
	 */
	//void DrawLines(Shader shader, glm::vec4 color);

	/**
	 * @brief Draw the vertex array object with solid faces and a wireframe. This method is virtual.
	 * @param shader The shader, which should be used.
	 * @param color A glm::vec4 color (RGBA).
	 * @return Void.
	 */
	//void DrawWithLines(Shader shader, glm::vec4 color);

	/**
	 * @brief Calculate if there is an intersection with this object. This is done by using glm::intersectRayTriangle. This method is virtual.
	 * @param orig The origin of the ray.
	 * @param dir The direction of the ray.
	 * @return Bool Return true if an intersection with the object exists.
	 */
	//bool Intersection(glm::vec3 orig, glm::vec3 dir);

	//unsigned int VAO;												/**< The vertex array object id. */

protected:
	/**
	 * @brief Generate the vertex array object.
	 * @return Void.
	 */
	//void setupVAO();

	/**
	 * @brief Generate the vertex array object, when vertices are changed.
	 * @return Void.
	 */
	//void updateVAO();

	/**
	 * @brief Clear the vertices, indices, faces and edges vectors.
	 * @return Void.
	 */
	//void clearArrays();

	/**
	 * @brief Add a vertex to the vertex vector.
	 * @return Void.
	 */
	//void addVertex(Vertex v);

	/**
	 * @brief Add a vertex to the vertex vector.
	 * @return Void.
	 */
	//void addEdge(Edge e);

	/**
	 * @brief Add a vertex to the vertex vector.
	 * @return Void.
	 */
	//void addFace(Face f);

	/**
	 * @brief Add three vertex indices to the index vector. Used for triangles
	 * @param i1 Vertex id of the first vertex.
	 * @param i2 Vertex id of the second vertex.
	 * @param i3 Vertex id of the third vertex.
	 * @return Void.
	 */
	//void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);

	/**
	 * @brief Add two vertex indices to the index vector. Used for vectors (lines).
	 * @param i1 Vertex id of the first vertex.
	 * @param i2 Vertex id of the second vertex.
	 * @return Void.
	 */
	//void addIndices(unsigned int i1, unsigned int i2);

	/**
	 * @brief Calculate the face normal, which is defined by three vertices.
	 * @param v1 First vertex.
	 * @param v2 Second vertex.
	 * @param v3 Third vertex.
	 * @return glm::vec3 A 3d vector [x,y,z].
	 */
	//glm::vec3 computeFaceNormal(Vertex v1, Vertex v2, Vertex v3);

	//bool						smooth;								/**< Stores the object shading configuration. True => smooth shading, false => flat shading. */
	//unsigned int				vbo;								/**< Vertex buffer object. */
	//unsigned int				ebo;								/**< Element buffer object. */
	//std::vector<Vertex>			vertices;							/**< Vector of the object vertices. */
	//std::vector<unsigned int>	indices;							/**< Vector of the object indices. */
	//std::vector<Edge>			edges;								/**< Vector of the object edges. */
	//std::vector<Face>			faces;								/**< Vector of the object faces. */
	
};

#endif
