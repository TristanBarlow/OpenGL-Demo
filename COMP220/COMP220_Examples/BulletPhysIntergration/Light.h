#pragma once


#include "WorldObject.h"

/*
*Light inherits from WorldObject
*/
class Light: public WorldObject
{
public:

	/* Defualt constructor for the light class
	* camera argument used for the initialiser of the mesh class
	*/
	void moveCircle();

	float intensity = 2.0f;
private:
		
		int radius = 4;
		double angle = 0.1;
};