#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

SDL_Window* window;

SDL_GLContext glContext;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

struct vertex
{
	vec3 vertexPos;
	vec4 vertexCol;
};

struct Transform
{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
};
struct MVP
{
	GLint modelMatrixLocation;
	GLint viewMatrixLocation;
	GLint projectionMatrixLocation;
};
class Camera
{
public:
	vec3 worldPos = vec3(2.0f, 0.0f, -20.0f);//pos of the camera
	vec3 centre = vec3(0.0f, 0.0f, 0.0f); //point the camera looks at
	vec3 up = vec3(0.0f, 1.0f, 0.0f);//the up direction of the camera(where is directly above of the camera)
	vec3 forward = normalize(centre - worldPos);
	vec3 right = cross(up, forward);
	vec3 centreMag = (centre - worldPos);

	float length = centreMag.length();
	float rotatedX = 0;
	float rotatedY = 0;
	float rotatedZ = 0;
	void strafe(float x)
	{
		vec3 forward = normalize(centre - worldPos);
		vec3 strafeDirection = right*x;
		worldPos.x += strafeDirection.x;
		worldPos.z += strafeDirection.z;
		centre.x += strafeDirection.x;
		centre.z += strafeDirection.z;
	};

	void move(float z)
	{
		vec3 forward = normalize(centre - worldPos);
		vec3 moveDirection = forward*z;
		worldPos.x += moveDirection.x;
		worldPos.z += moveDirection.z;
		centre.z += moveDirection.z;
		centre.x += moveDirection.x;
	}
	void lift(float y)
	{
		vec3 liftDirection = up * y;
		worldPos.y += liftDirection.y;
		centre.y += liftDirection.y;
	}
	void rotateX(float x)
	{
		centre.z = (float)(centre.z + sin(x)*forward.x + cos(x)*forward.z);
		centre.x = (float)(centre.x + cos(x)*forward.x - sin(x)*forward.z);
	}
	void rotateY(float y)
	{
		glRotatef(-y, centre.x, centre.y, centre.z);

	}
};

class sphere
{
private:
	vec3 worldPos = vec3(0.0f, 0.0f, 0.0f);
public:
	void draw()
	{

	};

};

Camera camera;

// initialises modules
bool Init();

//Deallocates memory and calls SDL closing functions
void Close();

Transform calculateTransform(Camera*, vec3,vec3,vec3);


//loads obj file and parses headers adding vertecies to the referenced vector, ditto with the element array.
bool loadOBJ(const char * path, vector <vertex> & out_vertices, vector<int> & elemtArray) 
{
	vector<vec3> temp_vertices;
	std::vector< unsigned int > vertexIndices;
	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) 
	{
		char lineHeader[1028];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break; // EOF = End Of File. Quit the loop.
		}
		if (strcmp(lineHeader, "v") == 0)
		{
			vec3 vertex;
			
			fscanf(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
			
		}
		if (strcmp(lineHeader, "f") == 0)
		{
			int vertexIndex[4], uvIndex[4], normalIndex[4];

			fscanf(file, "%d", &vertexIndex[0]);
			fscanf(file, "%d", &vertexIndex[1]);
			fscanf(file, "%d", &vertexIndex[2]);
			fscanf(file, "%d", &vertexIndex[3]);

			//int matches = fscanf(file, "%d %d %d &d", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2], &vertexIndex[3]);
			elemtArray.push_back(vertexIndex[0]-1);
			elemtArray.push_back(vertexIndex[1]-1);
			elemtArray.push_back(vertexIndex[2]-1);

			
			elemtArray.push_back(vertexIndex[2]-1);
			elemtArray.push_back(vertexIndex[0]-1);
			elemtArray.push_back(vertexIndex[3]-1);
			 

			
		}
		if (strcmp(lineHeader, "z") == 0)
		{
			cout << "z found cya" << endl;
			break;
		}
	}

	//Added vertex data to vertex array with random colour
	for (unsigned int i = 0; i < temp_vertices.size(); i++)
	{
		vec3 vert = temp_vertices[i];
		vec4 vertexCol = vec4(sin(rand()%90), sin(rand() % 90), sin(rand() % 90),1);
		//vec4 vertexCol = vec4(0.5f, 0.5f, 0.5f, 1.0);
		vertex foo = { vert, vertexCol };
		out_vertices.push_back(foo);
		//cout << vertex.x << " : " << vertex.y << " : " << vertex.z << endl;
	}
}
void createGridVec(vector<vertex>&lineVector)
{
	for (int i = -500; i < 500; i++)
	{
		vec3 lineVert1 = vec3(i *0.02, -1, 50);
		vec3 lineVert2 = vec3(i*0.02, -1, -50);
		vec3 lineVert3 = vec3(50, -1, i *0.02);
		vec3 lineVert4 = vec3(-50, -1, i*0.02);
		vec4 col = vec4(0.5, 0.5, 0.5, 1.0);
		vertex lineVertex = { lineVert1, col };
		vertex lineVertex2 = { lineVert2, col };
		vertex lineVertex3 = { lineVert3, col };
		vertex lineVertex4 = { lineVert4, col };
		lineVector.push_back(lineVertex);
		lineVector.push_back(lineVertex2);
		lineVector.push_back(lineVertex3);
		lineVector.push_back(lineVertex4);
	}
}
void drawGrid(MVP &MVPLocation,GLuint &lineBuff , vector <vertex> &lineVerts )
{
	Transform MVPMatrix = calculateTransform(&camera, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(10.0f, 10.0f, 10.0f));
	glUniformMatrix4fv(MVPLocation.modelMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.modelMatrix));
	glUniformMatrix4fv(MVPLocation.viewMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.viewMatrix));
	glUniformMatrix4fv(MVPLocation.projectionMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.projectionMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, lineBuff);
	glBufferData(GL_ARRAY_BUFFER, lineVerts.size() * sizeof(vertex), &lineVerts[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), ((void*)offsetof(vertex, vertexCol)));

	glDrawArrays(GL_LINES, 0, lineVerts.size());
}