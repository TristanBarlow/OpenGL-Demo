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
