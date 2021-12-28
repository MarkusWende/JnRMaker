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


#include "../include/resource_manager.h"
#include "../include/message_manager.h"

std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, TextureAtlas> ResourceManager::TextureAtlases;
//std::map<std::string, std::unique_ptr<sf::RenderTexture>> ResourceManager::RenderTextures;
std::map<std::string, Framebuffer>  ResourceManager::Framebuffers;
std::map<std::string, Shader>       ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar* file, GLboolean alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

TextureAtlas ResourceManager::CreateTextureAtlasEmpty(std::string name, GLboolean alpha, glm::vec2 spriteSize, glm::vec2 spriteScale)
{
    TextureAtlases[name] = createTextureAtlasEmpty(alpha, spriteSize, spriteScale);
    return TextureAtlases[name];
}

TextureAtlas ResourceManager::CreateTextureAtlasFromFile(std::string name, GLboolean alpha, glm::vec2 spriteSize, glm::vec2 spriteScale, const GLchar* file)
{
    TextureAtlases[name] = createTextureAtlasFromFile(alpha, spriteSize, spriteScale, file);
    return TextureAtlases[name];
}

TextureAtlas ResourceManager::GetTextureAtlas(std::string name)
{
    return TextureAtlases[name];
}

//GLvoid ResourceManager::CreateRenderTexture(GLuint width, GLuint height, std::string name)
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
GLvoid ResourceManager::ResizeRenderTexture(GLuint width, GLuint height, std::string name)
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

GLvoid ResourceManager::UpdateRenderTexture(sf::Uint8* data, GLuint width, GLuint height, std::string name)
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

sf::Texture* ResourceManager::GetTexture(std::string name)
{
	return &Textures[name];
}

sf::RenderTexture* ResourceManager::GetRenderTexture(std::string name)
{
	return RenderTextures.find(name)->second.get();
}
*/
Texture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

Framebuffer ResourceManager::CreateFramebuffer(std::string name, GLuint width, GLuint height)
{
    Framebuffers[name] = generateFramebuffer(width, height);
    return Framebuffers[name];
}

void ResourceManager::DeleteFramebuffer(std::string name)
{
    glDeleteBuffers(1, &Framebuffers[name].GetID());
}

Framebuffer ResourceManager::GetFramebuffer(std::string name)
{
    return Framebuffers[name];
}

Framebuffer ResourceManager::ResizeFramebuffer(std::string name, GLuint width, GLuint height)
{
    Framebuffers[name].Resize(width, height);

    return Framebuffers[name];
}

std::string ResourceManager::getNameHash(std::string tilesetName, std::string tileName)
{
    std::stringstream hash;
    std::hash<std::string> hash_fn;
    size_t str_hash = hash_fn(tilesetName);
    hash << str_hash << ":" << tileName;
    return hash.str();
}


// PRIVATE:
////////////////////////////////////////////////////////////////////////////
Texture2D ResourceManager::loadTextureFromFile(const GLchar* file, GLboolean alpha)
{
    // Create Texture object
    Texture2D texture;
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
    texture.Generate(width, height, image);
    // And finally free image data
    SOIL_free_image_data(image);
    return texture;
}

TextureAtlas ResourceManager::createTextureAtlasEmpty(GLboolean alpha, glm::vec2 spriteSize, glm::vec2 spriteScale)
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
    texture.Generate(spriteSize, spriteScale);
    // And finally free image data
    //SOIL_free_image_data(image);

    return texture;
}

TextureAtlas ResourceManager::createTextureAtlasFromFile(GLboolean alpha, glm::vec2 spriteSize, glm::vec2 spriteScale, const GLchar* file)
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
    texture.Generate(width, height, image, spriteSize, spriteScale);
    // And finally free image data
    //SOIL_free_image_data(image);

    return texture;
}

Framebuffer ResourceManager::generateFramebuffer(GLuint width, GLuint height)
{
    // create framebuffer object
    Framebuffer fb;

    // generate framebuffer
    fb.Generate(width, height);

    return fb;
}

Shader ResourceManager::loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        FILE* stream;
#ifdef _WIN32
        freopen_s(&stream, "log.txt", "w", stdout);
        fprintf(stream, "\tFailed to read shader files\n");
        fclose(stream);
#endif // _WIN32
#ifdef __linux__
        stream = fopen("./log.txt", "w");
        fprintf(stream, "\tFailed to read shader files\n");
        fclose(stream);
#endif // __linux__
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    const GLchar* gShaderCode = geometryCode.c_str();
    // 2. Now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}