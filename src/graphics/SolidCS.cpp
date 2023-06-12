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
 * @file solid_cs.cpp
 * @brief
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */

#include "SolidCS.h"
#include "ResourceManagerOld.h"


  // PUBLIC:
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

CoordinateSystem::CoordinateSystem(std::shared_ptr<ILogger> logger, std::shared_ptr<Resources> resources, glm::vec3 offset) : Solid(logger, resources)
{
	center_ = offset;
	layer_ = layer_t::FORE;

	axis_.insert(std::make_pair("X",  new Line(logger, resources, "X",  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f))));
	axis_.insert(std::make_pair("Y",  new Line(logger, resources, "Y",  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f))));
	axis_.insert(std::make_pair("Z",  new Line(logger, resources, "Z",  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f))));
	//axis_.insert(std::make_pair("nX", new Line("nX", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f))));
	//axis_.insert(std::make_pair("nY", new Line("nY", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f))));
	//axis_.insert(std::make_pair("nZ", new Line("nZ", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f))));

	resolution_ = glm::vec2(2000, 2000);
	scale_ = glm::vec3(1.0f, 1.0f, 1.0f) * 1.5f;

	axis_["X"]->SetColor({ 1.0f, 0.0f, 0.0f });
	axis_["X"]->SetAlpha(1.0f);
	axis_["Y"]->SetColor({ 0.0f, 1.0f, 0.0f });
	axis_["Y"]->SetAlpha(1.0f);
	axis_["Z"]->SetColor({ 0.0f, 0.0f, 1.0f });
	axis_["Z"]->SetAlpha(1.0f);
	/*
	axis_["nX"]->SetColor({ 1.0f, 0.0f, 0.0f });
	axis_["nX"]->SetAlpha(1.0f);
	axis_["nY"]->SetColor({ 0.0f, 1.0f, 0.0f });
	axis_["nY"]->SetAlpha(1.0f);
	axis_["nZ"]->SetColor({ 0.0f, 0.0f, 1.0f });
	axis_["nZ"]->SetAlpha(1.0f);
	*/
}

GLvoid CoordinateSystem::Draw(glm::mat4 projection, glm::mat4 view)
{
	resources_->GetShader("line")->Use();
	resources_->GetShader("line")->SetMatrix4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	resources_->GetShader("line")->SetMatrix4("view", view);
	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, center_);
	model = glm::rotate(model, roll_, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, yaw_, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, pitch_, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale_);
	resources_->GetShader("line")->SetMatrix4("model", model);
	resources_->GetShader("line")->SetVector2f("u_resolution", glm::vec2(resolution_.x, resolution_.y));
	// positive axis
	resources_->GetShader("line")->SetFloat("u_dashSize", 10.0f);
	resources_->GetShader("line")->SetFloat("u_gapSize", 0.0f);
	glLineWidth(2.0f);
	resources_->GetShader("line")->SetVector4f("color", { axis_["X"]->GetColor(), axis_["X"]->GetAlpha() });
	axis_["X"]->Draw();
	resources_->GetShader("line")->SetVector4f("color", { axis_["Y"]->GetColor(), axis_["Y"]->GetAlpha() });
	axis_["Y"]->Draw();
	resources_->GetShader("line")->SetVector4f("color", { axis_["Z"]->GetColor(), axis_["Z"]->GetAlpha() });
	axis_["Z"]->Draw();
	// negative axis
	/*
	resources_->GetShader("line")->SetFloat("u_dashSize", 10.0f);
	resources_->GetShader("line")->SetFloat("u_gapSize", 10.0f);
	resources_->GetShader("line")->SetVector4f("color", { axis_["nX"]->GetColor(), axis_["nX"]->GetAlpha() });
	axis_["nX"]->Draw();
	resources_->GetShader("line")->SetVector4f("color", { axis_["nY"]->GetColor(), axis_["nY"]->GetAlpha() });
	axis_["nY"]->Draw();
	resources_->GetShader("line")->SetVector4f("color", { axis_["nZ"]->GetColor(), axis_["nZ"]->GetAlpha() });
	axis_["nZ"]->Draw();
	*/
	glLineWidth(1.0f);
}