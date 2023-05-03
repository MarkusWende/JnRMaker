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
 * @file primitive.cpp
 * @brief
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */

#include "Primitive.h"


// PUBLIC:
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Primitive::Primitive()
{

}

Primitive::~Primitive()
{

}
*/
/*
void Primitive::Draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//Solid::Draw();
}

void Primitive::DrawLines(Shader shader, glm::vec4 color)
{
	shader.SetVector3f("color", color.r, color.g, color.b);
	shader.SetFloat("alpha", color.a);
	glLineWidth(1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Primitive::DrawWithLines(Shader shader, glm::vec4 color)
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	shader.SetVector3f("color", color.r, color.g, color.b);
	shader.SetFloat("alpha", color.a);
	glLineWidth(1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool Primitive::Intersection(glm::vec3 orig, glm::vec3 dir)
{
	if (indices.size() >= 3)
	{
		for (int i = 0; i < indices.size();)
		{
			glm::vec3 vert0 = vertices.at(indices.at(i)).Position;
			glm::vec3 vert1 = vertices.at(indices.at(i + 1)).Position;
			glm::vec3 vert2 = vertices.at(indices.at(i + 2)).Position;
			glm::vec2 pos;
			float dist;
			if (glm::intersectRayTriangle(orig, dir, vert0, vert1, vert2, pos, dist))
				return true;

			i = i + 3;
		}
	}
	else if (indices.size() == 2)
	{
		glm::vec3 vert0 = vertices.at(indices.at(0)).Position;
		glm::vec3 vert1 = vertices.at(indices.at(1)).Position;
		glm::vec3 vert2 = vert1;
		glm::vec2 pos;
		float dist;
		if (glm::intersectRayTriangle(orig, dir, vert0, vert1, vert2, pos, dist))
			return true;
	}

	return false;
}

*/
// PROTECTED:
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void Primitive::addVertex(Vertex v)
{
	vertices.push_back(v);
}

void Primitive::addEdge(Edge e)
{
	edges.push_back(e);
}

void Primitive::addFace(Face f)
{
	faces.push_back(f);
}

void Primitive::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
	indices.push_back(i1);
	indices.push_back(i2);
	indices.push_back(i3);
}

void Primitive::addIndices(unsigned int i1, unsigned int i2)
{
	indices.push_back(i1);
	indices.push_back(i2);
}

glm::vec3 Primitive::computeFaceNormal(Vertex v1, Vertex v2, Vertex v3)
{
	const float EPSILON = 0.000001f;

	glm::vec3 normal(0.0f);     // default return value (0,0,0)
	float nx, ny, nz;

	// find 2 edge vectors: v1-v2, v1-v3
	float ex1 = v2.Position.x - v1.Position.x;
	float ey1 = v2.Position.y - v1.Position.y;
	float ez1 = v2.Position.z - v1.Position.z;
	float ex2 = v3.Position.x - v1.Position.x;
	float ey2 = v3.Position.y - v1.Position.y;
	float ez2 = v3.Position.z - v1.Position.z;

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

void Primitive::setupVAO()
{
	// create buffers/arrays
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void Primitive::updateVAO()
{
	glBindVertexArray(vao);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void Primitive::clearArrays()
{
	vertices.clear();
	indices.clear();
	edges.clear();
	faces.clear();
}
*/