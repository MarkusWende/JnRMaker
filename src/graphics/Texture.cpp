#include <iostream>

#include "Texture.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif


Texture2D::Texture2D()
	: Width(0), Height(0), Internal_Format(GL_RGBA), Image_Format(GL_RGBA), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_NEAREST), Filter_Mag(GL_NEAREST)
{
	glGenTextures(1, &this->ID);
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char* data)
{
	this->Width = width;
	this->Height = height;
	// Create Texture
	glBindTexture(GL_TEXTURE_2D, this->ID);
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

void Texture2D::Delete()
{
	if (this->ID != 0)
		glDeleteTextures(1, &this->ID);
}


// Class TextureCM
TextureCM::TextureCM(std::shared_ptr<ILogger> logger)
	: ui_logger_(std::dynamic_pointer_cast<UILogger>(logger)), Width(0), Height(0), Internal_Format(GL_RGBA), Image_Format(GL_RGBA), Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Wrap_R(GL_CLAMP_TO_EDGE), Filter_Min(GL_NEAREST), Filter_Mag(GL_NEAREST)
{
	glGenTextures(1, &this->ID);
}

void TextureCM::Generate(std::vector<std::string>* faces)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);

	int width, height, nrChannels;

	for (unsigned int i = 0; i < faces->size(); i++)
	{
		//unsigned char* image = SOIL_load_image(faces->at(i).c_str(), &width, &height, &nrChannels, 0);
		unsigned char* image = stbi_load(faces->at(i).c_str(), &width, &height, &nrChannels, 0);
		if (image)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
			);
			stbi_image_free(image);
		}
		else
		{
			std::stringstream msg;
			msg << "Cubemap tex failed to load at path: " << faces->at(i) << "\treason: " << stbi_failure_reason();
			ui_logger_->Log(log_t::ERROR_T, "%s", msg.str().c_str());
			stbi_image_free(image);
		}
	}

	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, this->Filter_Mag);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, this->Wrap_R);
}

void TextureCM::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

TextureCL::TextureCL()
	: Width(0), Height(0), Internal_Format(GL_R16), Image_Format(GL_RED), Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Filter_Min(GL_LINEAR), Filter_Mag(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}

void TextureCL::Generate(GLuint width, GLuint height)
{
	unsigned char* data = (unsigned char*)malloc(height * width * 4);

	this->Width = width;
	this->Height = height;
	// Create Texture
	glBindTexture(GL_TEXTURE_2D, this->ID);
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_SHORT, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glFinish();
}

void TextureCL::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

void TextureCL::Delete()
{
	if (this->ID != 0)
		glDeleteTextures(1, &this->ID);
}

ColorMap::ColorMap()
	: Width(0), Internal_Format(GL_RGBA), Image_Format(GL_RGBA), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Mag(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}

void ColorMap::Generate(GLuint width, unsigned char* data)
{
	//unsigned char* data = (unsigned char*)malloc(width * 3);

	this->Width = width;
	// Create Texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->ID);
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, 1, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	// Unbind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFinish();
}

void ColorMap::Bind() const
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->ID);
	//glActiveTexture(GL_TEXTURE0);
}

void ColorMap::Delete()
{
	if (this->ID != 0)
		glDeleteTextures(1, &this->ID);
}

// Class TextureAtlas
TextureArray::TextureArray()
	: Width(0), Height(0), Internal_Format(GL_RGBA), Image_Format(GL_RGBA), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Wrap_R(GL_REPEAT), Filter_Min(GL_NEAREST), Filter_Mag(GL_NEAREST), Mip_Level(1)
{
	glGenTextures(1, &this->ID);
}

void TextureArray::Generate(GLuint width, GLuint height, unsigned char* data, glm::vec2 spriteSize)
{
	if (this->ID != 0)
	{
		glDeleteTextures(1, &this->ID);
		glGenTextures(1, &this->ID);
	}

	this->Width = width;
	this->Height = height;

	glBindTexture(GL_TEXTURE_2D_ARRAY, this->ID);

	auto tileW = (GLuint)spriteSize.x;           // number of pixels in a row of 1 tile
	auto tileH = (GLuint)spriteSize.y;           // number of pixels in a column of 1 tile
	GLuint channels = 4;          // 4 for RGBA

	auto tilesX = this->Width / tileW;
	auto tilesY = this->Height / tileH;
	auto imageCount = tilesX * tilesY;

	for (GLuint mip = 0; mip < this->Mip_Level; ++mip)
	{
		// Set Texture wrap and filter modes
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, this->Filter_Mag);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, this->Mip_Level - 1);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, this->Wrap_S);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, this->Wrap_T);
		//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, this->Wrap_R);

		//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 0, this->Internal_Format, this->Width, this->Height, imageCount);
		glTexImage3D(GL_TEXTURE_2D_ARRAY, mip, this->Internal_Format, tileW, tileH, imageCount, 0, this->Image_Format, GL_UNSIGNED_BYTE, nullptr);
	}


	std::vector<unsigned char> tile(tileW * tileH * channels);
	auto tileSizeX = tileW * channels;
	auto rowLen    = tilesX * tileSizeX;

	for (GLuint iy = 0; iy < tilesY; ++ iy)
	{
		for (GLuint ix = 0; ix < tilesX; ++ ix)
		{
			unsigned char *ptr = data + iy*rowLen*tileH + ix*tileSizeX;
			for (GLuint row = 0; row < tileH; ++ row)
				std::copy(ptr + row*rowLen, ptr + row*rowLen + tileSizeX,
						tile.begin() + row*tileSizeX);


			auto i = iy * tilesX + ix;
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
				0, 0, i,
				tileW, tileH, 1,
				this->Image_Format, GL_UNSIGNED_BYTE, tile.data());
		}
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void TextureArray::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->ID);
}

void TextureArray::Delete()
{
	if (this->ID != 0)
		glDeleteTextures(1, &this->ID);
}