#pragma once


#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <string>
#include <vector>

#include <GL\glew.h>
#include <SDL_opengl.h>

#include "Vertex.h"
#include "Camera.h"
#include "MVP.h"
#include "Transform.h"
#include "subMesh.h"

class Mesh
{
public:
	float aspectRatio = 4/3;
	vec3 worldPos =vec3(0.0f, 0.0f, 0.0f);
	vec3 worldRot = vec3(0.0f, 0.0f, 0.0f);
	vec3 worldScale = vec3(1.0f, 1.0f, 1.0f);
	void init(const std::string&, GLuint);
	void render(Camera&);
	void movement(float);
	std::vector<subMesh*> subMeshes;
	void copyBufferData();
	
private:
	Transform MVPMatrix;
	MVP MVPLoc;
	GLuint programToUse = 0;
	GLint lightDirectionLoc;
	GLfloat directionFromLightSource[3];
	vec3 lightSource = vec3(20.0f,10.0,20.0);
	vec3 tempLightDir;
};

bool loadMeshFromFile(const std::string&, std::vector<subMesh*>&);