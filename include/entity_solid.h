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
 * @file entity_solid.h
 * @brief This file contains the solid entity class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#ifndef SOLID_H
#define SOLID_H

#include "scene_entity.h"
#include "solid_model.h"

 /**
  * @brief Enumeration for layer types.
  */
enum class layer_t {
    FORE,                            /**< Background layer. */
    PLAYER,                          /**< player ground layer. */
    BACK,                            /**< foreground layer. */
};

/**
 * @brief Solid class represents a solid object. Solids are visible object in the rendered scene.
 */
class Solid : public SceneEntity
{
public:
	Solid();
	Solid(GLuint id, std::string name, layer_t layer);                            //!< constructor

    ~Solid() { };                                               //!< destructor

	/**
	 * @brief Draw the vertex array object.
	 * @return Void.
	 */
	virtual GLvoid Draw();

	/**
	 * @brief Draw the vertex array object in wireframes.
	 * @return Void.
	 */
	GLvoid DrawWireframe();

	/**
	 * @brief Calculates the intersection point with this object and a ray.
	 * @param orig The origin of the ray.
	 * @param dir The direction of the ray.
	 * @return Bool Return true if an intersection with the object exists.
	 */
	GLboolean Intersection(glm::vec3 orig, glm::vec3 dir, glm::vec3& pos);

	/**
	 * @brief Calculates the intersection curve with this object and a plane which is orthogonal to the xz-plane. The start and end input points define the location of the plane in the xz-plane.
	 * @param start The start of the vector which defines the plane.
	 * @param end The end of the vector which defines the plane.
	 * @return Bool Return true if an intersection with the object exists.
	 */
	GLboolean IntersectionCurve(glm::vec3 start, glm::vec3 end, std::vector<glm::vec3>& intersectCurve);

	GLvoid		Scale(glm::vec3 newScale) { scale_ = newScale; };												/**< @brief Scale the solid. @param newScale The new scale factor as a glm::vec3, to scale all 3 dimensions individually @return Void. */
	GLvoid		Scale(GLfloat newScale) { scale_.x = newScale; scale_.y = newScale; scale_.z = newScale; };	/**< @brief Scale the solid. @param newScale The new scale factor as a float, to scale all 3 dimensions equally @return Void. */
	GLvoid		Show(GLboolean status) { show_ = status; };													/**< @brief Show/Hide the solid. @param status If true, the solid is shown and if false the solid is hidden. @return Void. */
	GLvoid		Yaw(GLfloat yaw) { yaw_ = yaw; };														/**< @brief Set the yaw of the solid. @param yaw The yaw agnle in degree. @return Void. */
	GLvoid		Pitch(GLfloat pitch) { pitch_ = pitch; };													/**< @brief Set the pitch of the solid. @param pitch The pitch agnle in degree. @return Void. */
	GLvoid		Roll(GLfloat roll) { roll_ = roll; };													/**< @brief Set the roll of the solid. @param roll The roll agnle in degree. @return Void. */

	GLvoid		SetColor(glm::vec3 color) { color_ = color; };													/**< @brief Set the color (RGB) of the solid. @param color A 3d vector. @return Void. */
	GLvoid		SetColorWireframe(glm::vec3 color) { color_wireframe_ = color; };										/**< @brief Set the color (RGB) of the wireframe of the solid. @param . @return Void. */
	GLvoid		SetAlpha(GLfloat alpha) { alpha_ = alpha; };													/**< @brief Set the alpha of the solid. @param alpha The alpha as a float. @return Void. */
	GLvoid		SetAlphaWireframe(GLfloat alpha) { alpha_wireframe_ = alpha; };										/**< @brief Set the alpha of the wireframe of the solid. @param The alpha as a float. @return Void. */
	GLvoid		SetName(std::string name) { name_ = name; };													/**< @brief Set the name of the solid. @param name A string. @return Void. */
	GLvoid		SetSmooth(GLboolean smooth) { smooth_ = smooth; }													/**< @brief Set the shading of the solid. @param smooth The smooth switch, set the shading to smooth, when smooth=true or flat , when smooth=false. @return Void. */

	GLboolean	IsShown() { return show_; };													/**< @brief Get the status if the solid is shown or hidden. @return GLboolean Return true is the solid is shown and false if the slid is hidden. */
	glm::vec3	GetScale() { return scale_; };													/**< @brief Get the scale of the solid as a 3d vector. @return glm::vec3 A 3d vector. */
	glm::vec3	GetColor() { return color_; };													/**< @brief Get the RGB values of the solid as a 3d vector. @return glm::vec3 A 3d vector. */
	glm::vec3	GetColorWireframe() { return color_wireframe_; };											/**< @brief Get the RGB values of the wireframe of the solid as a 3d vector. @return glm::vec3 A 3d vector. */
	GLfloat		GetAlpha() { return alpha_; };													/**< @brief Get the alpha value of the solid. @return GLfloat A float value. */
	GLfloat		GetAlphaWireframe() { return alpha_wireframe_; };											/**< @brief Get the alpha value of the wireframe of the solid. @return GLfloat A float value. */
	std::string GetName() { return name_; };													/**< @brief Get the name of the solid. @return std::string Return the name as a string. */
	GLfloat		GetYaw() { return yaw_; };														/**< @brief Get the yaw angle of the solid. @return GLflaot Return the angle as a float. */
	GLfloat		GetRoll() { return roll_; };													/**< @brief Get the roll angle of the solid. @return GLflaot Return the angle as a float. */
	GLfloat		GetPitch() { return pitch_; };													/**< @brief Get the pitch angle of the solid. @return GLflaot Return the angle as a float. */
	GLuint		GetVAOId() { return model_.GetVAOId(name_); };
	std::vector<VertexNew>	GetModelVertices() { return model_.GetVertices(name_); }
	std::vector<GLuint>	GetModelIndices() { return model_.GetIndices(name_); }
	GLuint		GetId() { return id_; };

protected:
	GLvoid addMesh(std::string meshName);

	/**
	 * @brief Add a vertex to the vertex vector.
	 * @param orig Todo.
	 * @param orig Todo.
	 * @return Void.
	 */
	GLvoid addVertex(std::string meshName, VertexNew v);

	/**
	 * @brief Add a vertex to the vertex vector.
	 * @param orig Todo.
	 * @param orig Todo.
	 * @return Void.
	 */
	GLvoid addEdge(std::string meshName, EdgeNew e);

	/**
	 * @brief Add a vertex to the vertex vector.
	 * @param orig Todo.
	 * @param orig Todo.
	 * @return Void.
	 */
	GLvoid addFace(std::string meshName, FaceNew f);

	/**
	 * @brief Add three vertex indices to the index vector. Used for triangles
	 * @param i1 Vertex id of the first vertex.
	 * @param i2 Vertex id of the second vertex.
	 * @param i3 Vertex id of the third vertex.
	 * @return Void.
	 */
	GLvoid addIndices(std::string meshName, GLuint i1, GLuint i2, GLuint i3);

	/**
	 * @brief Add two vertex indices to the index vector. Used for vectors (lines).
	 * @param i1 Vertex id of the first vertex.
	 * @param i2 Vertex id of the second vertex.
	 * @return Void.
	 */
	GLvoid addIndices(std::string meshName, GLuint i1, GLuint i2);

	/**
	 * @brief Add one vertex index to the index vector.
	 * @param i1 Vertex id of the first vertex.
	 * @return Void.
	 */
	GLvoid addIndex(std::string meshName, GLuint i1);

	/**
	 * @brief Calculate the face normal, which is defined by three vertices.
	 * @param v1 First vertex.
	 * @param v2 Second vertex.
	 * @param v3 Third vertex.
	 * @return glm::vec3 A 3d vector [x,y,z].
	 */
	glm::vec3 computeFaceNormal(VertexNew v1, VertexNew v2, VertexNew v3);

	/**
	 * @brief Calculate the face normal, which is defined by three positions.
	 * @param v1 Position of the first vertex.
	 * @param v2 Position of the second vertex.
	 * @param v3 Position of the third vertex.
	 * @return glm::vec3 A 3d vector [x,y,z].
	 */
	glm::vec3 computeFaceNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

	/**
	 * @brief Calculate the center of a triangle.
	 * @param v1 Position of the first vertex.
	 * @param v2 Position of the second vertex.
	 * @param v3 Position of the third vertex.
	 * @return glm::vec3 A 3d vector [x,y,z].
	 */
	glm::vec3 computeTriangleCenter(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

	/**
	 * @brief Calculate the intersection of two vectors. Return true if the vectors are intersecting within its length. Only x and z values of the vectors are taken into account.
	 * @param vP0 Position of the first vertex.
	 * @param vP1 Position of the second vertex.
	 * @param uP0 Position of the third vertex.
	 * @param uP1 Position of the third vertex.
	 * @return GLboolean Return true if the vectors are intersecting.
	 */
	GLboolean vecVecIntersection(glm::vec3 vP0, glm::vec3 vP1, glm::vec3 uP0, glm::vec3 uP1);

	GLvoid updateNormals(std::string meshName);

	/**
	 * @brief Clear the mesh (vertices, indices, faces and edges) of the solid. The method is forwarded to the model_ attribute.
	 * @return Void.
	 */
	GLvoid clearMesh(std::string meshName);

	/**
	 * @brief Triangulate the mesh of the solid. This generates faces, edges and calculates the vertex normals. The method is forwarded to the model_ attribute.
	 * @return Void.
	 */
	GLvoid triangulateMesh(std::string meshName);

	/**
	 * @brief Try to reduce the number of faces and edges of the mesh. The method is forwarded to the model_ attribute.
	 * @return Void.
	 */
	GLvoid reduceMesh(std::string meshName);

	/**
	 * @brief Copy the reduced mesh to work on a copy of the mesh. The method is forwarded to the model_ attribute.
	 * @return Void.
	 */
	GLvoid copyReducedMesh(std::string meshName);

	/**
	 * @brief Update the vertex array object. Use this every time you change the vertices. The method is forwarded to the model_ attribute.
	 * @return Void.
	 */
	GLvoid updateVAO(std::string meshName);

	GLuint		getVAOId(std::string meshName) { return model_.GetVAOId(meshName); };					/**< @brief Get the id of the vao. @param meshName The name of the mesh. @return GLuint The id as an unsigned integer. */
	glm::vec3	getVertexPosition(std::string meshName, GLuint id) { return model_.GetVertexPosition(meshName, id); };		/**< @brief Get the vertex coordinates by id. @param meshName The name of the mesh. @param id The id of the vertex as a GLuint. @return glm::vec3 The coordinate of the vertex as a 3d vector. */
	glm::vec3	getVertexNormal(std::string meshName, GLuint id) { return model_.GetVertexNormal(meshName, id); };		/**< @brief Get the vertex normal by id. @param meshName The name of the mesh. @param id The id of the vertex as a GLuint. @return glm::vec3 The nromal of the vertex as a 3d vector. */
	EdgeNew		getEdge(std::string meshName, GLuint id) { return model_.GetEdge(meshName, id); };				/**< @brief Get the edge of the mesh by id. @param meshName The name of the mesh. @param id The id of the edge as a GLuint. @return GLuint The id as an unsigned integer. */
	GLuint		getNumberOfEdges(std::string meshName) { return model_.GetNumberOfEdges(meshName); };			/**< @brief Get the number of edges in the mesh. @param meshName The name of the mesh. @return GLuint The number of edges as an unsigned integer. */
	GLfloat		getMeshReductionProgress(std::string meshName) { return model_.GetMeshReductionProgress(meshName); }	/**< @brief Get the progress of the mesh reduction as a float. @param meshName The name of the mesh. @return GLfloat The mesh reduction progress as a float between 0.0f and 1.0f. */
	GLboolean	isMeshReductionQueueLocked() { return model_.IsMeshReductionQueueLocked(); };		/**< @brief Return the mesh reduction queue status. @return GLboolean Return true if the mesh reduction queue is locked. */
	GLboolean	isMeshReductionCostQueueEmpty() { return model_.IsMeshReductionCostQueueEmpty(); };		/**< @brief Return true if the mesh reduction queue empty. @return GLboolean Return true if the mesh reduction queue is empty. */

	GLvoid		setVertexPosition(std::string meshName, GLuint id, glm::vec3 pos) { model_.SetVertexPosition(meshName, id, pos); };		/**< @brief Set the 3d coordinate of a vertex, which is adressed by its id. @param meshName The name of the mesh. @param id The id of the vertex. @param pos The 3d coordinate of the new position. @return GLvoid. */
	GLvoid		setVertexPositionY(std::string meshName, GLuint id, GLfloat y) { model_.SetVertexPositionY(meshName, id, y); };		/**< @brief Set the y coordinate of a vertex, which is adressed by its id. @param meshName The name of the mesh. @param id The id of the vertex. @param y The y coordinate of the new position. @return GLvoid. */
	GLvoid		setVertexNormal(std::string meshName, GLuint id, glm::vec3 normal) { model_.SetVertexNormal(meshName, id, normal); };		/**< @brief Set the normal of a vertex, which is adressed by its id. @param meshName The name of the mesh. @param id The id of the vertex. @param normal The 3d normal vector. @return GLvoid. */
	GLvoid		setEdgeLength(std::string meshName, GLuint id, GLfloat length) { model_.SetEdgeLength(meshName, id, length); };		/**< @brief Set the length of an edge, adressed by its id. @param meshName The name of the mesh. @param id The id of the edge. @param length The length of the edge. @return GLvoid. */


    layer_t                 layer_;                             /**< foreground layer. */
    std::string             name_;                              /**< @brief The name of the solid. */
    GLuint                  id_;
    glm::vec3	            scale_;								/**< @brief The scale factor, a 3d vector, of the solid. */
    glm::vec3	            color_;								/**< @brief The color (RGB), a 3d vector, of the solid. */
    glm::vec3	            color_wireframe_;					/**< @brief The wirframe color (RGB), a 3d vector, of the solid. */
    GLfloat		            alpha_;								/**< @brief The alpha, a float, of the solid. */
    GLfloat		            alpha_wireframe_;					/**< @brief The wireframe alpha, a float, of the solid. */
    GLboolean	            show_;								/**< @brief Flag, if the solid is shown or hidden. */
    GLboolean	            smooth_;							/**< @brief Flag, if the solid is build with flat (false) or smooth (true) shading. */

    GLfloat yaw_;
    GLfloat pitch_;
    GLfloat roll_;

	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version)
	{
		//ar(cereal::base_class<SceneEntity>(this));
		//ar(CEREAL_NVP(layer_));
		ar(CEREAL_NVP(name_));
		ar(CEREAL_NVP(id_));
	}

private:
    ModelNew	            model_;								/**< @brief Every vertex, index, face etc. is stored in the model attribute. */
};
CEREAL_CLASS_VERSION(Solid, 1);

#endif
