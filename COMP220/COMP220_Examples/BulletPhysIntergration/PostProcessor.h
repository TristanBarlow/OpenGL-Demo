#pragma once

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "glm/glm.hpp"
#include "Texture.h"
#include "ShaderLoader.h"



using namespace std;

class PostProcessor
{
public:
	void init(const char*, const char*, int = 800, int =600, bool = false);
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
	bool outline;
	glm::vec3 outLineColour = glm::vec3(0.5f,0.2f,0.5f);
	GLuint postProcColourLoc;
	GLuint depthBufferID;
	GLuint frameBufferID;
	GLuint PostProcTexLoc;
	GLuint screenVAO;
	GLuint screenQuadVBOID;
};