#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
#include "Vertex.h"
#include "Camera.h"
#include "ObjLoader.h"
#include "Transform.h"
#include "ShaderLoader.h"
#include "MVP.h"
#include "Grid.h"

using namespace std;
using namespace glm;

SDL_Window* window;

SDL_GLContext glContext;

int SCREEN_WIDTH = 1200;
int SCREEN_HEIGHT = 800;
float aspectRatio = (SCREEN_WIDTH / SCREEN_HEIGHT);


struct Mesh 
{
	vector<vertex> vertArray;
	vector<int> elementBuff;
};
Camera camera;
class Sphere
{
private:
	Transform MVPMatrix;
	MVP MVPLoc;
	Mesh objMesh;
	vector<vertex> vertarray;
	vector<int> elemtarray;
	
public:
	vec3 worldScale;
	vec3 worldRot;
	vec3 worldPos;
	void begin(vec3, vec3, vec3, Mesh&);
	void draw(GLuint vertexbuffer, GLuint ebo, GLuint programID)
	{
		
		MVPLoc = { glGetUniformLocation(programID, "modelMatrix"),
			glGetUniformLocation(programID, "viewMatrix"),
			glGetUniformLocation(programID, "projectionMatrix") };

		MVPMatrix = calculateTransform(camera,aspectRatio, worldPos, worldRot, worldScale);

		glUniformMatrix4fv(MVPLoc.modelMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.modelMatrix));
		glUniformMatrix4fv(MVPLoc.viewMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.viewMatrix));
		glUniformMatrix4fv(MVPLoc.projectionMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.projectionMatrix));

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertarray.size() * sizeof(vertex), &vertarray[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemtarray.size() * sizeof(int), &elemtarray[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), ((void*)offsetof(vertex, vertexCol)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), ((void*)offsetof(vertex, textureCoords)));
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, vertarray.size());
		glDrawElements(GL_TRIANGLES, elemtarray.size(), GL_UNSIGNED_INT, 0);
	};

};
void Sphere::begin(vec3 inWorldPos, vec3 inWorldRot, vec3 inWorldScale, Mesh &mesh)
{
	worldPos = inWorldPos;
	worldRot = inWorldRot;
	worldScale= inWorldScale;
	objMesh = mesh;
	vertarray = mesh.vertArray;
	elemtarray = mesh.elementBuff;
	MVPMatrix = calculateTransform(camera,aspectRatio, worldPos, worldRot, worldScale);
};


// initialises modules
bool Init();

//Deallocates memory and calls SDL closing functions
void Close();
