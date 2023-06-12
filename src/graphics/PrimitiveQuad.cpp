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
 * @file primitive_cube.cpp
 * @author Kirsch Audio GmbH
 * @brief This file contains the Primitive Cube class, which is used to create and draw cubes.
 */


#include "PrimitiveQuad.h"


 // PUBLIC:
 /////////////////////////////////////////////////////////////////////////////////////////////////////////

Quad::Quad(std::shared_ptr<ILogger> logger, std::shared_ptr<Resources> resources, std::string meshName) : Primitive(logger, resources, meshName)
{
	addMesh(meshName);
	smooth_ = false;

	build();
}



// PRIVATE:
/////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// build vertices of quad with flat shading
///////////////////////////////////////////////////////////////////////////////
GLvoid Quad::build()
{
	clearMesh(name_);
	VertexNew tmpVertexA;
	VertexNew tmpVertexB;
	VertexNew tmpVertexC;
	VertexNew tmpVertexD;

	// vertex position
	
	// A
	tmpVertexA.ID = 0;
	tmpVertexA.Position = { -0.5f, -0.5f, 0.0f };
	tmpVertexA.Normal = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	tmpVertexA.TexCoords = { 0.0f, 0.0f };
	addVertex(name_, tmpVertexA);

	// B
	tmpVertexB.ID = 1;
	tmpVertexB.Position = { -0.5f, 0.5f, 0.0f };
	tmpVertexB.Normal = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	tmpVertexB.TexCoords = { 0.0f, 1.0f };
	addVertex(name_, tmpVertexB);

	// C
	tmpVertexC.ID = 2;
	tmpVertexC.Position = { 0.5f, 0.5f, 0.0f };
	tmpVertexC.Normal = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	tmpVertexC.TexCoords = { 1.0f, 1.0f };
	addVertex(name_, tmpVertexC);

	// D
	tmpVertexD.ID = 3;
	tmpVertexD.Position = { 0.5f, -0.5f, 0.0f };
	tmpVertexD.Normal = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	tmpVertexD.TexCoords = { 1.0f, 0.0f };
	addVertex(name_, tmpVertexD);

	// add indicies
	addIndices(name_, 0, 1, 2);
	addIndices(name_, 0, 2, 3);

	// vertical edges
	EdgeNew tmpEdgeAB;
	EdgeNew tmpEdgeBC;
	EdgeNew tmpEdgeCD;
	EdgeNew tmpEdgeDA;

	tmpEdgeAB.ID = 0;
	tmpEdgeAB.VertIds.push_back(0);
	tmpEdgeAB.VertIds.push_back(1);
	tmpEdgeAB.Length = 2.0f;
	tmpEdgeAB.Center = glm::vec3(-0.5f, 0.0f, 0.0f);

	tmpEdgeBC.ID = 1;
	tmpEdgeBC.VertIds.push_back(1);
	tmpEdgeBC.VertIds.push_back(2);
	tmpEdgeBC.Length = 2.0f;
	tmpEdgeBC.Center = glm::vec3(0.0f, 0.5f, 0.0f);

	tmpEdgeCD.ID = 2;
	tmpEdgeCD.VertIds.push_back(2);
	tmpEdgeCD.VertIds.push_back(3);
	tmpEdgeCD.Length = 2.0f;
	tmpEdgeCD.Center = glm::vec3(0.5f, 0.0f, 0.0f);

	tmpEdgeDA.ID = 3;
	tmpEdgeDA.VertIds.push_back(3);
	tmpEdgeDA.VertIds.push_back(0);
	tmpEdgeDA.Length = 2.0f;
	tmpEdgeDA.Center = glm::vec3(0.0f, -0.5f, 0.0f);

	addEdge(name_, tmpEdgeAB);
	addEdge(name_, tmpEdgeBC);
	addEdge(name_, tmpEdgeCD);
	addEdge(name_, tmpEdgeDA);

	updateVAO(name_);
}