#include "PostProcessor.h"

void PostProcessor::init(const char* vertShader, const char* fragShader, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	PostProcShader = LoadShaders(vertShader, fragShader);

	//The texture we are going to render to
	renderTextureID = createTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
	//The depth buffer
	depthBufferID;
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
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
