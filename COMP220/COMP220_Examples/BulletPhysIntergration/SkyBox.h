#pragma once

#include "Mesh.h"
#include "MVP.h"
#include "MVPTransform.h"

/*
*Needs to be implemented
*/

class SkyBox
{
public:
	SkyBox(Camera& cam);
	void init(Mesh & mesh, GLuint shaderProgram, GLuint texture);
	void render();

private :
	Mesh * s_mesh = NULL;

	Camera & s_camera;

	MVPTransform s_MVPTransform;

	MVP s_MVPLoc;
	Transform s_transform;

	GLuint s_shaderProgram;
	GLuint s_skyboxUniform;
	GLuint s_texture;
	GLuint skyboxVAO, skyboxVBO;
};			 