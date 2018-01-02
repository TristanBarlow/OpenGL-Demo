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
	void init(const char* vertShader, const char* fragShader, int width = 800, int height =600, bool isOutline = false);
	void bindBuff();
	/*
	*If only one post process being called make sure to call PostProcessor.unBindBuff();
	*/
	void unbindBuff();
	void drawTexture();
	GLuint getFrameBuffer() { return frameBufferID; };
	void setInputTexture(GLuint&);
	~PostProcessor();
	GLuint renderTextureID;
	GLuint screenVAO;
	GLuint screenQuadVBOID;
	GLuint PostProcTexLoc;
private:
	GLuint PostProcShader;
	bool outline;
	glm::vec3 outLineColour = glm::vec3(0.5f,0.2f,0.5f);
	GLuint postProcColourLoc;
	GLuint frameBufferID;
	GLuint depthBufferID;
	

};


