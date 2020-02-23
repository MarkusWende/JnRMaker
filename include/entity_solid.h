#ifndef SOLID_H
#define SOLID_H

#include "scene_entity.h"


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Solid : public SceneEntity
{
public:

    // Constructor with vectors
    Solid()
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
