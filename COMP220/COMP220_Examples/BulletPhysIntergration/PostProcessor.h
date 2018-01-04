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
	/** initialises the postprocessor class, generating buffers and loading shaders 
	*vertShader the vertex shader used in the shader program to load
	*fragSgader the fragment shader used in the shader program to load
	*width if the width of the texture to be created
	*height is the height of the texture to be created

	*/
	void init(const char* vertShader, const char* fragShader, int width = 800, int height =600);

	/**
	*Draws the texture that has been stored in the frame buffer applies post processing effect at the same time
	*/
	void drawTexture();

	/**
	*Binds the frame buffer
	*/
	void bindBuff() { glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID); };

	/**
	*If only one post process being called make sure to call PostProcessor.unBindBuff();
	*/
	void unbindBuff() { glBindFramebuffer(GL_FRAMEBUFFER, 0); };

	~PostProcessor();

private:
	GLuint PostProcShader;
	GLuint postProcColourLoc;

	GLuint screenVAO;
	GLuint screenQuadVBOID;

	GLuint frameBufferID;
	GLuint depthBufferID;
	
	GLuint renderTextureID;
	GLuint PostProcTexLoc;
};


