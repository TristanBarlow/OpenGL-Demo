#pragma once


#include <GL\glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <string>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

//http://gameprogrammingpatterns.com/contents.html
class TextureManager
{
public:

	~TextureManager();

	/**Loads texture from a file
	*filename is the name of the file to load the texture from
	*/
	GLuint loadTexture(const string& filename);

	/**LoadsSkybox texture and returns the GLuint that referes to the texture
	*faces is the list of faces that make up the the skybox texture
	*/
	GLuint loadSkyboxTexture(vector<const char* > faces);

	/**Gets the GLuint assosciated with the input string, when the texture is not loaded it tries to load it.
	*SearchString is the string key used to find the GLuint 
	*/
	GLuint getTexture(const string& SearchString);

private:
	
	/**
	*itterates through the textureMap and calls glDeleteTextures for all textures in the map
	*/
	void TextureManagerDelete();

	//textureMap
	map<const string , GLuint> textureMap;
};


/**Creates a texture of given width and height
*width the width of the texture to be created
*height the height of the texture to be created
*/
GLuint createTexture(int width = 800, int height = 600);