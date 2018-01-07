#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <GL\glew.h>
#include <SDL_opengl.h>

using namespace std;

//http://gameprogrammingpatterns.com/contents.html
class ShaderManager
{
public:

	/**
	*Default constructor for the shader manager class
	*/
	ShaderManager();

	~ShaderManager();

	/**Loads texture from a file
	*filename is the name of the file to load the texture from
	*/
	GLuint loadShader(const char * vertex_file_path, const char * fragment_file_path, const string & shaderName);

	/**Gets the GLuint assosciated with the input string, when the texture is not loaded it tries to load it.
	*SearchString is the string key used to find the GLuint
	*/
	GLuint getShader(const string & SearchString);

private:

	/**
	*itterates through the shaderMap and calls gldeleteshader for all elements in the map
	*/
	void ShaderManagerDelete();

	const string & defualtShaderName = "defualt";

	string createShortenedName(const char * vertex_file_path, const char * fragment_file_path);
	//textureMap
	map<const string, GLuint> shaderMap;
};
