#pragma once
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "glm/glm.hpp"
#include "Texture.h"
#include "ShaderLoader.h"

using namespace glm;

class PostProcessBloom
{
public:
	void renderLuminance();

	void PostProcBloomInit(const char* vertShader,int numberOfBlurs, int SCREEN_WIDTH, int SCREEN_HEIGHT);

	void bind1stBuff() { glBindFramebuffer(GL_FRAMEBUFFER, firstFrameBufferID); };
	void bind2ndBuff() { glBindFramebuffer(GL_FRAMEBUFFER, secondFrameBufferID); };
	void bind3rdBuff() { glBindFramebuffer(GL_FRAMEBUFFER, thirdFrameBufferID); };
	void bind4thBuff() { glBindFramebuffer(GL_FRAMEBUFFER, fourthFrameBufferID); };
	void unBindBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); };

	void applyBloom();

	~PostProcessBloom();
	float resolution = 0;
	float radius = 0;

private:
	int timesToBlur = 2;

	void blurTexture( GLuint Texture);

	void firstBlur();

	void AnotherOne();

	void finalDraw();

	void generateBuffers(GLuint & depthBuffID, GLuint  & frameBuffID, GLuint & texture, int w, int h);

	vec2 direction;
	GLuint directionLoc;

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

	GLuint fourthFrameBufferID;
	GLuint fourthDepthBufferID;

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
	GLuint combinedTextureID;
};
