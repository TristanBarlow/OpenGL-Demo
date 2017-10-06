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
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

SDL_Window* window;

SDL_GLContext glContext;

struct vertex
{
	vec3 vertexPos;
	vec4 vertexCol;
};


// initialises modules
bool Init();

//Deallocates memory and calls SDL closing functions
void Close();

