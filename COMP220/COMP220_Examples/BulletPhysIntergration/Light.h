#pragma once


#include "WorldObject.h"
#include "Camera.h"

/*
*Light inherits from WorldObject
*/
class Light: public WorldObject
{
public:


	/* Defualt constructor for the light class
	* camera argument used for the initialiser of the mesh class
	*/
	Light(Camera & cam);

	void moveCircle();

	float intensity = 2.0f;
private:
		
		int radius = 4;
		double angle = 0.1;
};