#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

SDL_Window* window;

SDL_GLContext glContext;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

struct vertex
{
	vec3 vertexPos;
	vec4 vertexCol;
};

struct Transform
{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

class Camera
{
public:
	vec3 worldPos = vec3(2.0f, 0.0f, -20.0f);//pos of the camera
	vec3 centre = vec3(0.0f, 0.0f, 0.0f); //point the camera looks at
	vec3 up = vec3(0.0f, 1.0f, 0.0f);//the up direction of the camera(where is directly above of the camera)
	vec3 forward = normalize(centre - worldPos);
	vec3 right = cross(up, forward);
	vec3 centreMag = (centre - worldPos);
	void strafe(float x)
	{
		vec3 strafeDirection = right*x;
		worldPos.x += strafeDirection.x;
		worldPos.z += strafeDirection.z;
		centre.x += strafeDirection.x;
		centre.z += strafeDirection.z;
	};

	void move(float z)
	{
		vec3 moveDirection = forward*z;
		worldPos.x += moveDirection.x;
		worldPos.z += moveDirection.z;
		centre.z += moveDirection.z;
		centre.z += moveDirection.x;
	}
	void lift(float y)
	{
		vec3 liftDirection = up * y;
		worldPos.y += liftDirection.y;
		centre.y += liftDirection.y;
	}
	void rotateX(float x)
	{
		vec3 vVector = (centre - worldPos)*x;	// Get the view vector
		centre.x = vVector.x;
		centre.y = vVector.y;
		//cout << centre.y << ":" << centre.x << endl;
	}
	void rotateY(float y)
	{
		vec3 rotateDirection = forward * y;
	}
};

Camera camera;

// initialises modules
bool Init();

//Deallocates memory and calls SDL closing functions
void Close();

Transform calculateTransform(Camera*);
