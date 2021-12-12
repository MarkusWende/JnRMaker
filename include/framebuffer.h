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
 * @file framebuffer.h
 * @author Kirsch Audio GmbH
 * @brief This file contains the OpenGL framebuffer class.
 */


#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

//#include <SFML/OpenGL.hpp>
//#define GLEW_STATIC
#include <GL/glew.h>

/**
 * @brief Defines an OpenGL framebuffer object with a bound texture.
 */
class Framebuffer
{
public:
	/**
	 * @brief Constructor for Framebuffer.
	 */
	Framebuffer();

	/**
	 * @brief Destructor for Framebuffer.
	 */
	~Framebuffer() { }

	// Getters
	GLuint& GetID() { return id_; }								/**< @brief Get the framebuffer ID. @return A reference to an OpenGL unsignet integer. */
	GLuint& GetTextureID() { return tex_id_; }						/**< @brief Get the texture ID. @return A reference to an OpenGL unsignet integer. */

	// General member functions
	/**
	 * @brief Generates the framebuffer object with a bound texture.
	 * @param width Framebuffer width.
	 * @param height Framebuffer height.
	 * @return void
	 */
	void Generate(GLuint width, GLuint height);

	void Resize(GLuint width, GLuint height);

	/**
	 * @brief Binds the framebuffer as the current active framebuffer object.
	 * @return void
	 */
	void Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, id_); }

	/**
	 * @brief Delete the framebuffer and texture.
	 * @return void
	 */
	void Delete();

private:
	GLuint id_;										/**< Holds the ID of the framebuffer and is used for all framebuffer operations to reference to this particlar framebuffer. */
	GLuint tex_id_;									/**< Holds the ID of the bound texture and is used for all texture operations to reference to this particlar texture. */
	GLuint width_;									/**< Width in pixels of the texture and framebuffer. */
	GLuint height_;									/**< Height in pixels of the texture and framebuffer. */
	GLuint internal_format_;						/**< Format of the texture. */
	GLuint image_format_;							/**< Format of loaded image. */
	GLuint wrap_s_;									/**< Wrapping mode on S axis. */
	GLuint wrap_t_;									/**< Wrapping mode on T axis. */
	GLuint filter_min_;								/**< Filtering mode if texture pixels < screen pixels. */
	GLuint filter_max_;								/**< Filtering mode if texture pixels > screen pixels. */
	GLuint rbo_;
};

#endif	/* FRAMEBUFFER_H */
