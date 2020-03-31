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


#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <memory>
#include <iostream>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/OpenGL.hpp>

/**
 * @brief The sindgleton resource manager class manages all resources like, textures, audio etc that are accessed across the application.
 */
class ResourceManager
{
public:
  static std::map<std::string, sf::Texture> Textures;                                 /**< Contains all SFML textures. */
  static std::map<std::string, std::unique_ptr<sf::RenderTexture>> RenderTextures;    /**< Contains all SFML render textures. */

  /**
	 * @brief Load a SFML texture from a file.
   * @param file The path and filename to the image.
   * @param maskColor A color that masks particular areas in the image and is replaced with transparancy (alpha = 0.0f).
   * @param name The name of the texture. The name is also the key in the map.
	 * @return GLvoid.
	 */
  static GLvoid LoadTexture(const char* file, sf::Color maskColor, std::string name);

  /**
	 * @brief Create an empty SFML render texture.
   * @param width Width of the new render texture.
   * @param height Height of the new render texture.
   * @param name The name of the render texture. The name is also the key in the map.
	 * @return GLvoid.
	 */
  static GLvoid CreateRenderTexture(GLuint width, GLuint height, std::string name);

  /**
	 * @brief Removes the old reference to the render texture and creates an empty SFML render texture with the same name.
   * @param width Width of the new render texture.
   * @param height Height of the new render texture.
   * @param name The name of the render texture. The name is also the key in the map.
	 * @return GLvoid.
	 */
  static GLvoid UpdateRenderTexture(GLuint width, GLuint height, std::string name);

  /**
	 * @brief Get the SFML texture by name.
   * @param name The name of the texture.
	 * @return Pointer to the texture.
	 */
  static sf::Texture* GetTexture(std::string name);

  /**
	 * @brief Get the SFML render texture by name.
   * @param name The name of the render texture.
	 * @return Pointer to the render texture.
	 */
  static sf::RenderTexture* GetRenderTexture(std::string name);

private:
  // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
  ResourceManager() { }
  ~ResourceManager() { }

  /**
	 * @brief Load a SFML texture from a file.
   * @param file The path and filename to the image.
	 * @return The SFML texture.
	 */
  static sf::Texture loadTextureFromFile(const char* file);
};

#endif
