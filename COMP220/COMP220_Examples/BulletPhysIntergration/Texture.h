#pragma once


#include <GL\glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <string>

using namespace std;

/**Loads texture from a file
*filename is the name of the file to load the texture from 
*/
GLuint loadTexture(const string& filename);

/**Creates a texture of given width and height
*width the width of the texture to be created
*height the height of the texture to be created
*/
GLuint createTexture(int width = 800, int height = 600);