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
 * @file ResourceManagerOldOld.h
 * @brief This file contains the singleton resource manager class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Framebuffer.h"
#include "Shader.h"
#include "Texture.h"

//#include <SFML/Graphics/Image.hpp>
//#include <SFML/Graphics/Texture.hpp>
//#include <SFML/Graphics/Sprite.hpp>
//#include <SFML/Graphics/RenderTexture.hpp>
//#include <SFML/Graphics/Color.hpp>
//#include <SFML/OpenGL.hpp>


/**
 * @brief The sindgleton resource manager class manages all resources like, textures, audio etc that are accessed across the application.
 */
class ResourceManagerOld
{
public:
  static std::map<std::string, Texture2D>		Textures;							/**< Texture vector. Stores every texture used in the application. */
  static std::map<std::string, TextureArray> TextureArrays;							/**< Texture vector. Stores every texture used in the application. */
  //static std::map<std::string, std::unique_ptr<sf::RenderTexture>> RenderTextures;    /**< Contains all SFML render textures. */
  static std::map<std::string, Framebuffer>	Framebuffers;						        /**< Framebuffer vector. Stores every framebuffer used in the application. */
  //static std::map<std::string, Shader>		Shaders;							/**< Shader vector. Stores every shader used in the application. */

  /**
     * @brief Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader.
     * @param vShaderFile Filename of the vertex shader.
     * @param fShaderFile Filename of the fragment shader.
     * @param gShaderFile Filename of the geometry shader.
     * @param name Resource name of the shader.
     * @return Shader Return the shader object.
     */
  //static Shader   LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);

  /**
   * @brief Retrieves a stored shader.
   * @param name Resource name of the shader.
   * @return Shader Return the shader object.
   */
  //static Shader GetShader(std::string name);

  /**
    * @brief Loads (and generates) a texture from file.
    * @param file Filename of the texture.
    * @param alpha Set the alpha channel true or false.
    * @param name Resource name of the texture.
    * @return Texture2D Return the texture object.
    */
  static Texture2D LoadTexture(const GLchar* file, GLboolean alpha, std::string name);

  static Texture2D CreateTexture(unsigned char* data, GLuint width, GLuint height, GLboolean alpha, std::string name);
  static Texture2D CreateTexture(const unsigned char* data, int size, std::string name);

  static TextureArray CreateTextureArray(unsigned char* data, GLuint width, GLuint height, GLuint spriteSizeX, GLuint spriteSizeY, GLboolean alpha, std::string name);


  //static TextureAtlas CreateTextureAtlasEmpty(std::string name, GLboolean alpha, glm::vec2 spriteSize, glm::vec2 spriteScale);
  //static TextureAtlas CreateTextureAtlasFromFile(std::string name, GLboolean alpha, glm::vec2 spriteSize, glm::vec2 spriteScale, const GLchar* file);
  //static TextureAtlas GetTextureAtlas(std::string name);

  /**
	* @brief Create an texture from id.
	* @return GLvoid.
	*/
  //static GLvoid CreateEmptyTexture(GLuint width, GLuint height, GLboolean alpha, std::string name);

    /**
     * @brief Removes the old reference to the render texture and creates an empty SFML render texture with the same name.
     * @param width Width of the new render texture.
     * @param height Height of the new render texture.
     * @param name The name of the render texture. The name is also the key in the map.
     * @return GLvoid.
     */
    //static GLvoid ResizeRenderTexture(GLuint width, GLuint height, std::string name);

    //static GLvoid UpdateRenderTexture(sf::Uint8* data, GLuint width, GLuint height, std::string name);

   /**
     * @brief Retrieves a stored texture.
     * @param name Resource name of the texture.
     * @return Texture2D Return the texture object.
     */
   static Texture2D GetTexture(std::string name);

   static TextureArray GetTextureArray(std::string name);

   static const std::map<std::string, Texture2D> & GetTextureMap();

  /**
	 * @brief Get the SFML render texture by name.
   * @param name The name of the render texture.
	 * @return Pointer to the render texture.
	 */
  //static sf::RenderTexture* GetRenderTexture(std::string name);

  /**
     * @brief Generate a framebuffer.
     * @param name Resource name of the framebuffer.
     * @param width Width of the framebuffer.
     * @param height Height of the framebuffer.
     * @return Framebuffer Return the framebuffer object.
     */
  static Framebuffer CreateFramebuffer(std::shared_ptr<ILogger> logger, std::string name, GLuint width, GLuint height, GLenum type);

//   /**
//    * @brief Delete framebuffer by name.
//    * @param name Resource name of the framebuffer.
//    * @return Void.
//    */
//   static void DeleteFramebuffer(std::string name);

  /**
     * @brief Resize a given framebuffer.
     * @param name Resource name of the framebuffer.
     * @param width New width of the framebuffer.
     * @param height New height of the framebuffer.
     * @return Framebuffer Return the framebuffer object.
     */
  static Framebuffer ResizeFramebuffer(std::string name, GLuint width, GLuint height);

  /**
   * @brief Retrieves a stored framebuffer.
   * @param name Resource name of the framebuffer.
   * @return Framebuffer Return the framebuffer object.
   */
  static Framebuffer GetFramebuffer(std::string name);

  static std::string GetNameHash(std::string tilesetName, std::string tileName);

  static GLvoid DeleteTextureArray(std::string name);

private:
  // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
  ResourceManagerOld() { }
  ~ResourceManagerOld() { }

  /**
     * @brief Loads (and generates) a texture from file.
     * @param file Filename of the texture.
     * @param alpha Set the alpha channel true or false.
     * @return Texture2D Return the texture object.
     */
  static Texture2D loadTextureFromFile(const GLchar* file, GLboolean alpha);

  static Texture2D createTextureFromData(unsigned char* data, GLuint width, GLuint height, GLboolean alpha);
  static Texture2D createTextureFromData(const unsigned char* data, int size);

  static TextureArray loadTextureArrayFromFile(const GLchar* file, GLboolean alpha);

  static TextureArray createTextureArrayFromData(unsigned char* data, GLuint width, GLuint height, GLuint spriteSizeX, GLuint spriteSizeY, GLboolean alpha, std::string name);

  //static TextureAtlas createTextureAtlasEmpty(GLboolean alpha, glm::vec2 spriteSize, glm::vec2 spriteScale);
  //static TextureAtlas createTextureAtlasFromFile(GLboolean alpha, glm::vec2 spriteSize, glm::vec2 spriteScale, const GLchar* file);

  /**
     * @brief Generate a framebuffer.
     * @param width Width of the framebuffer.
     * @param height Height of the framebuffer.
     * @return Framebuffer Return the framebuffer object.
     */
  static Framebuffer generateFramebuffer(std::shared_ptr<ILogger> logger, GLuint width, GLuint height, GLenum type);

  /**
     * @brief Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader.
     * @param vShaderFile Filename of the vertex shader.
     * @param fShaderFile Filename of the fragment shader.
     * @param gShaderFile Filename of the geometry shader.
     * @return Shader Return the shader object.
     */
  //static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
};

#endif