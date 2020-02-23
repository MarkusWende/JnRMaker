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


#ifndef CAMERA_H
#define CAMERA_H

#include "scene_entity.h"

/**
 * @brief Camera class represents a camera object.
 */
class Camera : public SceneEntity
{
public:
    Camera() { init(); }                                            //!< constructor
    ~Camera() { };                                                  //!< destructor

private:
	GLvoid init() { };

};
#endif
