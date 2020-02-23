#ifndef CAMERA_H
#define CAMERA_H

#include "scene_entity.h"


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera : public SceneEntity
{
public:

    // Constructor with vectors
    Camera()
    {
		    init();
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
	void init()
	{

	}

};
#endif
