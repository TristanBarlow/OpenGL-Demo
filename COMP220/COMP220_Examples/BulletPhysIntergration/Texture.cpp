#include "Texture.h"

void TextureManager::loadTexture(const string & filename)
{
	GLuint textureID;


	GLenum	textureFormat = GL_RGB;
	GLenum	internalFormat = GL_RGB8;

	SDL_Surface * surface = IMG_Load(filename.c_str());
	if (surface == nullptr)
	{
		printf("Could not load file %s", IMG_GetError());
		return;
	}

	GLint	nOfColors = surface->format->BytesPerPixel;
	if (nOfColors == 4)
	{
		if (surface->format->Rmask == 0x000000ff) {
			textureFormat = GL_RGBA;
			internalFormat = GL_RGBA8;
		}
		else {
			textureFormat = GL_BGRA;
			internalFormat = GL_RGBA8;
		}
	}
	else if (nOfColors == 3)
	{
		if (surface->format->Rmask == 0x000000ff) {
			textureFormat = GL_RGB;
			internalFormat = GL_RGB8;
		}
		else {
			textureFormat = GL_BGR;
			internalFormat = GL_RGB8;
		}
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	SDL_FreeSurface(surface);

	textureMap.insert(pair<const string &, GLuint>(filename, textureID));
}

GLuint TextureManager::loadSkyboxTexture(vector<const char* > faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (GLuint i = 0; i < faces.size(); i++)
	{			
		SDL_Surface * surface = IMG_Load(faces[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
		SDL_FreeSurface(surface);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	//textureMap.insert(pair<const string &, GLuint>("skybox", textureID));
	return textureID;
	
}

GLuint createTexture(int width, int height)
{
	// generate texture
	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	return textureID;
}

