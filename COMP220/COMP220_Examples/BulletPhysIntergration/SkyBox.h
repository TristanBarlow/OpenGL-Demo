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
	/**Defualt constructor of the skybox class
	*cam is a reference to the camera, used for the initialisaion of the MVPTransform class
	*/
	SkyBox(Camera& cam);

	/**Initialisation of the skybox class, without this it will not work
	*mesh is a reference to the mesh to render 
	* shaderProgram is the shader program to use when renderign the textured mesh
	* texture is the texture of the skybox 
	*/
	void init(Mesh & mesh, GLuint shaderProgram, GLuint texture);

	/**
	*does the actuall drawing of the skybox, without this the mesh will not be rendered. 
	*/
	void render();

private :

	//pointer to the mesh used when rendering the skybox
	Mesh * s_mesh = NULL;

	Camera & s_camera;

	MVPTransform s_MVPTransform;

	MVP s_MVPLoc;
	Transform s_transform;

	//shader program to use in the rendering
	GLuint s_shaderProgram;
	
	//uniform of the skyboxtexture
	GLuint s_skyboxUniform;

	//texture ID
	GLuint s_texture;

	//VBO and VAO used in the rendering of the skybox
	GLuint skyboxVAO, skyboxVBO;
};			 