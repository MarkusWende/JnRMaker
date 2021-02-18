#ifndef MESH_NEW_H
#define MESH_NEW_H

#include <algorithm>
#include <ppl.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/intersect.hpp>

#include "message_manager.h"
#include "resource_manager.h"
#include "time_helper.h"

struct VertexNew {
	glm::vec3		Position;
	glm::vec3		Normal;
	glm::vec2		TexCoords;
	GLuint			ID;
};

struct EdgeNew {
	std::vector<GLuint>	VertIds;
	std::vector<GLuint>	SharingFacesIds;
	glm::vec3			Center;
	GLfloat				Length;
	GLuint				ID;
};

struct FaceNew {
	std::vector<GLuint>	VertIds;
	glm::vec3			Center;
	std::vector<GLuint>	NeighborFaceIds;
	GLuint				ID;
	glm::vec3			Normal;
	GLfloat				Cost;
	//GLuint				Size;	//TODO (Markus): Size is not used and can be removed?
};

class MeshNew
{
public:

	MeshNew()
	{
		init("Mesh");
	}

	MeshNew(std::string meshName)
	{
		init(meshName);
	}

	~MeshNew()
	{
		// cleaning up
		glDeleteVertexArrays(1, &vao_);
		glDeleteBuffers(1, &vbo_);
		glDeleteBuffers(1, &ebo_);
	}

	GLvoid Draw()
	{
		glBindVertexArray(vao_);
		glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	GLvoid DrawWireframe()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(vao_);
		glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//Warning: If if orig+dir is pointing on an edge between two triangles, this intersection does not always work
	GLboolean Intersection(glm::vec3 orig, glm::vec3 dir, glm::vec3& pos)
	{
		if (indices_.size() >= 3)
		{
			for (int i = 0; i < indices_.size();i=i+3)
			{
				glm::vec3 vert0 = vertices_.at(indices_.at(i)).Position;
				glm::vec3 vert1 = vertices_.at(indices_.at(i + 1)).Position;
				glm::vec3 vert2 = vertices_.at(indices_.at(i + 2)).Position;

				glm::vec3 c = glm::vec3(0.0f);
				c.x = (vert0.x + vert1.x + vert2.x) / 3.0f;
				c.y = (vert0.y + vert1.y + vert2.y) / 3.0f;
				c.z = (vert0.z + vert1.z + vert2.z) / 3.0f;

				vert0 = c + (vert0 - c) * 1.00001f;
				vert1 = c + (vert1 - c) * 1.00001f;
				vert2 = c + (vert2 - c) * 1.00001f;

				glm::vec2 position;
				float dist;
				if (glm::intersectRayTriangle(orig, dir, vert0, vert1, vert2, position, dist))
				{
					pos = orig + dir * dist;
					if ((pos.x >= -1.0f && pos.x <= 1.0f) && (pos.z >= -1.0f && pos.z <= 1.0f))
					{
						return true;
					}
				}
				else
				{
					//detection whether there is an intersection with the edges of the triangle
					/*
					if (vecVecIntersection(vert0, vert1, orig, glm::vec3(orig + glm::vec3(0.0f, 2.0f, 0.0f)), pos))
						return true;
					else if (vecVecIntersection(vert0, vert2, orig, glm::vec3(orig + glm::vec3(0.0f, 2.0f, 0.0f)), pos))
						return true;
					else if (vecVecIntersection(vert1, vert2, orig, glm::vec3(orig + glm::vec3(0.0f, 2.0f, 0.0f)), pos))
						return true;
					*/
				}
			}

			//Solution until edge intersection detection is implemented
			/*
			if (orig.x + orig.z == 0)
			{
				for (int i = 0; i < indices_.size();i = i + 3)
				{
					glm::vec3 vert0 = vertices_.at(indices_.at(i)).Position;
					glm::vec3 vert1 = vertices_.at(indices_.at(i + 1)).Position;
					glm::vec3 vert2 = vertices_.at(indices_.at(i + 2)).Position;

					//sortVertPositions(vert0, vert1, vert2);

					glm::vec3 new_orig = glm::vec3(orig.x + 0.0000001, orig.y, orig.z + 0.0000001);
					glm::vec2 position;
					float dist;
					if (glm::intersectRayTriangle(new_orig, dir, vert0, vert1, vert2, position, dist))
					{
						pos = orig + dir * dist;
						if ((pos.x >= -1.0f && pos.x <= 1.0f) && (pos.z >= -1.0f && pos.z <= 1.0f))
						{
							return true;
						}
					}
				}
			}
			*/
		}
		else if (indices_.size() == 2)			// Intersection with a line
		{
			glm::vec3 vert0 = vertices_.at(indices_.at(0)).Position;
			glm::vec3 vert1 = vertices_.at(indices_.at(1)).Position;
			glm::vec3 vert2 = vert1;
			glm::vec2 position;
			float dist;
			if (glm::intersectRayTriangle(orig, dir, vert0, vert1, vert2, position, dist))
				return true;
		}

		pos = { -99.0f, -99.0f, -99.0f };
		return false;
	}

	GLboolean IntersectionCurve(glm::vec3 start, glm::vec3 end, std::vector<glm::vec3>& intersectCurve)
	{
		if (indices_.size() >= 3)
		{

			for (size_t i = 0; i < indices_.size()-1; i++)
			{
				glm::vec3 vert0 = vertices_.at(indices_.at(i)).Position;
				glm::vec3 vert1 = vertices_.at(indices_.at(i+1)).Position;

				glm::vec3 orig = vert0;
				glm::vec3 dir0 = glm::normalize(vert1 - vert0);
				glm::vec3 dir1 = glm::normalize(vert0 - vert1);

				glm::vec2 tmpPosition;		// not used
				glm::vec3 position;
				GLfloat dist;

				GLfloat length = glm::distance(vert0, vert1);


				if (glm::intersectRayTriangle(orig, dir0, start, end, start + glm::vec3(0.0f, -1.0f, 0.0f), tmpPosition, dist))
				{
					position = orig + dir0 * dist;
					if ((position.x >= -1.0f && position.x <= 1.0f) && (position.z >= -1.0f && position.z <= 1.0f))
					{
						if ((glm::length(vert0 - position) + glm::length(vert1 - position)) <= (length + 0.001f))
						{
							{
								intersectCurve.push_back(position);
							}
						}
					}
				}
				else if (glm::intersectRayTriangle(orig, dir0, start, end, end + glm::vec3(0.0f, 1.0f, 0.0f), tmpPosition, dist))
				{
					position = orig + dir0 * dist;
					if ((position.x >= -1.0f && position.x <= 1.0f) && (position.z >= -1.0f && position.z <= 1.0f))
					{
						if ((glm::length(vert0 - position) + glm::length(vert1 - position)) <= (length + 0.001f))
						{
							{
								intersectCurve.push_back(position);
							}
						}
					}
				}
				else if (glm::intersectRayTriangle(orig, dir1, start, end, start + glm::vec3(0.0f, -1.0f, 0.0f), tmpPosition, dist))
				{
					position = orig + dir1 * dist;
					if ((position.x >= -1.0f && position.x <= 1.0f) && (position.z >= -1.0f && position.z <= 1.0f))
					{
						if ((glm::length(vert0 - position) + glm::length(vert1 - position)) <= (length + 0.001f))
						{
							{
								intersectCurve.push_back(position);
							}
						}
					}
				}
				else if (glm::intersectRayTriangle(orig, dir1, start, end, end + glm::vec3(0.0f, 1.0f, 0.0f), tmpPosition, dist))
				{
					position = orig + dir1 * dist;
					if ((position.x >= -1.0f && position.x <= 1.0f) && (position.z >= -1.0f && position.z <= 1.0f))
					{
						if ((glm::length(vert0 - position) + glm::length(vert1 - position)) <= (length + 0.001f))
						{
							{
								intersectCurve.push_back(position);
							}
						}
					}
				}

			}

			if (intersectCurve.empty())
			{
				return false;
			}
		}
		else
			return false;

		return true;
	}

	GLvoid AddVertex(VertexNew v)
	{
		vertices_.push_back(v);
	}

	GLvoid AddEdge(EdgeNew e)
	{
		GLuint edgeKey = getHash(e.VertIds[0], e.VertIds[1]);
		edges_.insert(std::pair<GLuint, EdgeNew>(edgeKey, e));
	}

	GLvoid AddFace(FaceNew v)
	{
		faces_.push_back(v);
	}

	GLvoid AddIndex(GLuint i)
	{
		indices_.push_back(i);
	}

	GLvoid ClearMesh()
	{
		vertices_.clear();
		indices_.clear();
		edges_.clear();
		faces_.clear();
	}

	GLvoid Triangulate()
	{
		// Clear all faces and edges
		faces_.clear();
		edges_.clear();

		// Get number of faces
		GLuint numFaces = indices_.size() / 3;

		// Clear vertice normals
		for (unsigned int i = 0; i < numFaces; i++)
		{
			GLuint v0ID = vertices_.at(indices_.at(i * 3)).ID;
			GLuint v1ID = vertices_.at(indices_.at(i * 3 + 1)).ID;
			GLuint v2ID = vertices_.at(indices_.at(i * 3 + 2)).ID;
			vertices_.at(v0ID).Normal = glm::vec3(0.0f, 0.0f, 0.0f);
			vertices_.at(v1ID).Normal = glm::vec3(0.0f, 0.0f, 0.0f);
			vertices_.at(v2ID).Normal = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		for (unsigned int i = 0; i < numFaces; i++)
		{
			// Generating face
			FaceNew newFace;

			// Face id
			newFace.ID = i;

			// Face vertices
			GLuint v0ID = vertices_.at(indices_.at(i * 3)).ID;
			GLuint v1ID = vertices_.at(indices_.at(i * 3 + 1)).ID;
			GLuint v2ID = vertices_.at(indices_.at(i * 3 + 2)).ID;
			newFace.VertIds.push_back(v0ID);
			newFace.VertIds.push_back(v1ID);
			newFace.VertIds.push_back(v2ID);

			// Face center
			newFace.Center = getCenter(v0ID, v1ID, v2ID);

			// Cost (initialize with 0)// Ini
			newFace.Cost = 0.0f;

			// Normal
			newFace.Normal = computeFaceNormal(vertices_.at(v0ID).Position, vertices_.at(v1ID).Position, vertices_.at(v2ID).Position);
			if (newFace.Normal.y < 0.0f)
			{
				newFace.Normal = newFace.Normal * -1.0f;
			}

			faces_.push_back(newFace);

			// Update face vertice normals
			vertices_.at(v0ID).Normal += newFace.Normal;
			vertices_.at(v1ID).Normal += newFace.Normal;
			vertices_.at(v2ID).Normal += newFace.Normal;

			// Generating edges for face
			GLuint edge0Key = getHash(v0ID, v1ID);
			GLuint edge1Key = getHash(v1ID, v2ID);
			GLuint edge2Key = getHash(v2ID, v0ID);

			std::map<GLuint, EdgeNew>::iterator itEdge0, itEdge1, itEdge2;
			itEdge0 = edges_.find(edge0Key);
			itEdge1 = edges_.find(edge1Key);
			itEdge2 = edges_.find(edge2Key);

			if (itEdge0 != edges_.end())
			{
				edges_[edge0Key].SharingFacesIds.push_back(newFace.ID);
			}
			else
			{
				EdgeNew edge0;
				edge0.VertIds.push_back(v0ID);
				edge0.VertIds.push_back(v1ID);
				edge0.Center = getCenter(v0ID, v1ID);
				edge0.Length = glm::length(vertices_.at(indices_.at(i * 3)).Position - vertices_.at(indices_.at(i * 3 + 1)).Position);
				edge0.SharingFacesIds.push_back(newFace.ID);
				edge0.ID = edge0Key;
				edges_.insert(std::pair<GLuint, EdgeNew>(edge0Key, edge0));
			}

			if (itEdge1 != edges_.end())
			{
				edges_[edge1Key].SharingFacesIds.push_back(newFace.ID);
			}
			else
			{
				EdgeNew edge1;
				edge1.VertIds.push_back(v1ID);
				edge1.VertIds.push_back(v2ID);
				edge1.Center = getCenter(v1ID, v2ID);
				edge1.Length = glm::length(vertices_.at(indices_.at(i * 3 + 1)).Position - vertices_.at(indices_.at(i * 3 + 2)).Position);
				edge1.SharingFacesIds.push_back(newFace.ID);
				edge1.ID = edge1Key;
				edges_.insert(std::pair<GLuint, EdgeNew>(edge1Key, edge1));
			}

			if (itEdge2 != edges_.end())
			{
				edges_[edge2Key].SharingFacesIds.push_back(newFace.ID);
			}
			else
			{
				EdgeNew edge2;
				edge2.VertIds.push_back(v2ID);
				edge2.VertIds.push_back(v0ID);
				edge2.Center = getCenter(v2ID, v0ID);
				edge2.Length = glm::length(vertices_.at(indices_.at(i * 3 + 2)).Position - vertices_.at(indices_.at(i * 3)).Position);
				edge2.SharingFacesIds.push_back(newFace.ID);
				edge2.ID = edge2Key;
				edges_.insert(std::pair<GLuint, EdgeNew>(edge2Key, edge2));
			}
		}

		// Normalize vertice normals
		for (unsigned int i = 0; i < numFaces; i++)
		{
			GLuint v0ID = vertices_.at(indices_.at(i * 3)).ID;
			GLuint v1ID = vertices_.at(indices_.at(i * 3 + 1)).ID;
			GLuint v2ID = vertices_.at(indices_.at(i * 3 + 2)).ID;
			vertices_.at(v0ID).Normal = glm::normalize(vertices_.at(v0ID).Normal);
			vertices_.at(v1ID).Normal = glm::normalize(vertices_.at(v1ID).Normal);
			vertices_.at(v2ID).Normal = glm::normalize(vertices_.at(v2ID).Normal);
		}

		// Generate neighbor face ids
		for (unsigned int i = 0; i < faces_.size(); i++)
		{
			GLuint v0ID = faces_.at(i).VertIds.at(0);
			GLuint v1ID = faces_.at(i).VertIds.at(1);
			GLuint v2ID = faces_.at(i).VertIds.at(2);

			for (int j = 0; j < edges_[getHash(v0ID, v1ID)].SharingFacesIds.size(); j++)
			{
				if (edges_[getHash(v0ID, v1ID)].SharingFacesIds.at(j) != faces_.at(i).ID)
					faces_.at(i).NeighborFaceIds.push_back(faces_.at(edges_[getHash(v0ID, v1ID)].SharingFacesIds.at(j)).ID);
			}
			for (int j = 0; j < edges_[getHash(v1ID, v2ID)].SharingFacesIds.size(); j++)
			{
				if (edges_[getHash(v1ID, v2ID)].SharingFacesIds.at(j) != faces_.at(i).ID)
					faces_.at(i).NeighborFaceIds.push_back(faces_.at(edges_[getHash(v1ID, v2ID)].SharingFacesIds.at(j)).ID);
			}
			for (int j = 0; j < edges_[getHash(v2ID, v0ID)].SharingFacesIds.size(); j++)
			{
				if (edges_[getHash(v2ID, v0ID)].SharingFacesIds.at(j) != faces_.at(i).ID)
					faces_.at(i).NeighborFaceIds.push_back(faces_.at(edges_[getHash(v2ID, v0ID)].SharingFacesIds.at(j)).ID);
			}
		}

		queue_is_locked_ = true;

		queue_costs_.clear();

		for (int i = 0; i < faces_.size(); i++)
		{
			GLfloat cost = 0.0f;

			for (int j = 0; j < faces_.at(i).NeighborFaceIds.size(); j++)
			{
				if (faces_.at(i).NeighborFaceIds.size() == 3)
				{
					cost += glm::angle(faces_.at(i).Normal, faces_.at(faces_.at(i).NeighborFaceIds.at(j)).Normal);
				}
				else
					cost = 3.0f;
			}

			if (faces_.at(i).NeighborFaceIds.size() == 0)
				cost = 3.0f;

			cost = cost / 3.0f;
			faces_.at(i).Cost = cost;


			if (cost <= 0.001)
			{
				queue_costs_.insert(std::make_pair(i, cost));
			}
		}
		queue_is_locked_ = false;

		// clear faces and edges after usage
		faces_.clear();
		edges_.clear();
	}

	void Reduce()
	{
		int counter = 0;
		while (!queue_costs_.empty())
		{
			counter++;

			progress_mesh_reduction_ = (float)counter / (float)(queue_costs_.size() + counter);

			FaceNew f = faces_.at(queue_costs_.begin()->first);

			if (f.NeighborFaceIds.size() != 3)
				return;

			std::vector<GLuint>	indicesList;
			indicesList.push_back((f.ID) * 3);
			indicesList.push_back((faces_.at(f.NeighborFaceIds.at(0)).ID) * 3);
			indicesList.push_back((faces_.at(f.NeighborFaceIds.at(1)).ID) * 3);
			indicesList.push_back((faces_.at(f.NeighborFaceIds.at(2)).ID) * 3);

			std::vector<GLuint>	indicesNew;
			indicesNew.clear();

			std::sort(indicesList.begin(), indicesList.end(), std::less<int>());

			
			GLuint indicesListCounter = 0;
			for(int i = 0; i < indices_.size();)
			{
				if (i != indicesList.at(indicesListCounter))
				{
					indicesNew.push_back(indices_.at(i));
					indicesNew.push_back(indices_.at(i+1));
					indicesNew.push_back(indices_.at(i+2));
				}
				else
					if (indicesListCounter < indicesList.size()-1)
						indicesListCounter++;
				i = i + 3;
			}

			indicesNew.push_back(faces_.at(f.NeighborFaceIds.at(0)).VertIds.at(0));
			indicesNew.push_back(faces_.at(f.NeighborFaceIds.at(2)).VertIds.at(1));
			indicesNew.push_back(faces_.at(f.NeighborFaceIds.at(1)).VertIds.at(2));

			indices_.clear();
			indices_ = indicesNew;

			Triangulate();
		}
		faces_.clear();
		edges_.clear();
	}

	/**
	 * @brief Setup the OpenGL vertex array object.
	 * @return Void.
	 */
	GLvoid UpdateVAO()
	{
		glBindVertexArray(vao_);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(VertexNew), &vertices_[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		if (!indices_.empty())
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(indices_.data()), &indices_[0], GL_STATIC_DRAW);
		}

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNew), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNew), (void*)offsetof(VertexNew, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNew), (void*)offsetof(VertexNew, TexCoords));

		glBindVertexArray(0);
	}

	/** 
	* @brief Updates the normal vectors of the mesh. Indices have to be in counter-clockwise order for this to work (OpenGL standard)
	* @return
	*/
	GLvoid updateNormals() {
		//Finds all indice-entries for all vertices and calculates the vertice-normal by averaging over the normals of each adjacent face
		concurrency::parallel_for_each(std::begin(vertices_), std::end(vertices_), [&](VertexNew v) {

			std::vector<GLuint> indices_of_indices;
			std::vector<GLuint>::iterator iter;
			for (iter = indices_.begin(); iter < indices_.end(); iter++) { //searching for all indices with the vertex ID
				if (*iter == v.ID)
				{
					indices_of_indices.push_back(std::distance(indices_.begin(), iter)); //Add indices to list
				}
			}

			std::vector<glm::vec3> normals_of_faces;
			for (GLuint i : indices_of_indices) {
				if (i % 3 == 0) {
					glm::vec3 vectorOne = GetVertexPosition(indices_[i + 1]) - GetVertexPosition(indices_[i]);
					glm::vec3 vectorTwo = GetVertexPosition(indices_[i + 2]) - GetVertexPosition(indices_[i]);
					normals_of_faces.push_back(glm::normalize(glm::cross(vectorOne, vectorTwo)));
				}
				else if (i % 3 == 1) {
					glm::vec3 vectorOne = GetVertexPosition(indices_[i]) - GetVertexPosition(indices_[i - 1]);
					glm::vec3 vectorTwo = GetVertexPosition(indices_[i + 1]) - GetVertexPosition(indices_[i - 1]);
					normals_of_faces.push_back(glm::normalize(glm::cross(vectorOne, vectorTwo)));
				}
				else if (i % 3 == 2) {
					glm::vec3 vectorOne = GetVertexPosition(indices_[i - 1]) - GetVertexPosition(indices_[i - 2]);
					glm::vec3 vectorTwo = GetVertexPosition(indices_[i]) - GetVertexPosition(indices_[i - 2]);
					normals_of_faces.push_back(glm::normalize(glm::cross(vectorOne, vectorTwo)));
				}
			}
			glm::vec3 normal_of_vertex(0, 0, 0);
			for (glm::vec3 normal_of_face : normals_of_faces) {
				normal_of_vertex = normal_of_vertex + normal_of_face;
			}
			normal_of_vertex.x = normal_of_vertex.x / normals_of_faces.size();
			normal_of_vertex.y = normal_of_vertex.y / normals_of_faces.size();
			normal_of_vertex.z = normal_of_vertex.z / normals_of_faces.size();
			normal_of_vertex = glm::normalize(normal_of_vertex);
			SetVertexNormal(v.ID, normal_of_vertex);
			});
		return;
	}

	void GetCorners(VertexNew* v1, VertexNew* v2, VertexNew* v3, VertexNew* v4) {
		VertexNew lowerLeft, upperLeft, lowerRight, upperRight;
		lowerLeft.Position = glm::vec3(0, 0, 0);
		upperLeft.Position = glm::vec3(0, 0, 0);
		lowerRight.Position = glm::vec3(0, 0, 0);
		upperRight.Position = glm::vec3(0, 0, 0);
		for (auto vertex : vertices_)
		{
			if (vertex.Position.x <= lowerLeft.Position.x && vertex.Position.z <= lowerLeft.Position.z) 
			{
				lowerLeft = vertex;
			}
			if (vertex.Position.x <= upperLeft.Position.x && vertex.Position.z >= upperLeft.Position.z) 
			{
				upperLeft = vertex;
			}
			if (vertex.Position.x >= lowerRight.Position.x && vertex.Position.z <= lowerRight.Position.z)
			{
				lowerRight = vertex;
			}
			if (vertex.Position.x >= upperRight.Position.x && vertex.Position.z >= upperRight.Position.z)
			{
				upperRight = vertex;
			}
		}
		*v1 = lowerLeft;
		*v2 = upperLeft;
		*v3 = lowerRight;
		*v4 = upperRight;
		return;
	}

	GLuint		GetVAOId()								{ return vao_;  }
	glm::vec3	GetVertexPosition(GLuint id)			{ return vertices_.at(id).Position; }
	glm::vec3	GetVertexNormal(GLuint id)				{ return vertices_.at(id).Normal; }
	EdgeNew		GetEdge(GLuint id)						{ return edges_.at(id); }
	GLuint		GetNumberOfEdges()						{ return edges_.size(); }
	GLfloat		GetMeshReductionProgress()				{ return progress_mesh_reduction_; };
	std::vector<VertexNew>	GetVertices()				{ return vertices_; }
	std::vector<GLuint>		GetIndices()				{ return indices_; }

	GLvoid SetVertexPosition(GLuint id, glm::vec3 pos)	{ vertices_.at(id).Position = pos; }
	GLvoid SetVertexPositionY(GLuint id, GLfloat y)		{ vertices_.at(id).Position.y = y; }
	GLvoid SetVertexNormal(GLuint id, glm::vec3 normal)	{ vertices_.at(id).Normal = normal; }
	GLvoid SetEdgeLength(GLuint id, GLfloat length)		{ edges_.at(id).Length = length; }
	GLvoid SetMeshReductionProgress(GLfloat progress)	{ progress_mesh_reduction_ = progress; };

	GLboolean	IsMeshReductionQueueLocked()			{ return queue_is_locked_; };
	GLboolean	IsMeshReductionCostQueueEmpty()			{ return queue_costs_.empty(); };

private:
	GLvoid init(std::string name)
	{
		glGenVertexArrays(1, &vao_);
		glGenBuffers(1, &vbo_);
		glGenBuffers(1, &ebo_);

		mesh_name_ = name;
		queue_is_locked_ = false;
		progress_mesh_reduction_ = 0.0f;
	}

	GLvoid sortVertPositions(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2)
	{
		glm::vec3 tmpPos0 = v0, tmpPos1 = v1, tmpPos2 = v2;

		if (((v0.x + v0.z) < (v1.x + v1.z)) && ((v0.x + v0.z) < (v2.x + v2.z)))
		{
			if ((v1.x + v1.z) < (v2.x + v2.z))
			{
				tmpPos0 = v0;
				tmpPos1 = v1;
				tmpPos2 = v2;
			}
			else if ((v2.x + v2.z) < (v1.x + v1.z))
			{
				tmpPos0 = v0;
				tmpPos1 = v2;
				tmpPos2 = v1;
			}
		}
		else if (((v1.x + v1.z) < (v0.x + v0.z)) && ((v1.x + v1.z) < (v2.x + v2.z)))
		{
			if ((v0.x + v0.z) < (v2.x + v2.z))
			{
				tmpPos0 = v1;
				tmpPos1 = v0;
				tmpPos2 = v2;
			}
			else if ((v2.x + v2.z) < (v0.x + v0.z))
			{
				tmpPos0 = v1;
				tmpPos1 = v2;
				tmpPos2 = v0;
			}
		}
		else if (((v2.x + v2.z) < (v0.x + v0.z)) && ((v2.x + v2.z) < (v2.x + v2.z)))
		{
			if ((v0.x + v0.z) < (v1.x + v1.z))
			{
				tmpPos0 = v2;
				tmpPos1 = v0;
				tmpPos2 = v1;
			}
			else if ((v1.x + v1.z) < (v0.x + v0.z))
			{
				tmpPos0 = v2;
				tmpPos1 = v1;
				tmpPos2 = v0;
			}
		}

		v0 = tmpPos0;
		v1 = tmpPos1;
		v2 = tmpPos2;
	}

	glm::vec3 getCenter(GLuint v0ID, GLuint v1ID, GLuint v2ID)
	{
		glm::vec3 c = glm::vec3(0.0f);

		c.x = (vertices_.at(v0ID).Position.x + vertices_.at(v1ID).Position.x + vertices_.at(v2ID).Position.x) / 3.0f;
		c.y = (vertices_.at(v0ID).Position.y + vertices_.at(v1ID).Position.y + vertices_.at(v2ID).Position.y) / 3.0f;
		c.z = (vertices_.at(v0ID).Position.z + vertices_.at(v1ID).Position.z + vertices_.at(v2ID).Position.z) / 3.0f;

		return c;
	}

	glm::vec3 getCenter(GLuint v0ID, GLuint v1ID)
	{
		glm::vec3 c = glm::vec3(0.0f);

		c.x = (vertices_.at(v0ID).Position.x + vertices_.at(v1ID).Position.x) / 2.0f;
		c.y = (vertices_.at(v0ID).Position.y + vertices_.at(v1ID).Position.y) / 2.0f;
		c.z = (vertices_.at(v0ID).Position.z + vertices_.at(v1ID).Position.z) / 2.0f;

		return c;
	}

	GLuint getHash(GLuint v0ID, GLuint v1ID)
	{
		// Szudzik function
		return v0ID >= v1ID ? v0ID * v0ID + v0ID + v1ID : v0ID + v1ID * v1ID;
	}
	/*
	void orderedErase(std::vector<GLuint>& vc, int index)
	{
		if (index < vc.size())
		{
			auto er_it = vc.begin() + index;
			//vc.erase(std::remove(er_it, end(vc), *er_it));
			std::remove(vc.begin(), vc.end(), index);
		}
	}

	void unorderedErase(std::vector<GLuint>& vc, int index)
	{
		if (index < vc.size())
		{
			vc[index] = vc.back();
			vc.pop_back();
			vc.pop_back();
			vc.pop_back();
		}

	}
	*/
	GLboolean vecVecIntersection(glm::vec3 vP0, glm::vec3 vP1, glm::vec3 uP0, glm::vec3 uP1, glm::vec3 &pos)
	{
		GLfloat x1, x2, x3, x4, z1, z2, z3, z4;
		x1 = vP0.x;
		x2 = vP1.x;
		x3 = uP0.x;
		x4 = uP1.x;
		z1 = vP0.z;
		z2 = vP1.z;
		z3 = uP0.z;
		z4 = uP1.z;

		if (((x1 - x2) * (z3 - z4) - (z1 - z2) * (x3 - x4)) == 0.0f)
		{
			true;
		}

		GLfloat Px = ((x1 * z2 - z1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * z4 - z3 * x4)) / ((x1 - x2) * (z3 - z4) - (z1 - z2) * (x3 - x4));
		GLfloat Pz = ((x1 * z2 - z1 * x2) * (z3 - z4) - (z1 - z2) * (x3 * z4 - z3 * x4)) / ((x1 - x2) * (z3 - z4) - (z1 - z2) * (x3 - x4));

		GLfloat lengthV0 = glm::length(glm::vec2(x1, z1) - glm::vec2(x2, z2));
		GLfloat lengthV1 = glm::length(glm::vec2(x3, z3) - glm::vec2(x4, z4));
		GLfloat lengthV0P0AndP = glm::distance(glm::vec2(x1, z1), glm::vec2(Px, Pz));
		GLfloat lengthV0P1AndP = glm::distance(glm::vec2(x2, z2), glm::vec2(Px, Pz));
		GLfloat lengthV1P0AndP = glm::distance(glm::vec2(x3, z3), glm::vec2(Px, Pz));
		GLfloat lengthV1P1AndP = glm::distance(glm::vec2(x4, z4), glm::vec2(Px, Pz));

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
			pos = glm::vec3(Px, 0.0f, Pz);
			return true;
		}

		return false;
	}

	glm::vec3 computeFaceNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
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


	GLuint					vao_;
	GLuint					vbo_;
	GLuint					ebo_;

	std::vector<VertexNew>		vertices_;
	std::vector<GLuint>			indices_;
	std::map<GLuint, EdgeNew>	edges_;
	std::vector<FaceNew>		faces_;
	std::string					mesh_name_;
	std::map<GLuint, GLfloat>	queue_costs_;
	GLboolean					queue_is_locked_;
	GLfloat						progress_mesh_reduction_;
};
#endif