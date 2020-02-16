#ifndef SCENE_H
#define SCENE_H

#include "../include/camera.h"

#include <SFML/OpenGL.hpp>

class Scene
{
public:
	GLuint						Width, Height;
	Camera						CameraObj;

	Scene(GLuint width, GLuint height);
	~Scene();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	void Update(GLuint width, GLuint height);
	void Render(GLuint tex);

	// Getters
	Camera& GetCamera() { return this->CameraObj; };

private:;

};

#endif
