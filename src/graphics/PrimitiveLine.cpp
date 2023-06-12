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
 * @file primitive_line.cpp
 * @brief
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#include "PrimitiveLine.h"


// PUBLIC:
/////////////////////////////////////////////////////////////////////////////////////////////////////////

Line::Line(std::shared_ptr<ILogger> logger, std::shared_ptr<Resources> resources, std::string meshName, glm::vec3 begin, glm::vec3 end) : Primitive(logger, resources, meshName)
{
	addMesh(meshName);
	VertexNew v0, v1;

	v0.ID = 0;
	v1.ID = 1;

	v0.TexCoords = glm::vec2(0.0f, 0.0f);
	v1.TexCoords = glm::vec2(1.0f, 1.0f);

	v0.Normal = glm::vec3(begin - end);
	v0.Normal = glm::vec3(end - begin);

	v0.Position = begin;
	v1.Position = end;

	addVertex(name_, v0);
	addVertex(name_, v1);

	addIndices(name_, 0, 1);

	EdgeNew e;
	e.ID = 0;
	e.VertIds.push_back(0);
	e.VertIds.push_back(1);
	e.Length = glm::length(v0.Position - v1.Position);
	e.SharingFacesIds = std::vector<GLuint>();

	glm::vec3 c = glm::vec3(0.0f);
	c.x = (v0.Position.x + v1.Position.x) / 2.0f;
	c.y = (v0.Position.y + v1.Position.y) / 2.0f;
	c.z = (v0.Position.z + v1.Position.z) / 2.0f;
	e.Center = c;

	addEdge(name_, e);

	updateVAO(name_);
}

glm::vec3 Line::GetLineEndPosition()
{
	return getVertexPosition(name_, 1);
}

glm::vec3 Line::GetLineStartPosition()
{
	return getVertexPosition(name_, 0);
}

GLvoid Line::SetStartPosition(glm::vec3 p)
{
	setVertexPosition(name_, 0, p);
	//vertices.at(0).Position = p;
	updateVAO(name_);
}

GLvoid Line::SetEndPosition(glm::vec3 p)
{
	setVertexPosition(name_, 1, p);
	//vertices.at(1).Position = p;
	updateVAO(name_);
}

GLvoid Line::Draw()
{
	glBindVertexArray(getVAOId(name_));
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}