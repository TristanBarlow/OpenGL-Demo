#pragma once
#include<GL/glew.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <iostream>
using namespace glm;
class Camera
{
public:
	/**
	*Camera constructor
	*aspectRatio used to set the viewMatrix Not constant incase of a resizable screen in the future
	* location Spawn location of the camera defaulted
	* Centre location used to determine which direction the camera is facing defaulted
	* Up direction used to dertermine which direction Up is for the camera.
	*/
	Camera(float aspectratio, vec3& location = vec3(40.0f, 5.0f, 40.0f), vec3& centre = vec3(-20.0f, 0.0f, -20.0f), vec3& up = vec3(0.0f, 1.0f, 0.0f));

	// the forward direction vector of the camera (unit vector) 
	vec3 forward = normalize(centre - worldPos);
	// the right direction vector of the camera (unit vector) 
	vec3 right = cross(up, forward);
	// the direction from the centre point to the world pos (mainly used for debugging)
	vec3 centreMag = (centre - worldPos);

	//The cameraMatrix used in all 3d rendering in the scene Very Important variable
	mat4 cameraMatrix;
	
	// lenght of the cameraMag
	float length = centreMag.length();

	float aspectRatio;


	//camera Movement functions

	/**
	*Moves the camera to the left or the right
	* X the amount to strafe the camera by
	*/
	void strafe(float x);

	/**
	*moves the camera along the forward direction vector
	* Z the amount to move the camera by
	*/
	void move  (float z);

	/**
	*moves the camera along in the up direction vector
	* Y is the amount to move the camera by
	*/
	void lift  (float y);

	/**Takes an X and a Y and rotates the camera around to get a new centre point to look at THANKS RICH!
	* X the X offset
	* Y the Y offset
	*/
	void rotate(float x, float y);

	//Setters and getters!

	/**Sets the private variable WorldPos to the argument
	* newWorldPos is the new position to set to
	*/
	void setWorldPos(vec3& newWorldPos) { worldPos = newWorldPos; };

	/**gets the private variable WorldPos
	* returns a reference to the worldPos
	*/
	vec3& getWorldPos() { return worldPos; };

	/**Sets the private variable Centre to the argument
	* newCentre is the new position to set to
	*/
	void setCentre(vec3& newCentre) { centre = newCentre; };

	/*gets the private variable centre
	* returns a reference to the centre
	*/
	vec3& getCentre() { return centre; };

	/**Sets the private variable Centre to the argument
	* newCentre is the new position to set to
	*/
	void setUp(vec3& newUp) { up = newUp; };

	/**gets the private variable up
	* returns a reference to the up
	*/
	vec3& getUp() { return up; };

private:

	//worldPosition of the camera
	vec3 worldPos = vec3(40.0f, 5.0f, 40.0f);
	//point the camera looks at
	vec3 centre = vec3(45.0f, 0.0f, 45.0f);
	//the up direction of the camera(where is directly above of the camera)
	vec3 up = vec3(0.0f, 1.0f, 0.0f);

	/**
	* Recalculates camera values
	*/
	void update();
	
	// used in the rotate function increase for more mouse sensitivity
	float mouseSens = 200.0;
	
	//used to track origin and calculate new rotation of the camera (mouse input) THANKS RICH!
	float TurnDegreesFromOriginX = 90.0f;
	float TurnDegreesFromOriginY= 0.0;
};