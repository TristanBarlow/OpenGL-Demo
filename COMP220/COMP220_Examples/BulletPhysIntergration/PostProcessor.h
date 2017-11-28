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
	GLuint getFrameBuffer();
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

class PostProcBloom: public PostProcessor
{
public:
	void PostProcBloomInit(const char* vertShader, const char* fragShader, int SCREEN_WIDTH, int SCREEN_HEIGHT);
	GLuint luminanceTextureID;
	void bind1stBuff();
	void bind2ndBuff();
	void applyBloom();
	void secondPass();
	~PostProcBloom();
	float resolution = 0;
	float radius = 0;

private:
	GLuint secondresolutionLoc;
	GLuint secondradiusLoc;
	GLuint secondFrameBufferID;
	GLuint secondDepthBufferID;
	GLuint bloomShader;
	GLuint secondPostProcTexLoc0;
	GLuint secondPostProcTexLoc11;
};
