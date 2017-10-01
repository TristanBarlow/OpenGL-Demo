#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

SDL_Window* window;

SDL_GLContext glContext;

// initialises modules
bool Init();

//Deallocates memory and calls SDL closing functions
void Close();