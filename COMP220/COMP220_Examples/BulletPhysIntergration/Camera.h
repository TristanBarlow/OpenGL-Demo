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
	Camera(float, vec3& = vec3(40.0f, 5.0f, 40.0f), vec3& = vec3(-20.0f, 0.0f, -20.0f), vec3& = vec3(0.0f, 1.0f, 0.0f));
	vec3 worldPos = vec3(40.0f, 5.0f, 40.0f);  //pos of the camera
	vec3 centre = vec3(45.0f, 0.0f, 45.0f);   //point the camera looks at
	vec3 up = vec3(0.0f, 1.0f, 0.0f);		 //the up direction of the camera(where is directly above of the camera)
	vec3 forward = normalize(centre - worldPos);
	vec3 right = cross(up, forward);
	vec3 centreMag = (centre - worldPos);
	mat4 cameraMatrix;
	float length = centreMag.length();
	float aspectRatio;
	void strafe(float);
	void move(float);
	void lift(float);
	void rotate(float, float);
private:
	void update();
	float mouseSens = 200.0;
	float TurnDegreesFromOriginX = 90.0f;
	float TurnDegreesFromOriginY= 0.0;
};