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
 * @file resource_manager.h
 * @brief This file contains the singleton resource manager class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>

#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include "glm/glm.hpp"

#include "message_manager.h"

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D
{
public:
	// Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
	GLuint ID;
	// Texture image dimensions
	GLuint Width, Height; // Width and height of loaded image in pixels
	// Texture Format
	GLuint Internal_Format; // Format of texture object
	GLuint Image_Format; // Format of loaded image
	// Texture configuration
	GLuint Wrap_S; // Wrapping mode on S axis
	GLuint Wrap_T; // Wrapping mode on T axis
	GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
	GLuint Filter_Mag; // Filtering mode if texture pixels > screen pixels
	// Constructor (sets default texture modes)
	Texture2D();
	// Generates texture from image data
	void Generate(GLuint width, GLuint height, unsigned char* data);
	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;
};

// TextureCM is able to store and configure a cube map texture in OpenGL.
// It also hosts utility functions for easy management.
class TextureCM
{
public:
	// Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
	GLuint ID;
	// Texture image dimensions
	GLuint Width, Height; // Width and height of loaded image in pixels
	// Texture Format
	GLuint Internal_Format; // Format of texture object
	GLuint Image_Format; // Format of loaded image
	// Texture configuration
	GLuint Wrap_S; // Wrapping mode on S axis
	GLuint Wrap_T; // Wrapping mode on T axis
	GLuint Wrap_R; // Wrapping mode on R axis
	GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
	GLuint Filter_Mag; // Filtering mode if texture pixels > screen pixels
	// Constructor (sets default texture modes)
	TextureCM();
	// Generates texture from image data
	void Generate(std::vector<std::string>* faces);
	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;
};

class TextureCL
{
public:
	// Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
	GLuint ID;
	// Texture image dimensions
	GLuint Width, Height; // Width and height of loaded image in pixels
	// Texture Format
	GLuint Internal_Format; // Format of texture object
	GLuint Image_Format; // Format of loaded image
	// Texture configuration
	GLuint Wrap_S; // Wrapping mode on S axis
	GLuint Wrap_T; // Wrapping mode on T axis
	GLuint Wrap_R; // Wrapping mode on R axis
	GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
	GLuint Filter_Mag; // Filtering mode if texture pixels > screen pixels
	// Constructor (sets default texture modes)
	TextureCL();
	// Generates texture from image data
	void Generate(GLuint width, GLuint height);
	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;

	void Delete();
};

class ColorMap
{
public:
	// Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
	GLuint ID;
	// Texture image dimensions
	GLuint Width; // Width and height of loaded image in pixels
	// Texture Format
	GLuint Internal_Format; // Format of texture object
	GLuint Image_Format; // Format of loaded image
	// Texture configuration
	GLuint Wrap_S; // Wrapping mode on S axis
	GLuint Wrap_T; // Wrapping mode on T axis
	GLuint Wrap_R; // Wrapping mode on R axis
	GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
	GLuint Filter_Mag; // Filtering mode if texture pixels > screen pixels
	// Constructor (sets default texture modes)
	ColorMap();
	// Generates texture from image data
	void Generate(GLuint width, unsigned char* data);
	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;

	void Delete();
};

class TextureArray
{
public:
	// Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
	GLuint ID;
	// Texture image dimensions
	GLuint Width, Height; // Width and height of loaded image in pixels
	// Texture Format
	GLuint Internal_Format; // Format of texture object
	GLuint Image_Format; // Format of loaded image
	// Texture configuration
	GLuint Wrap_S; // Wrapping mode on S axis
	GLuint Wrap_T; // Wrapping mode on T axis
	GLuint Wrap_R; // Wrapping mode on T axis
	GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
	GLuint Filter_Mag; // Filtering mode if texture pixels > screen pixels
	GLuint Mip_Level;
	// Constructor (sets default texture modes)
	TextureArray();
	// Generates texture from image data
	void Generate(glm::vec2 spriteSize);
	void Generate(GLuint width, GLuint height, unsigned char* data, glm::vec2 spriteSize);
	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;

	void Delete();
};

#endif