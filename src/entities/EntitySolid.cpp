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
  * @file entity_solid.cpp
  * @author Kirsch Audio GmbH
  * @brief This file contains the Primitive class, which is used as a parent class to create and draw simple 3d objects.
  */

#include "EntitySolid.h"


  // PUBLIC:
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

Solid::Solid()
{
	layer_ = layer_t::FORE;
	name_ = "Solid";
	id_ = 0;
	entity_type_ = EntityType::SOLID_ENTITY;
	scale_ = glm::vec3(1.0f);
	show_ = true;
	color_ = { 1.0f, 1.0f, 1.0f };
	color_wireframe_ = { 1.0f, 1.0f, 1.0f };
	alpha_ = 1.0f;
	alpha_wireframe_ = 1.0f;
	smooth_ = true;
	center_ = { 0.0f, 0.0f, 0.0f };
	yaw_ = 0.0f;
	pitch_ = 0.0f;
	roll_ = 0.0f;
}

Solid::Solid(GLuint id, std::string name, layer_t layer)                            //!< constructor
{
	layer_ = layer;
	name_ = name;
	id_ = id;
	entity_type_ = EntityType::SOLID_ENTITY;
	scale_ = glm::vec3(1.0f);
	show_ = true;
	color_ = { 1.0f, 1.0f, 1.0f };
	color_wireframe_ = { 1.0f, 1.0f, 1.0f };
	alpha_ = 1.0f;
	alpha_wireframe_ = 1.0f;
	smooth_ = true;
	name_ = name;
	center_ = { 0.0f, 0.0f, 0.0f };
	yaw_ = 0.0f;
	pitch_ = 0.0f;
	roll_ = 0.0f;
}

GLvoid Solid::Draw()
{
	model_.Draw();
}

GLvoid Solid::DrawWireframe()
{
	model_.DrawWireframe();
}

GLboolean Solid::Intersection(glm::vec3 orig, glm::vec3 dir, glm::vec3& pos)
{
	return model_.Intersection(orig, dir, pos);
}

GLboolean Solid::IntersectionCurve(glm::vec3 start, glm::vec3 end, std::vector<glm::vec3>& intersectCurve)
{
	if (model_.IntersectionCurve(start, end, intersectCurve))
		return true;

	return false;
}

// PROTECTED:
/////////////////////////////////////////////////////////////////////////////////////////////////////////
GLvoid Solid::addMesh(std::string meshName)
{
	model_.AddMesh(meshName);
}

GLvoid Solid::addVertex(std::string meshName, VertexNew v)
{
	model_.AddVertex(meshName, v);
}

GLvoid Solid::addEdge(std::string meshName, EdgeNew e)
{
	model_.AddEdge(meshName, e);
}

GLvoid Solid::addFace(std::string meshName, FaceNew f)
{
	model_.AddFace(meshName, f);
}

GLvoid Solid::addIndices(std::string meshName, GLuint i1, GLuint i2, GLuint i3)
{
	model_.AddIndices(meshName, i1, i2, i3);
}

GLvoid Solid::addIndices(std::string meshName, GLuint i1, GLuint i2)
{
	model_.AddIndices(meshName, i1, i2);
}

GLvoid Solid::addIndex(std::string meshName, GLuint i1)
{
	model_.AddIndex(meshName, i1);
}

GLvoid Solid::clearMesh(std::string meshName)
{
	model_.ClearMesh(meshName);
}

GLvoid Solid::triangulateMesh(std::string meshName)
{
	model_.TriangulateMesh(meshName);
}

// GLvoid Solid::reduceMesh(std::string meshName)
// {
// //	model_.ReduceMesh(meshName);
// }

// GLvoid Solid::copyReducedMesh(std::string meshName)
// {
// //	model_.CopyReducedMesh(meshName);
// }

GLvoid Solid::updateVAO(std::string meshName)
{
	model_.UpdateVAO(meshName);
}

glm::vec3 Solid::computeFaceNormal(VertexNew v1, VertexNew v2, VertexNew v3)
{
	return computeFaceNormal(v1.Position, v2.Position, v3.Position);
}

glm::vec3 Solid::computeFaceNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	const float EPSILON = 0.000001f;

	glm::vec3 normal(0.0f);     // default return value (0,0,0)
	float nx, ny, nz;

	// find 2 edge vectors: v1-v2, v1-v3
	float ex1 = v2.x - v1.x;
	float ey1 = v2.y - v1.y;
	float ez1 = v2.z - v1.z;
	float ex2 = v3.x - v1.x;
	float ey2 = v3.y - v1.y;
	float ez2 = v3.z - v1.z;

	// cross product: e1 x e2
	nx = ey1 * ez2 - ez1 * ey2;
	ny = ez1 * ex2 - ex1 * ez2;
	nz = ex1 * ey2 - ey1 * ex2;

	// normalize only if the length is > 0
	float length = sqrtf(nx * nx + ny * ny + nz * nz);
	if (length > EPSILON)
	{
		// normalize
		float lengthInv = -1.0f / length;
		normal.x = nx * lengthInv;
		normal.y = ny * lengthInv;
		normal.z = nz * lengthInv;
	}

	return normal;
}

glm::vec3 Solid::computeTriangleCenter(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	glm::vec3 c = glm::vec3(0.0f);

	c.x = (v1.x + v2.x + v3.x) / 3.0f;
	c.y = (v1.y + v2.y + v3.y) / 3.0f;
	c.z = (v1.z + v2.z + v3.z) / 3.0f;

	return c;
}

GLboolean Solid::vecVecIntersection(glm::vec3 vP0, glm::vec3 vP1, glm::vec3 uP0, glm::vec3 uP1)
{
	GLfloat x1, x2, x3, x4, y1, y2, y3, y4;
	x1 = vP0.x;
	x2 = vP1.x;
	x3 = uP0.x;
	x4 = uP1.x;
	y1 = vP0.z;
	y2 = vP1.z;
	y3 = uP0.z;
	y4 = uP1.z;

	if (((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)) == 0.0f)
	{
		return true;
	}

	GLfloat Px = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
	GLfloat Py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

	GLfloat lengthV0 = glm::length(glm::vec2(x1, y1) - glm::vec2(x2, y2));
	GLfloat lengthV1 = glm::length(glm::vec2(x3, y3) - glm::vec2(x4, y4));
	GLfloat lengthV0P0AndP = glm::distance(glm::vec2(x1, y1), glm::vec2(Px, Py));
	GLfloat lengthV0P1AndP = glm::distance(glm::vec2(x2, y2), glm::vec2(Px, Py));
	GLfloat lengthV1P0AndP = glm::distance(glm::vec2(x3, y3), glm::vec2(Px, Py));
	GLfloat lengthV1P1AndP = glm::distance(glm::vec2(x4, y4), glm::vec2(Px, Py));

	if (lengthV0P0AndP < 0.0001f)
	{
		return false;
	}
	else if (lengthV0P1AndP < 0.0001f)
	{
		return false;
	}
	else if (lengthV1P0AndP < 0.0001f)
	{
		return false;
	}
	else if (lengthV1P1AndP < 0.0001f)
	{
		return false;
	}

	if (((lengthV0P0AndP + lengthV0P1AndP) <= lengthV0) &&
		((lengthV1P0AndP + lengthV1P1AndP) <= lengthV1))
	{
		return true;
	}

	return false;
}

// GLvoid Solid::updateNormals(std::string meshName)
// {
// //	model_.UpdateNormals(meshName);
// }
