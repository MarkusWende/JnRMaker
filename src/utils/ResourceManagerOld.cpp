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
 * @file resource_manager.cpp
 * @brief This file contains the singleton resource manager class implementation.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */

#include "ResourceManagerOld.h"

#include "stb_image.h"

std::map<std::string, Texture2D> ResourceManagerOld::Textures;
std::map<std::string, TextureArray> ResourceManagerOld::TextureArrays;
// std::map<std::string, TextureAtlas> ResourceManagerOld::TextureAtlases;
// std::map<std::string, std::unique_ptr<sf::RenderTexture>> ResourceManagerOld::RenderTextures;
std::map<std::string, Framebuffer> ResourceManagerOld::Framebuffers;
std::map<std::string, Shader> ResourceManagerOld::Shaders;

Shader ResourceManagerOld::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
    // Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    // return Shaders[name];
}

Shader ResourceManagerOld::GetShader(std::string name)
{
    // return Shaders[name];
}

Texture2D ResourceManagerOld::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D ResourceManagerOld::CreateTexture(unsigned char *data, GLuint width, GLuint height, GLboolean alpha, std::string name)
{
    Textures[name] = createTextureFromData(data, width, height, alpha);
    return Textures[name];
}

Texture2D ResourceManagerOld::CreateTexture(const unsigned char *data, int size, std::string name)
{
    Textures[name] = createTextureFromData(data, size);
    return Textures[name];
}

TextureArray ResourceManagerOld::CreateTextureArray(unsigned char *data, GLuint width, GLuint height, GLuint spriteSizeX, GLuint spriteSizeY, GLboolean alpha, std::string name)
{
    Textures[name] = createTextureFromData(data, width, height, alpha);
    TextureArrays[name] = createTextureArrayFromData(data, width, height, spriteSizeX, spriteSizeY, alpha, name);
    return TextureArrays[name];
}

/* TextureArray ResourceManagerOld::CreateTextureArrayEmpty(std::string name, GLboolean alpha, glm::vec2 spriteSize, glm::vec2 spriteScale)
{
    TextureAtlases[name] = createTextureAtlasEmpty(alpha, spriteSize, spriteScale);
    return TextureAtlases[name];
}
TextureAtlas ResourceManagerOld::CreateTextureAtlasFromFile(std::string name, GLboolean alpha, glm::vec2 spriteSize, glm::vec2 spriteScale, const GLchar* file)
{
    TextureAtlases[name] = createTextureAtlasFromFile(alpha, spriteSize, spriteScale, file);
    return TextureAtlases[name];
}
TextureAtlas ResourceManagerOld::GetTextureAtlas(std::string name)
{
    return TextureAtlases[name];
} */

// GLvoid ResourceManagerOld::CreateRenderTexture(GLuint width, GLuint height, std::string name)
//{
/*
RenderTextures.insert(std::make_pair(name, std::unique_ptr<sf::RenderTexture>(new sf::RenderTexture)));
sf::RenderTexture* rTexture = RenderTextures.find(name)->second.get();
if(!rTexture->create(width, height))
{
    std::stringstream msg;
    msg << "Creating render texture.";
    MessageManager::AddMessage(msg, message_t::ERROR_T);
}
*/
//}
/*
GLvoid ResourceManagerOld::ResizeRenderTexture(GLuint width, GLuint height, std::string name)
{
  RenderTextures.erase(name);
  RenderTextures.insert(std::make_pair(name, std::unique_ptr<sf::RenderTexture>(new sf::RenderTexture)));
  sf::RenderTexture* rTexture = RenderTextures.find(name)->second.get();
  if(!rTexture->create(width, height))
  {
      std::stringstream msg;
      msg << "Updating render texture.";
      MessageManager::AddMessage(msg, message_t::ERROR_T);
  }
}
GLvoid ResourceManagerOld::UpdateRenderTexture(sf::Uint8* data, GLuint width, GLuint height, std::string name)
{
    ResizeRenderTexture(width, height, name);
    sf::Texture tex;
    tex.create(width, height);
    tex.update(data);
    sf::Sprite spr;
    spr.setOrigin(0, 32);
    spr.setScale(1.0f, -1.0f);
    spr.setTexture(tex);
    RenderTextures[name]->draw(spr);
}
sf::Texture* ResourceManagerOld::GetTexture(std::string name)
{
    return &Textures[name];
}
sf::RenderTexture* ResourceManagerOld::GetRenderTexture(std::string name)
{
    return RenderTextures.find(name)->second.get();
}
*/
Texture2D ResourceManagerOld::GetTexture(std::string name)
{
    return Textures[name];
}

TextureArray ResourceManagerOld::GetTextureArray(std::string name)
{
    return TextureArrays[name];
}

const std::map<std::string, Texture2D> &ResourceManagerOld::GetTextureMap()
{
    const std::map<std::string, Texture2D> &ptr = Textures;
    return ptr;
}

Framebuffer ResourceManagerOld::CreateFramebuffer(std::shared_ptr<ILogger> logger, std::string name, GLuint width, GLuint height, GLenum type)
{
    Framebuffers[name] = generateFramebuffer(logger, width, height, type);
    return Framebuffers[name];
}

// void ResourceManagerOld::DeleteFramebuffer(std::string name)
// {
//     //glDeleteBuffers(1, &Framebuffers[name].GetID());
// }

Framebuffer ResourceManagerOld::GetFramebuffer(std::string name)
{
    return Framebuffers[name];
}

Framebuffer ResourceManagerOld::ResizeFramebuffer(std::string name, GLuint width, GLuint height)
{
    Framebuffers[name].Resize(width, height);

    return Framebuffers[name];
}

std::string ResourceManagerOld::GetNameHash(std::string tilesetName, std::string tileName)
{
    std::stringstream hash;
    std::hash<std::string> hash_fn;
    size_t str_hash = hash_fn(tilesetName);
    hash << str_hash << ":" << tileName;
    return hash.str();
}

GLvoid ResourceManagerOld::DeleteTextureArray(std::string name)
{
    TextureArrays[name].Delete();
}

// PRIVATE:
////////////////////////////////////////////////////////////////////////////
Texture2D ResourceManagerOld::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
    // Create Texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    else
    {
        texture.Internal_Format = GL_RGB;
        texture.Image_Format = GL_RGB;
    }

    // Load from file
    int image_width = 0;
    int image_height = 0;
    // unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
    unsigned char *image_data = stbi_load(file, &image_width, &image_height, NULL, texture.Image_Format == GL_RGBA ? 4 : 3);

    if (NULL == image_data)
    {
        std::stringstream msg;
        msg << stbi_failure_reason() << ": " << file;
        //MessageManager::AddMessage(msg, message_t::ERROR_T);
    }
    else
    {
        // Now generate texture
        texture.Generate(image_width, image_height, image_data);
    }

    stbi_image_free(image_data);

    return texture;
}

Texture2D ResourceManagerOld::createTextureFromData(unsigned char *data, GLuint width, GLuint height, GLboolean alpha)
{
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    else
    {
        texture.Internal_Format = GL_RGB;
        texture.Image_Format = GL_RGB;
    }

    texture.Generate(width, height, data);

    return texture;
}

Texture2D ResourceManagerOld::createTextureFromData(const unsigned char *data, int size)
{
    Texture2D texture;
    texture.Internal_Format = GL_RGBA;
    texture.Image_Format = GL_RGBA;

    int width;
    int height;
    int channels = 4;

    if (!stbi_info_from_memory(data, size, &width, &height, &channels))
    {
        std::stringstream msg;
        msg << stbi_failure_reason();
        //MessageManager::AddMessage(msg, message_t::ERROR_T);
        return texture;
    }

    unsigned char *img = stbi_load_from_memory(data, size, &width, &height, &channels, 4);
    // unsigned char* image_data = stbi_load_from_memory(data, size, &width, &height, &channelsInData, channels);

    /* exit if the image is larger than ~80MB */
    if (width && height > (80000000 / 4) / height)
    {
        std::stringstream msg;
        msg << "Image is bigger than 80MB.";
        //MessageManager::AddMessage(msg, message_t::ERROR_T);
        return texture;
    }

    if (NULL == img)
    {
        std::stringstream msg;
        msg << stbi_failure_reason();
        //MessageManager::AddMessage(msg, message_t::ERROR_T);
    }
    else
    {
        std::stringstream msg;
        msg << "Width: " << width << "\tHeight: " << height;
        //MessageManager::AddMessage(msg, message_t::DEBUG);
        texture.Generate((GLuint)width, (GLuint)height, img);
    }

    stbi_image_free(img);

    return texture;
}

TextureArray ResourceManagerOld::createTextureArrayFromData(unsigned char *data, GLuint width, GLuint height, GLuint spriteSizeX, GLuint spriteSizeY, GLboolean alpha, std::string name)
{
    TextureArray texArray;
    int channels = 0;
    if (alpha)
    {
        texArray.Internal_Format = GL_RGBA;
        texArray.Image_Format = GL_RGBA;
        channels = 4;
    }
    else
    {
        texArray.Internal_Format = GL_RGB;
        texArray.Image_Format = GL_RGB;
        channels = 3;
    }

    texArray.Generate(width, height, data, {spriteSizeX, spriteSizeY});

    std::vector<unsigned char> tile(spriteSizeX * spriteSizeY * channels);
    auto tilesX = width / spriteSizeX;
    auto tilesY = height / spriteSizeY;
    auto tileSizeX = spriteSizeX * channels;
    auto rowLen = tilesX * tileSizeX;

    for (GLuint iy = 0; iy < tilesY; ++iy)
    {
        for (GLuint ix = 0; ix < tilesX; ++ix)
        {
            auto ptr = data + iy * rowLen * spriteSizeY + ix * tileSizeX;
            for (GLuint row = 0; row < spriteSizeY; ++row)
                std::copy(ptr + row * rowLen, ptr + row * rowLen + tileSizeX,
                          tile.begin() + row * tileSizeX);

            std::stringstream tileName;
            tileName << "r" << iy << "c" << ix;
            std::string hashKey = GetNameHash(name, tileName.str());
            CreateTexture(tile.data(), spriteSizeX, spriteSizeY, alpha, hashKey);
        }
    }

    return texArray;
}

/* TextureAtlas ResourceManagerOld::createTextureAtlasEmpty(GLboolean alpha, glm::vec2 spriteSize, glm::vec2 spriteScale)
{
    TextureAtlas texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    else {
        texture.Internal_Format = GL_RGB;
        texture.Image_Format = GL_RGB;
    }
    // Now generate texture
    texture.Generate(spriteSize);
    // And finally free image data
    //SOIL_free_image_data(image);
    return texture;
}
TextureAtlas ResourceManagerOld::createTextureAtlasFromFile(GLboolean alpha, glm::vec2 spriteSize, glm::vec2 spriteScale, const GLchar* file)
{
    TextureAtlas texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    else {
        texture.Internal_Format = GL_RGB;
        texture.Image_Format = GL_RGB;
    }
    // Load image
    int width, height;
    unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

    if (NULL == image)
    {
        std::stringstream msg;
        msg << SOIL_last_result() << ": " << file;
        MessageManager::AddMessage(msg, message_t::ERROR_T);
    }
    // Now generate texture
    texture.Generate(width, height, image, spriteSize);
    // And finally free image data
    //SOIL_free_image_data(image);
    return texture;
} */

Framebuffer ResourceManagerOld::generateFramebuffer(std::shared_ptr<ILogger> logger, GLuint width, GLuint height, GLenum type)
{
    // create framebuffer object
    Framebuffer fb;

    // generate framebuffer
    fb.Generate(width, height, type);

    return fb;
}

Shader ResourceManagerOld::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
//     // 1. Retrieve the vertex/fragment source code from filePath
//     std::string vertexCode;
//     std::string fragmentCode;
//     std::string geometryCode;
//     try
//     {
//         // Open files
//         std::ifstream vertexShaderFile(vShaderFile);
//         std::ifstream fragmentShaderFile(fShaderFile);
//         std::stringstream vShaderStream, fShaderStream;
//         // Read file's buffer contents into streams
//         vShaderStream << vertexShaderFile.rdbuf();
//         fShaderStream << fragmentShaderFile.rdbuf();
//         // close file handlers
//         vertexShaderFile.close();
//         fragmentShaderFile.close();
//         // Convert stream into string
//         vertexCode = vShaderStream.str();
//         fragmentCode = fShaderStream.str();
//         // If geometry shader path is present, also load a geometry shader
//         if (gShaderFile != nullptr)
//         {
//             std::ifstream geometryShaderFile(gShaderFile);
//             std::stringstream gShaderStream;
//             gShaderStream << geometryShaderFile.rdbuf();
//             geometryShaderFile.close();
//             geometryCode = gShaderStream.str();
//         }
//     }
//     catch (...)
//     {
//         FILE *stream;
// #if _WIN32
//         freopen_s(&stream, "log.txt", "w", stdout);
//         fprintf(stream, "\tFailed to read shader files\n");
//         fclose(stream);
// #elif __linux__
//         stream = fopen("./log.txt", "w");
//         fprintf(stream, "\tFailed to read shader files\n");
//         fclose(stream);
// #else
//         (void)stream;
// #endif
//         delete stream;
//     }
//     const GLchar *vShaderCode = vertexCode.c_str();
//     const GLchar *fShaderCode = fragmentCode.c_str();
//     const GLchar *gShaderCode = geometryCode.c_str();
//     // 2. Now create shader object from source code
//     Shader shader;
//     shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
//     return shader;
}