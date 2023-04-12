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
 * @file solid_model.h
 * @brief This file contains the solid sprite class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#ifndef MODEL_NEW_H
#define MODEL_NEW_H

#include <map>
#include <memory>
//#include <ppl.h>

#include "solid_mesh.h"

class ModelNew
{
public:
	ModelNew()
	{
		mesh_copy_ptr_ = new MeshNew("Empty");;
		mesh_reduction_is_active_ = false;
	}

	~ModelNew()
	{
//		mesh_reduction_task.wait();
//		mesh_reduction_task.~task_group();
	}


	GLvoid Draw()
	{
		for (const auto& it : meshes_)
			it.second->Draw();
	}

	GLvoid DrawWireframe()
	{
		for (const auto& it : meshes_)
			it.second->DrawWireframe();
	}

	GLboolean Intersection(glm::vec3 orig, glm::vec3 dir, glm::vec3& pos)
	{
		for (const auto& it : meshes_)
		{
			if (it.second->Intersection(orig, dir, pos))
				return true;
		}
		return false;
	}

	GLboolean IntersectionCurve(glm::vec3 start, glm::vec3 end, std::vector<glm::vec3>& intersectCurve)
	{
		GLboolean status = false;
		for (const auto& it : meshes_)
		{
			if (it.second->IntersectionCurve(start, end, intersectCurve))
				status = true;
		}

		return status;
	}
	GLvoid AddMesh(std::string meshName)
	{
		meshes_.insert(std::make_pair(meshName, new MeshNew(meshName)));
	}

	GLvoid AddVertex(std::string meshName, VertexNew v)
	{
		meshes_[meshName]->AddVertex(v);
	}

	GLvoid AddEdge(std::string meshName, EdgeNew e)
	{
		meshes_[meshName]->AddEdge(e);
	}

	GLvoid AddFace(std::string meshName, FaceNew f)
	{
		meshes_[meshName]->AddFace(f);
	}

	GLvoid AddIndices(std::string meshName, GLuint i1, GLuint i2, GLuint i3)
	{
		meshes_[meshName]->AddIndex(i1);
		meshes_[meshName]->AddIndex(i2);
		meshes_[meshName]->AddIndex(i3);
	}

	GLvoid AddIndices(std::string meshName, GLuint i1, GLuint i2)
	{
		meshes_[meshName]->AddIndex(i1);
		meshes_[meshName]->AddIndex(i2);
	}

	GLvoid AddIndex(std::string meshName, GLuint i1)
	{
		meshes_[meshName]->AddIndex(i1);
	}

	GLvoid ClearMesh(std::string meshName)
	{
		meshes_[meshName]->ClearMesh();
	}

	GLvoid TriangulateMesh(std::string meshName)
	{
		meshes_[meshName]->Triangulate();
	}
/*
	GLvoid UpdateNormals(std::string meshName) {
		meshes_[meshName]->updateNormals();
	}

	GLvoid ReduceMesh(std::string meshName)
	{
		mesh_copy_ptr_ = new MeshNew(meshName);
		memcpy(mesh_copy_ptr_, meshes_.at(meshName).get(), sizeof(MeshNew));

		if (!mesh_reduction_is_active_)
		{

			mesh_reduction_task.run([this]()
			{
				mesh_reduction_is_active_ = true;
				mesh_copy_ptr_->SetMeshReductionProgress(0.0f);

				mesh_copy_ptr_->Reduce();

				mesh_reduction_is_active_ = false;
			});
		}
	}

	GLvoid CopyReducedMesh(std::string meshName)
	{
		meshes_.insert(std::make_pair(meshName, new MeshNew(meshName)));
		memcpy(meshes_.at(meshName).get(), mesh_copy_ptr_, sizeof(MeshNew));
	}
*/
	GLvoid UpdateVAO(std::string meshName)
	{
		meshes_[meshName]->UpdateVAO();
	}

	GLuint		GetVAOId(std::string meshName)						{ return meshes_[meshName]->GetVAOId(); }
	glm::vec3	GetVertexPosition(std::string meshName, GLuint id)	{ return meshes_[meshName]->GetVertexPosition(id); }
	glm::vec3	GetVertexNormal(std::string meshName, GLuint id)	{ return meshes_[meshName]->GetVertexNormal(id); }
	std::vector<VertexNew>	GetVertices(std::string meshName)		{ return meshes_[meshName]->GetVertices(); }
	std::vector<GLuint>	GetIndices(std::string meshName)			{ return meshes_[meshName]->GetIndices(); }
	EdgeNew		GetEdge(std::string meshName, GLuint id)			{ return meshes_[meshName]->GetEdge(id); }
	GLuint		GetNumberOfEdges(std::string meshName)				{ return meshes_[meshName]->GetNumberOfEdges(); }
	GLfloat		GetMeshReductionProgress()							{ return mesh_copy_ptr_->GetMeshReductionProgress(); }
	MeshNew		GetMesh(std::string meshName) { return *meshes_[meshName]; }
	
	GLvoid SetVertexPosition(std::string meshName, GLuint id, glm::vec3 pos)	{ return meshes_[meshName]->SetVertexPosition(id, pos); }
	GLvoid SetVertexPositionY(std::string meshName, GLuint id, GLfloat y)		{ return meshes_[meshName]->SetVertexPositionY(id, y); }
	GLvoid SetVertexNormal(std::string meshName, GLuint id, glm::vec3 normal)	{ return meshes_[meshName]->SetVertexNormal(id, normal); }
	GLvoid SetEdgeLength(std::string meshName, GLuint id, GLfloat length)		{ return meshes_[meshName]->SetEdgeLength(id, length); }

	GLboolean	IsMeshReductionQueueLocked()						{ return mesh_copy_ptr_->IsMeshReductionQueueLocked(); };
	GLboolean	IsMeshReductionCostQueueEmpty()						{ return mesh_copy_ptr_->IsMeshReductionCostQueueEmpty(); };

protected:
	//concurrency::task_group mesh_reduction_task;

private:
	std::map<std::string, std::unique_ptr<MeshNew>>		meshes_;
	MeshNew*											mesh_copy_ptr_;
	GLboolean											mesh_reduction_is_active_;

};

#endif
