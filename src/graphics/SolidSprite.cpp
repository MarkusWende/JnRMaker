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
 * @file solid_sprite.cpp
 * @brief
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#include "SolidSprite.h"


 // PUBLIC:
 /////////////////////////////////////////////////////////////////////////////////////////////////////////

Sprite::Sprite(std::shared_ptr<ILogger> logger, std::shared_ptr<Resources> resources, std::string meshName, GLboolean smooth, GLuint width, GLuint height) : Solid(logger, resources)
{
    (void)smooth;
    name_ = meshName;
    center_ = glm::vec3(0.5f, 0.5f, 0.0f);
    width_ = width;
    height_ = height;
    layer_ = layer_t::FORE;

    std::stringstream key;
    key << meshName << "_Quad" << scale_.x;
    quad_ = std::unique_ptr<Quad>(new Quad(logger, resources, meshName.c_str()));
}


GLvoid Sprite::Draw(glm::mat4 projection, glm::mat4 view)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, center_);
    model = glm::scale(model, scale_);
    resources_->GetShader("sprite")->Use();
    resources_->GetShader("sprite")->SetMatrix4("projection", projection);
    resources_->GetShader("sprite")->SetMatrix4("view", view);
    resources_->GetShader("sprite")->SetMatrix4("model", model);
    resources_->GetShader("sprite")->SetVector3f("dirLight.direction", 5.0f, 5.0f, 20.0f);
    resources_->GetShader("sprite")->SetVector3f("dirLight.ambient", 0.5f, 0.5f, 0.5f);
    resources_->GetShader("sprite")->SetVector3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    resources_->GetShader("sprite")->SetVector3f("dirLight.specular", 0.5f, 0.5f, 0.5f);
    resources_->GetShader("sprite")->SetFloat("material.shininess", 640.0f);
    resources_->GetShader("sprite")->SetVector3f("color", 1.0f, 1.0f, 1.0f);
    resources_->GetShader("sprite")->SetFloat("alpha", 1.0f);
    resources_->GetShader("sprite")->SetInteger("material.diffuse", 0);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    quad_->Draw();
}