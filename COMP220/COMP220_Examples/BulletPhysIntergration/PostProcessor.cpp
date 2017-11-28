#include "PostProcessor.h"

void PostProcessor::init(const char* vertShader, const char* fragShader, int SCREEN_WIDTH, int SCREEN_HEIGHT, bool isOutline)
{
	PostProcShader = LoadShaders(vertShader, fragShader);

	//The texture we are going to render to
	renderTextureID = createTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

	glGenRenderbuffers(1, &depthBufferID);
	//Bind the depth buffer
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
	//Set the format of the depth buffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, SCREEN_WIDTH, SCREEN_HEIGHT);

	//The frambuffer
	frameBufferID;
	glGenFramebuffers(1, &frameBufferID);

	//Bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

	//Bind the depth buffer as a depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
	//Bind the texture as a colour attachment 0 to the active framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTextureID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "checkFrameBufferStatus fail:" << endl;
	}



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

	PostProcTexLoc = glGetUniformLocation(PostProcShader, "texture0");
	if (isOutline)
	{
		postProcColourLoc = glGetUniformLocation(PostProcShader, "outlineColour");
		outline = true;
	}
	unbindBuff();
}

void PostProcessor::bindBuff()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
}

void PostProcessor::unbindBuff()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::drawTexture()
{
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Bind our Postprocessing Program
	glUseProgram(PostProcShader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTextureID);
	glUniform1i(PostProcTexLoc, 0);

	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	//Send across any values to the shader
	if (outline)
	{
		glUniform3fv(postProcColourLoc, 1, value_ptr(outLineColour));
	}

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint PostProcessor::getFrameBuffer()
{
	return frameBufferID;
}

void PostProcessor::setInputTexture(GLuint& overrideTexture)
{
	renderTextureID = overrideTexture;
}

PostProcessor::~PostProcessor()
{	
	glDeleteProgram(PostProcShader);
	glDeleteTextures(1, &renderTextureID);
	glDeleteRenderbuffers(1, &depthBufferID);
	glDeleteFramebuffers(1, &frameBufferID);
	glDeleteBuffers(1, &screenQuadVBOID);
	glDeleteVertexArrays(1, &screenVAO);
	glDisableVertexAttribArray(0);
}

void PostProcBloom::PostProcBloomInit(const char* vertShader, const char* fragShader, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	bloomShader = LoadShaders(vertShader, fragShader);
	
	resolution = 2048;
	radius = 3;

	//The texture we are going to render to
	luminanceTextureID = createTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
	//The frambuffer
	glGenFramebuffers(1, &secondFrameBufferID);

	glGenRenderbuffers(1, &secondDepthBufferID);
	//Bind the depth buffer
	glBindRenderbuffer(GL_RENDERBUFFER, secondDepthBufferID);
	//Set the format of the depth buffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, secondFrameBufferID);
	//Bind the depth buffer as a depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, secondDepthBufferID);
	//Bind the texture as a colour attachment 0 to the active framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, luminanceTextureID, 0);

	// do uniforms here!!
	secondPostProcTexLoc0 = glGetUniformLocation(bloomShader, "texture0");
	secondPostProcTexLoc11 = glGetUniformLocation(bloomShader, "texture1");
	secondresolutionLoc = glGetUniformLocation(bloomShader, "resolution");
	secondradiusLoc = glGetUniformLocation(bloomShader, "radius");

	unbindBuff();
}

void PostProcBloom::bind1stBuff()
{
	glBindFramebuffer(GL_FRAMEBUFFER, getFrameBuffer());
}

void PostProcBloom::bind2ndBuff()
{
	glBindFramebuffer(GL_FRAMEBUFFER, secondFrameBufferID);
}

void PostProcBloom::applyBloom()
{
	bind2ndBuff();
	drawTexture();
	unbindBuff();
	secondPass();
}

void PostProcBloom::secondPass()
{ 
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Bind our Postprocessing Program
	glUseProgram(bloomShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTextureID);
	glUniform1i(secondPostProcTexLoc0, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, luminanceTextureID);
	glUniform1i(secondPostProcTexLoc11, 1);

	glUniform1f(secondradiusLoc, radius);
	glUniform1f(secondresolutionLoc, resolution);

	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PostProcBloom::~PostProcBloom()
{
	glDeleteProgram(bloomShader);
	glDeleteTextures(1, &luminanceTextureID);
	glDeleteRenderbuffers(1, &secondDepthBufferID);
	glDeleteFramebuffers(1, &secondFrameBufferID);
}
