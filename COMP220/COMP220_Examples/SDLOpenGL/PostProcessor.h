#pragma once

#include <iostream>
#include "Texture.h"
#include "ShaderLoader.h"


using namespace std;

class PostProcessor
{
public:
	void init(const char*, const char*, int = 800, int =600);
	void bindBuff();
	void unbindBuff();
	/*
	*If only one post process being called make sure to call PostProcessor.unBindBuff();
	*/
	void drawTexture();
	void setInputTexture(GLuint&);
	~PostProcessor();
	GLuint renderTextureID;
private:
	GLuint PostProcShader;
	
	GLuint depthBufferID;
	GLuint frameBufferID;
	GLuint PostProcTexLoc;
	GLuint screenVAO;
	GLuint screenQuadVBOID;
};