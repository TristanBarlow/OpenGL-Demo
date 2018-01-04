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

	/**Initialises the bloom class, setting up uniforms and generating GLuints
	*vertShader the name of the vertex shader used in all three boom shader passes
	*numberOfBlurs is the number of times the guassian blur should be applied to the luminance texture. Higher equals smoother distrubution but more expensive
	*SCREEN_WIDTH screen width used to create the size of the textures
	*SCREEN_HEIGHT screen height used to create the size of the textures
	*/
	void PostProcBloomInit(const char* vertShader,int numberOfBlurs, int SCREEN_WIDTH, int SCREEN_HEIGHT);

	/**
	*applies the bloom by calling the other functions in their correct order
	*/
	void applyBloom();

	//Buffer binders
	void bind1stBuff() { glBindFramebuffer(GL_FRAMEBUFFER, firstFrameBufferID); };
	void bind2ndBuff() { glBindFramebuffer(GL_FRAMEBUFFER, secondFrameBufferID); };
	void bind3rdBuff() { glBindFramebuffer(GL_FRAMEBUFFER, thirdFrameBufferID); };
	void bind4thBuff() { glBindFramebuffer(GL_FRAMEBUFFER, fourthFrameBufferID); };
	void unBindBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); };

	/**
	*Deconstructor
	*/
	~PostProcessBloom();

private:
	//number of times to applie the increase blur function
	int timesToBlur = 2;

	//radius of the blur and the resolution the blur will be at
	float resolution = 0;
	float radius = 0;

	void blurTexture( GLuint Texture);

	/**
	*The first blur call, this uses just the raw luminance texture, so cannont be repeated for increasing blur amount
	*/
	void firstBlur();

	/**
	*This functions further applies gaussian blur once horizontally once vertically. can be called multiple times
	*/
	void increaseBlur();

	/**
	*This fucntion uses the last combinedTexture and renders it to screen adding it to the scene texture which results in the bloom effect
	*/
	void finalDraw();

	/**This function generates a framebuffer and attaches a texture to the said frame
	*frameBuffID the frameID of which to generate the buffer into
	*texture the texture that will be attahced to the frambuffer
	*/
	void generateBuffers(GLuint  & frameBuffID, GLuint & texture);

	/**
	*Reneders the scene texture removing anything with low lumninance
	*/
	void renderLuminance();

	//direction to blur (1,0) means horizontally (0,1) vertically
	vec2 direction;
	
	//Direction locaitons
	GLuint directionLoc;

	GLuint screenQuadVBOID;
	GLuint screenVAO;

	//resolution and radius locations
	GLuint secondResolutionLoc;
	GLuint secondRadiusLoc;
	GLuint thirdResolutionLoc;
	GLuint thirdRadiusLoc;

	//depth buffer ID
	GLuint depthBufferID;

	//frame buffers
	GLuint firstFrameBufferID;
	GLuint secondFrameBufferID;
	GLuint thirdFrameBufferID;
	GLuint fourthFrameBufferID;

	//shaders
	GLuint bloomShader1;
	GLuint bloomShader2;
	GLuint bloomShader3;

	//texture locations
	GLuint firstTextureLoc0;
	GLuint secondTextureLoc0;
	GLuint thirdTextureLoc1;
	GLuint thirdTextureLoc0;

	//Texture IDs
	GLuint luminanceTextureID;
	GLuint verticalTextureID;
	GLuint sceneTextureID;
	GLuint combinedTextureID;
};
