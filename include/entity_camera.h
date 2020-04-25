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
 * @file entity_camera.h
 * @brief This file contains the camera entity class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */

#include <iostream>

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "message_manager.h"
#include "resource_manager.h"


#ifndef CAMERA_H
#define CAMERA_H

/**
 * @brief Camera class represents a camera object.
 */
class Camera
{
public:
    Camera(GLfloat width, GLfloat height)
    {
        width_ = width;
        height_ = height;
        center_ = { width_ / 2, height_ / 2 };

        view_.setSize(width_, height_);
        view_.setCenter(center_.x, center_.y);

        ratio_ = 1.0f;
    }                                            //!< constructor
    ~Camera() { };                                                  //!< destructor

    GLvoid Zoom(GLint zoom)
    {
        ratio_ *= 1.0f + zoom * 0.1f;

        view_.zoom(1.0f + zoom * 0.1f);

        set();
    };

    GLvoid ZoomAt(sf::Vector2i pixel, GLint zoom)
    {
        ratio_ *= 1.0f + zoom * 0.1f;

        sf::RenderTexture* renderTex = ResourceManager::GetRenderTexture("viewport");
        const sf::Vector2f beforeCoord { renderTex->mapPixelToCoords(pixel) };
        view_.zoom(1.0f + zoom * 0.1f);
        set();
        const sf::Vector2f afterCoord { renderTex->mapPixelToCoords(pixel) };
        const sf::Vector2f offsetCoord { beforeCoord - afterCoord };
        view_.move(offsetCoord);
        set();
    }

    GLvoid Center(GLfloat x, GLfloat y)
    {
        view_.setCenter(x, y);
        set();
    }

    GLvoid Resize(GLuint width, GLuint height)
    {
        width_ = width;
        height_ = height;
        sf::RenderTexture* renderTex = ResourceManager::GetRenderTexture("viewport");
        sf::FloatRect visibleArea(0, 0, width_, height_);
        sf::View newView(visibleArea);
        newView.zoom(ratio_);
        renderTex->setView(newView);
        view_ = newView;
    }

    GLvoid Move(GLint x, GLint y)
    {
        sf::Vector2f newPos { (GLfloat)x, (GLfloat)y };
        sf::Vector2f delta = { newPos - center_ };
        delta = -1.0f * delta * ratio_;

        view_.move(delta);
        set();
        center_ = newPos;
    }

    GLvoid SetMousePos(GLint x, GLint y)
    {
        std::stringstream msg;
        msg << "x: " << x << "\ty: " << y;
        MessageManager::AddMessage(msg, message_t::INFO);
        sf::Vector2f newPos{ (GLfloat)x, (GLfloat)y };
        center_ = newPos;
    }

    sf::View* GetView() { return &view_; }

private:
    GLvoid set()
    {
        sf::RenderTexture* renderTex = ResourceManager::GetRenderTexture("viewport");
        renderTex->setView(view_);
    }

    sf::View										view_;
    GLfloat                                         width_;
    GLfloat                                         height_;
    GLfloat                                         ratio_;
    sf::Vector2f                                    center_;

};
#endif
