#pragma once
#include <GL\glew.h>
#include "MVPTransform.h"

struct MVP
{
	/**Gets the MVP locations for the model matrix
	*programToUse the shader program to get the variables from
	*/
	void getMVPuniforms(GLuint programToUse) 
	{
		modelMatrixLocation = glGetUniformLocation(programToUse, "modelMatrix");
		viewMatrixLocation = glGetUniformLocation(programToUse, "viewMatrix");
		projectionMatrixLocation = glGetUniformLocation(programToUse, "projectionMatrix");
	};

	/**
	* Sends uniform variables to the stored locations in the struct
	*/
	void sendMVPuniforms(MVPTransform& MVPToSend)
	{
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(MVPToSend.modelMatrix));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(MVPToSend.viewMatrix));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(MVPToSend.projectionMatrix));
	};

	GLint modelMatrixLocation;
	GLint viewMatrixLocation;
	GLint projectionMatrixLocation;
};