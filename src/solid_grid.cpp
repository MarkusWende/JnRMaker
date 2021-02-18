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


#include "../include/solid_grid.h"


 // PUBLIC:
 /////////////////////////////////////////////////////////////////////////////////////////////////////////

Grid::Grid(std::string meshName, GLboolean smooth, GLuint width, GLuint height)
{
	addMesh(meshName);
	name_ = meshName;
	smooth_ = smooth;
	layer_ = layer_t::FORE;
	scale_ = glm::vec3(1.0f, 1.0f, 1.0f);
	line_width_ = 0.5f;
	height_ = height;
	width_ = width;

	build();
}

GLvoid Grid::Draw(glm::mat4 projection, glm::mat4 view)
{
	ResourceManager::GetShader("line").Use();
	ResourceManager::GetShader("line").SetMatrix4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	ResourceManager::GetShader("line").SetMatrix4("view", view);
	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, center_);
	model = glm::rotate(model, glm::radians(roll_), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(yaw_), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(pitch_), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale_);
	ResourceManager::GetShader("line").SetMatrix4("model", model);
	ResourceManager::GetShader("line").SetVector2f("u_resolution", glm::vec2(1, 1));
	// positive axis
	ResourceManager::GetShader("line").SetFloat("u_dashSize", 10.0f);
	ResourceManager::GetShader("line").SetFloat("u_gapSize", 0.0f);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(line_width_);
	for (size_t i = 0; i <= height_; i++)
	{
		std::stringstream key;
		key << "axis_h_" << i;
		ResourceManager::GetShader("line").SetVector4f("color", { axis_h_[key.str()]->GetColor(), axis_h_[key.str()]->GetAlpha() });
		axis_h_[key.str()]->Draw();
	}

	for (size_t i = 0; i <= width_; i++)
	{
		std::stringstream key;
		key << "axis_v_" << i;
		ResourceManager::GetShader("line").SetVector4f("color", { axis_v_[key.str()]->GetColor(), axis_v_[key.str()]->GetAlpha() });
		axis_v_[key.str()]->Draw();
	}
	glLineWidth(1.0f);
	glDisable(GL_LINE_SMOOTH);
}

GLvoid Grid::build()
{
	for (size_t i = 0; i <= height_; i++)
	{
		std::stringstream key;
		key << "axis_h_" << i;
		axis_h_.insert(std::make_pair(key.str(), new Line(key.str(), glm::vec3(0.0f, (float)i, 0.0f) * scale_, glm::vec3((float)width_, (float)i, 0.0f) * scale_)));
		axis_h_[key.str()]->SetColor({1.0f, 1.0f, 1.0f});
		axis_h_[key.str()]->SetAlpha(1.0f);
	}
	
	for (size_t i = 0; i <= width_; i++)
	{
		std::stringstream key;
		key << "axis_v_" << i;
		axis_v_.insert(std::make_pair(key.str(), new Line(key.str(), glm::vec3((float)i, 0.0f, 0.0f) * scale_, glm::vec3((float)i, (float)height_, 0.0f) * scale_)));
		axis_v_[key.str()]->SetColor({ 1.0f, 1.0f, 1.0f });
		axis_v_[key.str()]->SetAlpha(1.0f);
	}
}