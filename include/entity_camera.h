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

#ifdef __EMSCRIPTEN__
#define GLM_FORCE_PURE
#else

#endif
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/intersect.hpp>

//#include <SFML/Graphics/View.hpp>
//#include <SFML/Graphics/RenderTexture.hpp>

#include "message_manager.h"
#include "resource_manager.h"
#include "scene_entity.h"
#include "primitive_cube.h"


#ifndef CAMERA_H
#define CAMERA_H

 /**
  * @brief Defines if the camera projection is orthographic or perspective.
  */
enum class CameraState {
    ORTHOGRAPHIC,
    PERSPECTIVE,
	UNDEFINED
};


// Default camera parameters.
const float YAW = 180.0f;							/**< Default camera yaw. */
const float PITCH = 0.0f;							/**< Default camera pitch. */
const float ROLL = 0.0f;								/**< Default camera roll. */
const float SCROLL_SPEED = 3.0f;								/**< Default camera scroll (zoom in and out) speed. */
const float DRAG_SPEED = 0.16f;							/**< Default camera drag speed. */
const float STRAFE_SPEED = 0.1f;								/**< Default camera strafe speed. */
const float ROTATE_SPEED = 14.0f;							/**< Default sensitivity to rotate the camera. */
const float ZOOM = 40.0f;							/**< Default zoom level. */
const float ZOOM_MAX = 45.0f;							/**< Maximal zoom level. Should not be higher then 45.0f. */
const float ZOOM_MIN = 1.0f;								/**< Minimal zoom level. Should not be lower then 1.0f. */


/**
 * @brief An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL.
 */
class Camera : public SceneEntity
{
public:
	/**
	 * @brief Constructor for Camera.
	 */
	Camera(glm::vec3 offset = { 0.0f, 0.0f, 0.0f })// : SceneEntity()
	{
		center_ = offset;
		entity_type_ = EntityType::CAMERA_ENTITY;
		focus_model_ = std::unique_ptr<Cube>(new Cube("CameraFocusCube"));
		focus_model_->Scale(glm::vec3(0.1f));
		init();
	}

	/**
	 * @brief Destructor for Camera.
	 */
	~Camera()
	{
	}

	// Getters
	glm::mat4	GetViewMatrix() { return glm::lookAt(position_, position_ + front_, up_); }		/**< @brief Get the view matrix. @return A 4x4 float matrix. */
	CameraState GetState() { return state_; }													/**< @brief Get the state of the camera. @return The camera state {ORTHOGRAPHIC, PERSPECTIVE}. */
	glm::vec3	GetPosition() { return position_; }												/**< @brief Get the camera position. @return A 3 element float vector. */
	glm::vec3	GetCenter() { return center_; }													/**< @brief Get the camera center. The Sposition the camera is pointing at. @return A 3 element float vector. */
	float		GetZoom() { return zoom_; }														/**< @brief Get the camera zoom. @return A float value. */
	float		GetYaw() { return yaw_; }														/**< @brief Get the camera yaw. The angle ranges from -180 to 180 degree. @return A float value. */
	float		GetPitch() { return pitch_; }													/**< @brief Get the camera pitch. The angle ranges from -90 to 90 degree. @return A float value. */
	float		GetRoll() { return roll_; }														/**< @brief Get the camera roll. The angle ranges from -180 to 180 degree. @return A float value. */
	float		GetDistance() { return distance_; }												/**< @brief Get the distance between the carera center and camera position. @return A float value. */

	// Setters
	GLvoid		SetSceneWidth(GLfloat width_in_percent) {scene_width_percent_ = width_in_percent;};
	GLvoid		SetSceneHeight(GLfloat height_in_percent) {scene_height_percent_ = height_in_percent;};

	/**
	 * @brief Sets the camera state to the given parameter and reinitialize the camera with the default state settings.
	 * @param newState The new camera state {ORTHOGRAPHIC or PERSPECTIVE}.
	 * @return Void
	 */
	void SetState(CameraState newState)
	{
		if (newState != state_)
		{

			// Default yaw and pitch values
			yaw_ = YAW;
			pitch_ = PITCH;
			roll_ = ROLL;

			// Default camera distance
			distance_ = 80.0;

			// Get the default camera position
			position_.x = center_.x + distance_ * glm::sin(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
			position_.z = center_.z + distance_ * glm::cos(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
			position_.y = center_.y + distance_ * glm::sin(glm::radians(pitch_));

			//position_ = glm::vec3(0.0f, 0.0f, 4.0f);

			// Update the front, right and up camera vectors
			// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			front_ = -glm::normalize(position_ - center_);
			right_ = glm::normalize(glm::cross(-world_up_, front_));
			up_ = glm::normalize(glm::cross(front_, right_));
			//front_ = glm::vec3(0.0f, 0.0f, -1.0f);
			//right_ = glm::vec3(1.0f, 0.0f, 0.0f);
			//up_ = glm::vec3(0.0f, -1.0f, 0.0f);

			// Default zoom and state
			zoom_ = ZOOM;

			// Set state
			state_ = newState;
		}
	}

	void Reset()
	{
		init();
	}


	// General member functions
	/**
	 * @brief Processes input received from any keyboard-like input system to move the camera.
	 * @param direction The direction, the camera will be moved to. See the enum class CameraMovement.
	 * @param deltaTime This parameter hold the time the hardware needs for one main loop. This prevents faster system to move the camera faster.
	 * @sa CameraMovement
	 * @return Void.
	 */
	void Move(GLfloat xOff, GLfloat yOff)
	{
		position_.x = xOff;
		position_.y = yOff;

		center_.x = xOff;
		center_.y = yOff;
		// The velocity of moving the camera depends on the zoom level and the time the hardware needs for one main loop
		//float velocity = STRAFE_SPEED * deltaTime * zoom_;

		// Update the Position depending on the direction the user move the camera
		/*
		if (direction == FORWARD)
			position_ += front_ * velocity;
		if (direction == BACKWARD)
			position_ -= front_ * velocity;
		if (direction == LEFT)
			position_ += right_ * velocity;
		if (direction == RIGHT)
			position_ -= right_ * velocity;
		if (direction == UP)
			position_ -= up_ * velocity;
		if (direction == DOWN)
			position_ += up_ * velocity;
		*/

		// keep camera position inside a cube, to prevent moving outside the FOV
		/*
		GLfloat cubeSize = 4.0f;
		if (position_.x > cubeSize)
			position_.x = cubeSize;
		else if (position_.x < -cubeSize)
			position_.x = -cubeSize;

		if (position_.y > cubeSize)
			position_.y = cubeSize;
		else if (position_.y < -cubeSize)
			position_.y = -cubeSize;

		if (position_.z > cubeSize)
			position_.z = cubeSize;
		else if (position_.z < -cubeSize)
			position_.z = -cubeSize;
		*/

		// Update the center of the camera
		//updateCameraCenter();
	}

	/**
	 * @brief Processes input received from a mouse input system to drag the camera.
	 * @param xoffset Mouse offset on the x-axis.
	 * @param yoffset Mouse offset on the y-axis.
	 * @param deltaTime This parameter hold the time the hardware needs for one main loop. This prevents faster system to move the camera faster.
	 * @return Void.
	 */
	void ProcessMouseDrag(float xoffset, float yoffset, float deltaTime)
	{
		// The velocity of dragging the camera depends on the zoom level and the time the hardware needs for one main loop
		if ((glm::abs(xoffset) < 400) && (glm::abs(yoffset) < 400))
		{
			float velocity = DRAG_SPEED * deltaTime * zoom_;
			distance_ = glm::length(center_ - position_);

			// Update the position of the camera
			position_ += right_ * xoffset * velocity / scene_width_percent_;
			position_ -= up_ * yoffset * velocity / scene_height_percent_;

			// Update the center of the camera
			//updateCameraCenter();
			center_ = position_ + distance_ * front_;
			center_.z = 0.0f;
			position_ = center_ - distance_ * front_;
		}
		else
		{
				std::stringstream msg;
					msg << "xOff: " << xoffset << "\tyOff: " << yoffset << "\n";
					MessageManager::AddMessage(msg, message_t::ERROR_T);
		}
	}

	/**
	 * @brief Processes input received from a mouse scroll-wheel event to scroll in and out the camera. Only requires input on the vertical wheel-axis.
	 * @param yoffset Mouse scroll y-axis wheel offset.
	 * @param deltaTime This parameter hold the time the hardware needs for one main loop. This prevents faster system to move the camera faster.
	 * @return Void.
	 */
	void ProcessMouseScroll(float yoffset, float deltaTime)
	{
		// The velocity of dragging the camera depends on the zoom level and the time the hardware needs for one main loop
		float velocity = deltaTime * zoom_ * SCROLL_SPEED;

		// Update the zoom level
		if (zoom_ >= ZOOM_MIN && zoom_ <= ZOOM_MAX)
			zoom_ -= yoffset * velocity;

		// Check if the Zoom is above the minimum level.
		if (zoom_ <= ZOOM_MIN)
			zoom_ = ZOOM_MIN;

		// Check if the Zoom is below the maximum level.
		if (zoom_ >= ZOOM_MAX)
			zoom_ = ZOOM_MAX;
	}

	/**
	 * @brief Processes input received from a mouse input system to rotate the camera.
	 * @param xoffset Mouse offset on the x-axis.
	 * @param yoffset Mouse offset on the y-axis.
	 * @param deltaTime This parameter hold the time the hardware needs for one main loop. This prevents faster system to move the camera faster.
	 * @return Void.
	 */

	void ProcessMouseRotate(float xoffset, float yoffset, float deltaTime)
	{
		if (state_ == CameraState::PERSPECTIVE)
		{
			// The velocity of rotating the camera depends the time the hardware needs for one main loop
			float velocity = ROTATE_SPEED * deltaTime;
			float yaw = xoffset * velocity / scene_width_percent_;
			float pitch = yoffset * velocity / scene_height_percent_;

			// Update pitch and yaw
			yaw_ += yaw;
			pitch_ += pitch;

			// If yaw gets greater or below the maximum and minimum angle in the unit circle, warp the angle.
			if (yaw_ > 360.0f)
				yaw_ = 0.0f;
			if (yaw_ < 0.0f)
				yaw_ = 360.0f;

			// Prevend the camera to flip if the pitch gets near the miximum and minimum value of 90 and -90 degree
			if (pitch_ > 89.999f)
				pitch_ = 89.999f;
			if (pitch_ < -89.999f)
				pitch_ = -89.999f;

			// Update camera position
			position_.x = center_.x + distance_ * glm::sin(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
			position_.z = center_.z + distance_ * glm::cos(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
			position_.y = center_.y + distance_ * glm::sin(glm::radians(pitch_));

			// Update the front, right and up camera vectors
			// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			front_ = -glm::normalize(position_ - center_);
			right_ = glm::normalize(glm::cross(-world_up_, front_));
			up_ = glm::normalize(glm::cross(front_, right_));
		}
	}


	/**
	* @brief Draws the camera center object.
	* @return Void.
	*/
	GLvoid DrawCenter(glm::mat4 projection, glm::mat4 view)
	{
		ResourceManager::GetShader("solid").Use();
		ResourceManager::GetShader("solid").SetMatrix4("projection", projection);
		ResourceManager::GetShader("solid").SetMatrix4("view", view);
		ResourceManager::GetShader("solid").SetVector3f("color", 1.0f, 1.0f, 1.0f);
		ResourceManager::GetShader("solid").SetFloat("alpha", 1.0f);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, center_);
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, focus_model_->GetScale());
		ResourceManager::GetShader("solid").SetMatrix4("model", model);
		focus_model_->Draw();
	}

private:
	/**
	 * @brief Default camera initialization.
	 * @return Void.
	 */
	void init()
	{
		// Camera center is coodinate origin
		center_ = glm::vec3(0.0f, 0.0f, 0.0f);
		world_up_ = glm::vec3(0.0f, 1.0f, 0.0f);

		// Default yaw and pitch values
		yaw_ = YAW;
		pitch_ = PITCH;

		// Default camera distance
		distance_ = 80.0;

		// Get the default camera position
		position_.x = center_.x + distance_ * glm::sin(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
		position_.z = center_.z + distance_ * glm::cos(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_));
		position_.y = center_.y + distance_ * glm::sin(glm::radians(pitch_));

		//position_ = glm::vec3(0.0f, 0.0f, 4.0f);

		// Update the front, right and up camera vectors
		// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		front_ = -glm::normalize(position_ - center_);
		right_ = glm::normalize(glm::cross(-world_up_, front_));
		up_ = glm::normalize(glm::cross(front_, right_));
		//front_ = glm::vec3(0.0f, 0.0f, -1.0f);
		//right_ = glm::vec3(1.0f, 0.0f, 0.0f);
		//up_ = glm::vec3(0.0f, -1.0f, 0.0f);

		// Default zoom and state
		zoom_ = ZOOM;
		roll_ = ROLL;
		state_ = CameraState::ORTHOGRAPHIC;

		//model_center_ = std::make_unique<Sphere>("CameraSphere", 1.0f, 6, 3, false);
		//model_center_->Scale(0.01f);
		scene_height_percent_ = 1.0f;
		scene_width_percent_ = 1.0f;
	}

	/**
	 * @brief Update the center of the camera. This is done by calculating the intersection between camera direction (front) and XZ plane.
	 * @return Void.
	 */
	void updateCameraCenter()
	{
		// Get the normalized camera direction vector
		//glm::vec3 RayDir = glm::normalize(front_);

		// Origin and normal of the XZ plane
		glm::vec3 PlaneOrigin(0, 0, 0);
		glm::vec3 PlaneNormal(0, 1, 0);

		// If the camera position is below 0 on the y axis, the XZ plane normal is swaped
		if (position_.y < 0.0f)
			PlaneNormal = glm::vec3(0, -1, 0);

		// Get the camera position and save it as the ray origin
		//glm::vec3 RayOrigin = position_;

		// Distance variable to store the distance between camera position and intersection point
		//float dist = 0.0f;

		// Get the intersection point between camera direction ray and the map mesh. GLM has a function which
		// takes a ray origin, direction.

		glm::vec3 tmpCenter = center_;

		if (tmpCenter.x != -99.0f && tmpCenter.y != -99.0f && tmpCenter.z != -99.0f)
			center_ = tmpCenter;

		// Assign the new distance
		distance_ = glm::length(center_ - position_);
	}

	glm::vec3			position_;						/**< Camera position in a cartesian coordinate system. */
	glm::vec3			center_;						/**< Camera center in a cartesian coordinate system. */
	glm::vec3			front_;							/**< Camera front vector. */
	glm::vec3			up_;							/**< Camera up vector. */
	glm::vec3			right_;							/**< Camera right vector.*/
	glm::vec3			world_up_;						/**< Camera world up vector. */
	
	float				distance_;						/**< Distance between camera position and center. */
	float				zoom_;							/**< Camera zoom level. */
	float				yaw_;							/**< Camera yaw angle. */
	float				pitch_;							/**< Camera pitch angle. */
	float				roll_;							/**< Camera roll angle. */

	GLfloat				scene_width_percent_;			/**< Scene width in percentage. */
	GLfloat				scene_height_percent_;			/**< Scene height in percentage. */

	CameraState			state_;							/**< Camera state. */
	std::unique_ptr<Cube>	focus_model_;
	
};

#endif

/**
 * @brief Camera class represents a camera object.
 */
/*
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
*/
