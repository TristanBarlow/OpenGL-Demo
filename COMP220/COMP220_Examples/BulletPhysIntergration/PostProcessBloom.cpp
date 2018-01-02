#include "PostProcessBloom.h"

void PostProcessBloom::renderLuminance()
{
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Bind our Postprocessing Program
	glUseProgram(bloomShader1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sceneTextureID);
	glUniform1i(firstTextureLoc0, 0);

	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void PostProcessBloom::PostProcBloomInit(const char* vertShader, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	bloomShader1 = LoadShaders(vertShader, "Shaders/PostProcBloomFragPass1.txt");
	bloomShader2 = LoadShaders(vertShader, "Shaders/PostProcBloomFragPass2.txt");
	bloomShader3 = LoadShaders(vertShader, "Shaders/PostProcBloomFragPass3.txt");

	resolution = 2048;
	radius = 10;

	//The texture we are going to render to
	sceneTextureID = createTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

	generateBuffers(firstDepthBufferID, firstFrameBufferID, sceneTextureID, SCREEN_WIDTH, SCREEN_WIDTH);

	//The texture we are going to render to
	luminanceTextureID = createTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

	generateBuffers(secondDepthBufferID, secondFrameBufferID, luminanceTextureID, SCREEN_WIDTH, SCREEN_HEIGHT);

	//third buffer
	verticalTextureID = createTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

	generateBuffers(thirdDepthBufferID, thirdFrameBufferID, verticalTextureID, SCREEN_WIDTH, SCREEN_HEIGHT);


	//create fullscreen quad
	GLfloat vertices[8] = { -1.0f, -1.0f, 1.0f, -1.0f,-1.0f, 1.0f, 1.0f, 1.0f };

	screenQuadVBOID;
	glGenBuffers(1, &screenQuadVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	screenVAO;
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	// do uniforms here!!

	firstTextureLoc0 = glGetUniformLocation(bloomShader1, "texture0");

	secondTextureLoc0 = glGetUniformLocation(bloomShader2, "texture0");
	secondResolutionLoc = glGetUniformLocation(bloomShader2, "resolution");
	secondRadiusLoc = glGetUniformLocation(bloomShader2, "radius");

	thirdTextureLoc0 = glGetUniformLocation(bloomShader3, "texture0");
	thirdTextureLoc1 = glGetUniformLocation(bloomShader3, "texture1");
	thirdResolutionLoc = glGetUniformLocation(bloomShader3, "resolution");
	thirdRadiusLoc = glGetUniformLocation(bloomShader3, "radius");
	unBindBuffer();
}

void PostProcessBloom::applyBloom()
{
	bind2ndBuff();
	renderLuminance();
	secondPass();
}

void PostProcessBloom::secondPass()
{
	bind3rdBuff();

	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(bloomShader2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, luminanceTextureID);

	glUniform1i(secondTextureLoc0, 0);
	glUniform1f(secondRadiusLoc, radius);
	glUniform1f(secondResolutionLoc, resolution);

	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	unBindBuffer();

	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Bind our Postprocessing Program
	glUseProgram(bloomShader3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sceneTextureID);
	glUniform1i(thirdTextureLoc0, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, verticalTextureID);
	glUniform1i(thirdTextureLoc1, 1);

	glUniform1f(thirdRadiusLoc, radius);
	glUniform1f(thirdResolutionLoc, resolution);

	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	unBindBuffer();
}

PostProcessBloom::~PostProcessBloom()
{

	glDeleteBuffers(1, &screenQuadVBOID);
	glDeleteVertexArrays(1, &screenVAO);
	glDisableVertexAttribArray(0);

	glDeleteProgram(bloomShader1);
	glDeleteProgram(bloomShader2);
	glDeleteProgram(bloomShader3);

	glDeleteTextures(1, &luminanceTextureID);
	glDeleteTextures(1, &verticalTextureID);
	glDeleteTextures(1, &sceneTextureID);

	glDeleteRenderbuffers(1, &firstDepthBufferID);
	glDeleteFramebuffers(1, &firstFrameBufferID);

	glDeleteRenderbuffers(1, &secondDepthBufferID);
	glDeleteFramebuffers(1, &secondFrameBufferID);

	glDeleteRenderbuffers(1, &thirdDepthBufferID);
	glDeleteFramebuffers(1, &thirdFrameBufferID);
}
void PostProcessBloom::generateBuffers(GLuint & depthBuffID, GLuint  & frameBuffID, GLuint & texture, int w, int h)
{

	glGenRenderbuffers(1, &depthBuffID);
	//Bind the depth buffer
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffID);
	//Set the format of the depth buffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);

	//The frambuffer
	frameBuffID;
	glGenFramebuffers(1, &frameBuffID);

	//Bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffID);

	//Bind the depth buffer as a depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffID);
	//Bind the texture as a colour attachment 0 to the active framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "checkFrameBufferStatus fail:" << endl;
	}

}