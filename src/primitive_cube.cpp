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
 * @file primitive_cube.cpp
 * @brief
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#include "../include/primitive_cube.h"


 // PUBLIC:
 /////////////////////////////////////////////////////////////////////////////////////////////////////////

Cube::Cube(std::string meshName, GLboolean smooth) : Primitive(meshName)
{
	addMesh(meshName);
	smooth_ = smooth;

	if (smooth_)
		buildVerticesSmooth();
	else
		buildVerticesFlat();
}

GLvoid Cube::SetEdge(glm::vec2 pos, GLuint id)
{
	EdgeNew edge = getEdge(name_, id);
	for (int i = 0; i < edge.VertIds.size(); i++)
	{
		glm::vec3 tmpVert = getVertexPosition(name_, edge.VertIds.at(i));
		tmpVert.x = pos.x;
		tmpVert.z = pos.y;
		setVertexPosition(name_, edge.VertIds.at(i), tmpVert);
	}

	updateVAO(name_);
}

GLvoid Cube::SetHeight(GLfloat topHeight, GLfloat bottomHeight)
{
	if (smooth_)
	{
		setVertexPositionY(name_, 0, topHeight);
		setVertexPositionY(name_, 3, topHeight);
		setVertexPositionY(name_, 4, topHeight);
		setVertexPositionY(name_, 7, topHeight);
		
		setVertexPositionY(name_, 1, bottomHeight);
		setVertexPositionY(name_, 2, bottomHeight);
		setVertexPositionY(name_, 5, bottomHeight);
		setVertexPositionY(name_, 6, bottomHeight);
	}
	else
	{
		setVertexPositionY(name_, 2, topHeight);
		setVertexPositionY(name_, 3, topHeight);
		setVertexPositionY(name_, 4, topHeight);
		setVertexPositionY(name_, 8, topHeight);
		setVertexPositionY(name_, 9, topHeight);
		setVertexPositionY(name_, 10, topHeight);
		setVertexPositionY(name_, 12, topHeight);
		setVertexPositionY(name_, 13, topHeight);
		setVertexPositionY(name_, 17, topHeight);
		setVertexPositionY(name_, 18, topHeight);
		setVertexPositionY(name_, 19, topHeight);
		setVertexPositionY(name_, 23, topHeight);
		setVertexPositionY(name_, 30, topHeight);
		setVertexPositionY(name_, 31, topHeight);
		setVertexPositionY(name_, 32, topHeight);
		setVertexPositionY(name_, 33, topHeight);
		setVertexPositionY(name_, 34, topHeight);
		setVertexPositionY(name_, 35, topHeight);

		setVertexPositionY(name_, 0, bottomHeight);
		setVertexPositionY(name_, 1, bottomHeight);
		setVertexPositionY(name_, 5, bottomHeight);
		setVertexPositionY(name_, 6, bottomHeight);
		setVertexPositionY(name_, 7, bottomHeight);
		setVertexPositionY(name_, 11, bottomHeight);
		setVertexPositionY(name_, 14, bottomHeight);
		setVertexPositionY(name_, 15, bottomHeight);
		setVertexPositionY(name_, 16, bottomHeight);
		setVertexPositionY(name_, 20, bottomHeight);
		setVertexPositionY(name_, 21, bottomHeight);
		setVertexPositionY(name_, 22, bottomHeight);
		setVertexPositionY(name_, 24, bottomHeight);
		setVertexPositionY(name_, 25, bottomHeight);
		setVertexPositionY(name_, 26, bottomHeight);
		setVertexPositionY(name_, 27, bottomHeight);
		setVertexPositionY(name_, 28, bottomHeight);
		setVertexPositionY(name_, 29, bottomHeight);
	}

	for (int i = 0; i < getNumberOfEdges(name_); i++)
	{
		setEdgeLength(name_, i, glm::abs(topHeight - bottomHeight));
	}
}

// PRIVATE:
/////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// build vertices of cube with smooth shading
//						
//	  E-------A		  y ^ z
//	 /|	     /|			|/
//	H-------D |			+--->x
//	| F-----|-B
//	|/		|/
//	G-------C
//
//	  4-------0
//	 /|	     /|
//	7-------3 |
//	| 5-----|-1
//	|/		|/
//	6-------2
///////////////////////////////////////////////////////////////////////////////
GLvoid Cube::buildVerticesSmooth()
{
    clearMesh(name_);
	VertexNew tmpVertexA;
	VertexNew tmpVertexB;
	VertexNew tmpVertexC;
	VertexNew tmpVertexD;
	VertexNew tmpVertexE;
	VertexNew tmpVertexF;
	VertexNew tmpVertexG;
	VertexNew tmpVertexH;
	
	// vertex position
	// A
	tmpVertexA.ID = 0;
	tmpVertexA.Position	= { 1.0f, 1.0f, 1.0f};
	tmpVertexA.Normal	= glm::normalize(glm::vec3( 1.0f, 1.0f, 1.0f));
	tmpVertexA.TexCoords = { 1.0f, 1.0f};
	addVertex(name_, tmpVertexA);

	// B
	tmpVertexB.ID = 1;
	tmpVertexB.Position	= { 1.0f,-1.0f, 1.0f};
	tmpVertexB.Normal = glm::normalize(glm::vec3( 1.0f,-1.0f, 1.0f));
	tmpVertexB.TexCoords = { 1.0f, 0.0f };
	addVertex(name_, tmpVertexB);

	// C
	tmpVertexC.ID = 2;
	tmpVertexC.Position = { 1.0f,-1.0f,-1.0f };
	tmpVertexC.Normal = glm::normalize(glm::vec3( 1.0f,-1.0f,-1.0f));
	tmpVertexC.TexCoords = { 0.0f, 0.0f };
	addVertex(name_, tmpVertexC);

	// D
	tmpVertexD.ID = 3;
	tmpVertexD.Position	= { 1.0f, 1.0f,-1.0f};
	tmpVertexD.Normal = glm::normalize(glm::vec3( 1.0f, 1.0f,-1.0f));
	tmpVertexD.TexCoords = { 0.0f, 1.0f };
	addVertex(name_, tmpVertexD);

	// E
	tmpVertexE.ID = 4;
	tmpVertexE.Position	= {-1.0f, 1.0f, 1.0f};
	tmpVertexE.Normal = glm::normalize(glm::vec3(-1.0f, 1.0f, 1.0f));
	tmpVertexE.TexCoords = { 0.0f, 1.0f };
	addVertex(name_, tmpVertexE);

	// F
	tmpVertexF.ID = 5;
	tmpVertexF.Position	= {-1.0f,-1.0f, 1.0f};
	tmpVertexF.Normal = glm::normalize(glm::vec3(-1.0f,-1.0f, 1.0f));
	tmpVertexF.TexCoords = { 0.0f, 1.0f };
	addVertex(name_, tmpVertexF);

	// G
	tmpVertexG.ID = 6;
	tmpVertexG.Position	= {-1.0f,-1.0f,-1.0f};
	tmpVertexG.Normal = glm::normalize(glm::vec3(-1.0f,-1.0f,-1.0f));
	tmpVertexG.TexCoords = { 0.0f, 1.0f };
	addVertex(name_, tmpVertexG);

	// H
	tmpVertexH.ID = 7;
	tmpVertexH.Position	= {-1.0f, 1.0f,-1.0f};
	tmpVertexH.Normal = glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f));
	tmpVertexH.TexCoords = { 0.0f, 0.0f };
	addVertex(name_, tmpVertexH);

	// add indicies
	addIndices(name_, 0, 1, 2);
	addIndices(name_, 0, 2, 3);
	addIndices(name_, 3, 2, 6);
	addIndices(name_, 3, 6, 7);
	addIndices(name_, 0, 3, 7);
	addIndices(name_, 0, 7, 4);
	addIndices(name_, 1, 6, 2);
	addIndices(name_, 1, 5, 6);
	addIndices(name_, 4, 7, 6);
	addIndices(name_, 4, 6, 5);
	addIndices(name_, 0, 4, 5);
	addIndices(name_, 0, 5, 1);

	// vertical edges
	EdgeNew tmpEdge0;
	EdgeNew tmpEdge1;
	EdgeNew tmpEdge2;
	EdgeNew tmpEdge3;

	tmpEdge0.ID = 0;
	tmpEdge0.VertIds.push_back(0);
	tmpEdge0.VertIds.push_back(1);
	tmpEdge0.Length = 2.0f;
	tmpEdge0.Center = glm::vec3(1.0f, 0.0f, 1.0f);

	tmpEdge1.ID = 1;
	tmpEdge1.VertIds.push_back(3);
	tmpEdge1.VertIds.push_back(2);
	tmpEdge1.Length = 2.0f;
	tmpEdge1.Center = glm::vec3(1.0f, 0.0f,-1.0f);

	tmpEdge2.ID = 2;
	tmpEdge2.VertIds.push_back(7);
	tmpEdge2.VertIds.push_back(6);
	tmpEdge2.Length = 2.0f;
	tmpEdge2.Center = glm::vec3(-1.0f, 0.0f,-1.0f);

	tmpEdge3.ID = 3;
	tmpEdge3.VertIds.push_back(4);
	tmpEdge3.VertIds.push_back(5);
	tmpEdge3.Length = 2.0f;
	tmpEdge3.Center = glm::vec3(-1.0f, 0.0f, 1.0f);

	addEdge(name_, tmpEdge0);
	addEdge(name_, tmpEdge1);
	addEdge(name_, tmpEdge2);
	addEdge(name_, tmpEdge3);

	updateVAO(name_);
}

///////////////////////////////////////////////////////////////////////////////
// build vertices of cube with flat shading
///////////////////////////////////////////////////////////////////////////////
GLvoid Cube::buildVerticesFlat()
{
	clearMesh(name_);

	float cubeVerts[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,		//  0
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,		//  1
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,		//  2
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,		//  3
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,		//  4
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,		//  5

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,		//  6
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,		//  7
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,		//  8
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,		//  9
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,		// 10
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,		// 11

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,		// 12
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,		// 13
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,		// 14
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,		// 15
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,		// 16
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,		// 17

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,		// 18
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,		// 19
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,		// 20
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,		// 21
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,		// 22
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,		// 23

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,		// 24
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,		// 25
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,		// 26
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,		// 27
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,		// 28
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,		// 29

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,		// 30
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,		// 31
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,		// 32
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,		// 33
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,		// 34
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f		// 35
	};

	for (int i = 0; i < 36; i++)
	{
		VertexNew tmpVertex;
		tmpVertex.Position.x = cubeVerts[i * 8 + 0] * 2.0f;
		tmpVertex.Position.y = cubeVerts[i * 8 + 1] * 2.0f;
		tmpVertex.Position.z = cubeVerts[i * 8 + 2] * 2.0f;
		tmpVertex.Normal.x = cubeVerts[i * 8 + 3];
		tmpVertex.Normal.y = cubeVerts[i * 8 + 4];
		tmpVertex.Normal.z = cubeVerts[i * 8 + 5];
		tmpVertex.TexCoords.s = cubeVerts[i * 8 + 6];
		tmpVertex.TexCoords.t = cubeVerts[i * 8 + 7];

		tmpVertex.ID = i;

		addVertex(name_, tmpVertex);
	}

	for (int i = 0; i < 36;)
	{
		addIndices(name_, i, i+1, i+2);
		i += 3;
	}

	// vertical edges
	EdgeNew tmpEdge0;
	EdgeNew tmpEdge1;
	EdgeNew tmpEdge2;
	EdgeNew tmpEdge3;

	tmpEdge0.ID = 0;
	tmpEdge0.VertIds.push_back(0);
	tmpEdge0.VertIds.push_back(5);
	tmpEdge0.VertIds.push_back(14);
	tmpEdge0.VertIds.push_back(15);
	tmpEdge0.VertIds.push_back(24);
	tmpEdge0.VertIds.push_back(29);
	tmpEdge0.VertIds.push_back(4);
	tmpEdge0.VertIds.push_back(13);
	tmpEdge0.VertIds.push_back(30);
	tmpEdge0.VertIds.push_back(35);
	tmpEdge0.Length = 2.0f;
	tmpEdge0.Center = glm::vec3(-1.0f, 0.0f,-1.0f);

	tmpEdge1.ID = 1;
	tmpEdge1.VertIds.push_back(1);
	tmpEdge1.VertIds.push_back(20);
	tmpEdge1.VertIds.push_back(21);
	tmpEdge1.VertIds.push_back(25);
	tmpEdge1.VertIds.push_back(2);
	tmpEdge1.VertIds.push_back(3);
	tmpEdge1.VertIds.push_back(19);
	tmpEdge1.VertIds.push_back(31);
	tmpEdge1.Length = 2.0f;
	tmpEdge1.Center = glm::vec3(1.0f, 0.0f, -1.0f);

	tmpEdge2.ID = 2;
	tmpEdge2.VertIds.push_back(7);
	tmpEdge2.VertIds.push_back(22);
	tmpEdge2.VertIds.push_back(26);
	tmpEdge2.VertIds.push_back(27);
	tmpEdge2.VertIds.push_back(8);
	tmpEdge2.VertIds.push_back(9);
	tmpEdge2.VertIds.push_back(18);
	tmpEdge2.VertIds.push_back(23);
	tmpEdge2.VertIds.push_back(32);
	tmpEdge2.VertIds.push_back(33);
	tmpEdge2.Length = 2.0f;
	tmpEdge2.Center = glm::vec3(-1.0f, 0.0f, -1.0f);

	tmpEdge3.ID = 3;
	tmpEdge3.VertIds.push_back(10);
	tmpEdge3.VertIds.push_back(12);
	tmpEdge3.VertIds.push_back(17);
	tmpEdge3.VertIds.push_back(34);
	tmpEdge3.VertIds.push_back(6);
	tmpEdge3.VertIds.push_back(11);
	tmpEdge3.VertIds.push_back(16);
	tmpEdge3.VertIds.push_back(28);
	tmpEdge3.Length = 2.0f;
	tmpEdge3.Center = glm::vec3(-1.0f, 0.0f, 1.0f);

	addEdge(name_, tmpEdge0);
	addEdge(name_, tmpEdge1);
	addEdge(name_, tmpEdge2);
	addEdge(name_, tmpEdge3);

	updateVAO(name_);
}