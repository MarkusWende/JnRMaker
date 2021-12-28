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
 * @file scene.h
 * @brief This file contains the scene class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */

#ifndef SCENE_H
#define SCENE_H

#include <sstream>
#include <map>
#include <memory>

#include "resource_manager.h"
#include "entity_camera.h"
#include "entity_solid.h"
#include "entity_object.h"
#include "entity_player.h"
#include "entity_level_layer.h"
#include "message_manager.h"
#include "tilemap_manager.h"
#include "project_manager.h"
#include "primitive_cube.h"
#include "solid_grid.h"
#include "solid_cs.h"
#include "solid_sprite.h"

#ifdef _WIN32
#include <filesystem>
#include <windows.h>
namespace fs = std::filesystem;
#endif // _WIN32
#ifdef __linux__
#include <experimental/filesystem>
#include <unistd.h>
namespace fs = std::experimental::filesystem;
#endif

//#include <SFML/OpenGL.hpp>
//#include <SFML/System/Vector2.hpp>
//#include <SFML/Graphics/RectangleShape.hpp>


/**
 * @brief Scene class represents the rendered the, which is stored in a texture and displayed in ImGui.
 */
class Scene
{
public:
	Scene(GLuint width, GLuint height);														//!< constructor
	~Scene();																				//!< destructor

	/**
	 * @brief Create a new map.
	 * @return GLvoid.
	 */
	GLvoid CreateMap(GLuint width, GLuint height, glm::vec2 spriteSize, glm::vec2 spriteScale);

	/**
	 * @brief Update the scene, when the window size changes.
	 * @return GLvoid.
	 */
	GLvoid Update(GLuint width, GLuint height);

	/**
	 * @brief Render all entities that are displayed to the scene texture.
	 * @return GLvoid.
	 */
	GLvoid Render();

	// Setters

	GLvoid SetMouseOverScene(GLboolean status) { mouse_over_scene_ = status; };
	//GLvoid SetAddSpriteFlag() { add_sprite_flag_ = true; };
	GLvoid SetActiveTilemap(std::string name) { active_tilemap_name_ = name; };
	GLvoid SetActiveSprite(std::string name) { active_sprite_name_ = name; };
	GLvoid SetActiveLayer(layer_t layer) { active_layer_ = layer; };
	GLvoid SetMousePosition(glm::vec2 pos) { mouse_position_ = pos; };

	GLvoid PlaceSprite();

	GLboolean IsMouseOverScene() { return mouse_over_scene_; };
	GLboolean IsMapNull() { return map_is_null_; };

	GLuint	GetWidth() { return width_; };
	GLuint	GetHeight() { return height_; };
	GLuint	GetMapWidth() { return map_width_; };
	GLuint	GetMapHeight() { return map_height_; };
	glm::vec2 GetSpriteSize() { return sprite_size_; };
	glm::vec2 GetSpriteScale() { return sprite_scale_; };
	Camera* GetCamera(std::string cameraName) { return e_cameras_.find(cameraName)->second.get(); };
	Sprite* GetSprite(std::string spriteName) { return e_sprites_.find(spriteName)->second.get(); };
	std::string GetActiveTilemap() { return active_tilemap_name_; };
	std::string GetActiveSprite() { return active_sprite_name_; };

	//std::map<std::string, std::unique_ptr<sf::Sprite>> 	tiles_;	/**< All solid entities are stored in this map. */

private:

	friend class cereal::access;
	template <class Archive>
	void save(Archive& ar, std::uint32_t const version) const
	{
		ar(CEREAL_NVP(e_solids_));
		ar( CEREAL_NVP(map_width_), CEREAL_NVP(map_height_) );

		//sf::Texture tex = ResourceManager::GetRenderTexture("tex_used_tiles")->getTexture();
		//sf::Image img;
		//img = tex.copyToImage();
		//const sf::Uint8* imgData = img.getPixelsPtr();
		//ar(cereal::make_nvp("tex_used_tiles_width", img.getSize().x));
		//ar(cereal::make_nvp("tex_used_tiles_height", img.getSize().y));
		//ar.saveBinaryValue(imgData, sizeof(sf::Uint8) * img.getSize().x * img.getSize().y * 4, "tex_used_tiles");

		//ar(CEREAL_NVP(map_fg_));
		//ar(CEREAL_NVP(map_pg_));
		//ar(CEREAL_NVP(map_bg_));
	}

	template <class Archive>
	void load(Archive& ar, std::uint32_t const version)
	{
		ar(e_solids_);

		GLuint width, height;
		ar(width, height);

		glm::vec2 sprSize = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteSize();
		glm::vec2 sprScale = TilemapManager::GetTilemap(active_tilemap_name_)->GetSpriteScale();
		CreateMap(width, height, glm::vec2(sprSize.x, sprSize.y), glm::vec2( sprScale.x, sprScale.y ));

		ar(width, height);
		//sf::Uint8* imgData = new sf::Uint8[sizeof(sf::Uint8) * width * height * 4];
		//ar.loadBinaryValue(imgData, sizeof(sf::Uint8) * width * height * 4, "tex_used_tiles");
		//ResourceManager::UpdateRenderTexture(imgData, width, height, "tex_used_tiles");

		//ar( map_fg_ );
		//ar( map_pg_ );
		//ar( map_bg_ );
	}

	std::map<std::string, std::unique_ptr<Camera>> 					e_cameras_;					/**< All camera entities used in the scene. */
	std::map<std::string, std::unique_ptr<Sprite>> 					e_sprites_;					/**< All sprite entities used in the scene. */
	std::map<std::string, std::unique_ptr<Solid>> 					e_solids_;					/**< All primitives used in the scene. */
	std::map<std::string, std::unique_ptr<CoordinateSystem>> 		e_cs_;						/**< All Coordinates systems used in the scene. */
	std::map<std::string, std::unique_ptr<Grid>> 					e_grids_;					/**< All grids used in the scene. */
	std::map<std::string, std::unique_ptr<Object>> 					e_objects_;					/**< All moving and interactable objects used in the scene. */
	//std::map<std::string, std::vector<std::vector<std::string>>> 	e_level_;					/**< Store the hashes of the drawn sprites in each layer used. */
	std::map<std::string, std::unique_ptr<LevelLayer>> 				e_level_layers_;			/**< Store the hashes of the drawn sprites in each layer used. */
	Player											e_player_;
	GLuint											width_;			/**< Height of the map in pixels. */
	GLuint											height_;			/**< Height of the map in pixels. */
	std::string     								active_tilemap_name_;		/**< Name of the tilemap which is currently displayed. */
	std::string     								active_sprite_name_;		/**< Name (key) of the sprite which is currently selected. */
	//std::vector<sf::Vertex> 						grid_;
	GLboolean										add_sprite_flag_;
	GLboolean										mouse_over_scene_;
	GLboolean										mouse_over_map_;
	GLboolean										map_is_null_;
	GLuint											map_width_;					/**< Width of the map. */
	GLuint											map_height_;				/**< Height of the map. */
	layer_t											active_layer_;
	glm::vec2										sprite_size_;
	glm::vec2										sprite_scale_;
	//sf::VertexArray									map_bg_vao_;
	//std::vector<std::vector<GLuint>>				map_bg_;
	//sf::VertexArray									map_pg_vao_;
	//std::vector<std::vector<GLuint>>				map_pg_;
	//sf::VertexArray									map_fg_vao_;
	//std::vector<std::vector<GLuint>>				map_fg_;
	glm::vec3						mouse_ray_start_;
	glm::vec3						mouse_ray_end_;
	glm::vec2						mouse_position_;
	GLuint							vao_;
	GLuint											current_tile_id_;

};
CEREAL_CLASS_VERSION(Scene, 1);

#endif
