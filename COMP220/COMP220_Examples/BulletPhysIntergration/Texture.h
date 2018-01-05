#pragma once


#include <GL\glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <string>
#include <map>
#include <vector>

using namespace std;

//http://gameprogrammingpatterns.com/contents.html
class TextureManager
{
public:
	/**Loads texture from a file
	*filename is the name of the file to load the texture from
	*/
	void loadTexture(const string& filename);

	GLuint loadSkyboxTexture(vector<const char* > faces);

	/**Gets the texture map reference
	/// TO DO**** return the texture not the map if its not there load it.
	*/
	map<const string , GLuint> & getTextureMap() { return textureMap; };

private:

	//textureMap
	map<const string , GLuint> textureMap;
};


/**Creates a texture of given width and height
*width the width of the texture to be created
*height the height of the texture to be created
*/
GLuint createTexture(int width = 800, int height = 600);