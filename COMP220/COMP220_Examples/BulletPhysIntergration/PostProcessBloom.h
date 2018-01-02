#pragma once
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "glm/glm.hpp"
#include "Texture.h"
#include "ShaderLoader.h"

class PostProcessBloom
{
public:
	void renderLuminance();

	void PostProcBloomInit(const char* vertShader, int SCREEN_WIDTH, int SCREEN_HEIGHT);

	void bind1stBuff() { glBindFramebuffer(GL_FRAMEBUFFER, firstFrameBufferID); };
	void bind2ndBuff() { glBindFramebuffer(GL_FRAMEBUFFER, secondFrameBufferID); };
	void bind3rdBuff() { glBindFramebuffer(GL_FRAMEBUFFER, thirdFrameBufferID); };
	void unBindBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); };

	void applyBloom();
	void secondPass();
	~PostProcessBloom();
	float resolution = 0;
	float radius = 0;

private:
	void generateBuffers(GLuint & depthBuffID, GLuint  & frameBuffID, GLuint & texture, int w, int h);

	GLuint screenQuadVBOID;
	GLuint screenVAO;

	GLuint secondResolutionLoc;
	GLuint secondRadiusLoc;
	GLuint thirdResolutionLoc;
	GLuint thirdRadiusLoc;

	GLuint firstDepthBufferID;
	GLuint firstFrameBufferID;

	GLuint secondFrameBufferID;
	GLuint secondDepthBufferID;

	GLuint thirdFrameBufferID;
	GLuint thirdDepthBufferID;

	GLuint bloomShader1;
	GLuint bloomShader2;
	GLuint bloomShader3;

	GLuint firstTextureLoc0;
	GLuint secondTextureLoc0;
	GLuint thirdTextureLoc1;
	GLuint thirdTextureLoc0;

	GLuint luminanceTextureID;
	GLuint verticalTextureID;
	GLuint sceneTextureID;
};
