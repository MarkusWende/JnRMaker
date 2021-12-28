#include <iostream>

#include "../include/texture.h"


Texture2D::Texture2D()
	: Width(0), Height(0), Internal_Format(GL_RGBA), Image_Format(GL_RGBA), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR_MIPMAP_LINEAR), Filter_Mag(GL_NEAREST)
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


// Class TextureCM
TextureCM::TextureCM()
	: Width(0), Height(0), Internal_Format(GL_RGBA), Image_Format(GL_RGBA), Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Wrap_R(GL_CLAMP_TO_EDGE), Filter_Min(GL_LINEAR), Filter_Mag(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}

void TextureCM::Generate(std::vector<std::string>* faces)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);

	int width, height, nrChannels;

	for (unsigned int i = 0; i < faces->size(); i++)
	{
		unsigned char* image = SOIL_load_image(faces->at(i).c_str(), &width, &height, &nrChannels, 0);
		if (image)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
			);
			SOIL_free_image_data(image);
		}
		else
		{
			std::stringstream msg;
			msg << "Cubemap tex failed to load at path: " << faces->at(i);
			MessageManager::AddMessage(msg, message_t::ERROR_T);
			SOIL_free_image_data(image);
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
TextureAtlas::TextureAtlas()
	: Width(0), Height(0), Internal_Format(GL_RGBA), Image_Format(GL_RGBA), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Wrap_R(GL_REPEAT), Filter_Min(GL_NEAREST), Filter_Mag(GL_NEAREST)
{
	glGenTextures(1, &this->ID);
}

void TextureAtlas::Generate(glm::vec2 spriteSize, glm::vec2 spriteScale)
{
	this->Width = 0;
	this->Height = 0;
	GLsizei layerCount = 1;
	GLsizei mipLevelCount = 1;

	// Create Texture
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->ID);
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, this->Filter_Mag);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, this->Wrap_R);
	//glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	// Allocate the storage.
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, this->Internal_Format, this->Width, this->Height, layerCount);
	// Upload pixel data.
	// The first 0 refers to the mipmap level (level 0, since there's only 1)
	// The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
	// The final 0 refers to the layer index offset (we start from index 0 and have 2 levels).
	// Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
	//glTexSubImage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount - 1, 0, 0, 0, this->Width, this->Height, layerCount, GL_RGBA, GL_UNSIGNED_BYTE, texels);


	/* // Always set reasonable texture parameters
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	
	glTexImage3D(	GL_TEXTURE_2D_ARRAY,
					0, texture.Internal_Format, 
                tileW, tileH, imageCount, 0,
                texture.Image_Format, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	glCopyImageSubData();

	//unsigned char* data = ...; // 16x16 texture atlas image data
    int tileW = ...;           // number of pixels in a row of 1 tile
    int tileH = ...;           // number of pixels in a column of 1 tile
    int channels = 4;          // 4 for RGBA

    //int tilesX = 16;
    //int tilesY = 16;
    //int imageCount = tilesX * tilesY;

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, texture.Internal_Format, 
                tileW, tileH, imageCount, 0,
                texture.Image_Format, GL_UNSIGNED_BYTE, nullptr);

    std::vector<unsigned char> tile(tileW * tileH * channels);
    int tileSizeX = tileW * channels;
    int rowLen    = tilesX * tileSizeX;

    for (int iy = 0; iy < tilesY; ++ iy)
    {
        for (int ix = 0; ix < tilesX; ++ ix)
        {
            unsigned char *ptr = data + iy*rowLen + ix*tileSizeX;
            for (int row = 0; row < tileH; ++ row)
                std::copy(ptr + row*rowLen, ptr + row*rowLen + tileSizeX,
                        tile.begin() + row*tileSizeX);


            int i = iy * tilesX + ix;
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                0, 0, i,
                tileW, tileH, 1,
                this->Image_Format, GL_UNSIGNED_BYTE, tile.data());
        }
    } */
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void TextureAtlas::Generate(GLuint width, GLuint height, unsigned char* data, glm::vec2 spriteSize, glm::vec2 spriteScale)
{
	this->Width = width;
	this->Height = height;

	glBindTexture(GL_TEXTURE_2D_ARRAY, this->ID);
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, this->Filter_Mag);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, this->Wrap_T);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, this->Wrap_R);

	int tileW = spriteSize.x;           // number of pixels in a row of 1 tile
	int tileH = spriteSize.x;           // number of pixels in a column of 1 tile
	int channels = 4;          // 4 for RGBA

	int tilesX = this->Width / spriteSize.x;
	int tilesY = this->Height / spriteSize.y;
	int imageCount = tilesX * tilesY;

	//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 0, this->Internal_Format, this->Width, this->Height, imageCount);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, this->Internal_Format, tileW, tileH, imageCount, 0, this->Image_Format, GL_UNSIGNED_BYTE, nullptr);

	std::vector<unsigned char> tile(tileW * tileH * channels);
	int tileSizeX = tileW * channels;
	int rowLen    = tilesX * tileSizeX;

	for (int iy = 0; iy < tilesY; ++ iy)
	{
		for (int ix = 0; ix < tilesX; ++ ix)
		{
			unsigned char *ptr = data + iy*rowLen*tileH + ix*tileSizeX;
			for (int row = 0; row < tileH; ++ row)
				std::copy(ptr + row*rowLen, ptr + row*rowLen + tileSizeX,
						tile.begin() + row*tileSizeX);


			int i = iy * tilesX + ix;
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
				0, 0, i,
				tileW, tileH, 1,
				this->Image_Format, GL_UNSIGNED_BYTE, tile.data());
		}
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void TextureAtlas::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->ID);
}