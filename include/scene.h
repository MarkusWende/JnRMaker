#ifndef SCENE_H
#define SCENE_H

#include <sstream>
#include <map>
#include <memory>

#include <SFML/OpenGL.hpp>
#include <SFML/System/Vector2.hpp>

#include "entity_camera.h"
#include "entity_solid.h"
#include "resource_manager.h"
#include "message_manager.h"
#include "scene_entity.h"

class Scene
{
public:
	Scene(GLuint width, GLuint height);
	~Scene();

	GLvoid Update();
	GLvoid Render();

	GLvoid SetSizeWidth(GLuint width) { width_ = width; };
	GLvoid SetSizeHeight(GLuint height) { height_ = height; };

private:
	std::map<std::string, std::unique_ptr<Camera>> e_cameras_;
	std::map<std::string, std::unique_ptr<Solid>> e_solids_;

	GLuint																					width_;
	GLuint																					height_;

};

#endif
