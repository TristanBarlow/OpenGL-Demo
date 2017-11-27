#pragma once


#include <GL\glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <string>

using namespace std;

GLuint loadTexture(const string& filename);

GLuint createTexture(int= 800, int = 600);