#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include <SFML/System/Vector3.hpp>
#include <SFML/OpenGL.hpp>

class SceneEntity
{
public:
  SceneEntity() : position(sf::Vector3<GLfloat>(0.0f, 0.0f, 0.0f)) { };
  ~SceneEntity() { };

protected:
  sf::Vector3<GLfloat> position;

private:

};

#endif
