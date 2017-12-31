#pragma once


#include "WorldObject.h"
#include "Camera.h"

/**
*Light inherits from WorldObject
*/
class Light: public WorldObject
{
public:


	/** Defualt constructor for the light class
	* camera argument used for the initialiser of the mesh class
	*/
	Light(Camera & cam);

	/**
	*Does what it says on the tin and moves the light in a circle.
	*/
	void moveCircle();

	/**Sets the new light intensity
	*the new intensity of the light.
	*/
	void setLightIntensity(const float newIntensity) { intensity = newIntensity; };
private:
		float intensity = 2.0f;
		int radius = 4;
		double angle = 0.1;
};