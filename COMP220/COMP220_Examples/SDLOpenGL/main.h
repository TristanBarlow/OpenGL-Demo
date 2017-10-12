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

struct face 
{
	int facenum;
	bool four;
	int faces[4];
	face(int facen, int  f1, int f2, int f3) : facenum(facen) 
	{
		faces[0] = f1;
		faces[1] = f2;
		faces[2] = f3;
		four = false;
	}
	face(int facen, int  f1, int f2, int f3, int f4) : facenum(facen)
	{
		faces[0] = f1;
		faces[1] = f2;
		faces[2] = f3;
		faces[3] = f4;
		four = true;
	}
};

struct Transform
{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
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
	void strafe(float x)
	{
		vec3 strafeDirection = right*x;
		worldPos.x += strafeDirection.x;
		worldPos.z += strafeDirection.z;
		centre.x += strafeDirection.x;
		centre.z += strafeDirection.z;
	};

	void move(float z)
	{
		vec3 moveDirection = forward*z;
		worldPos.x += moveDirection.x;
		worldPos.z += moveDirection.z;
		centre.z += moveDirection.z;
		centre.z += moveDirection.x;
	}
	void lift(float y)
	{
		vec3 liftDirection = up * y;
		worldPos.y += liftDirection.y;
		centre.y += liftDirection.y;
	}
	void rotateX(float x)
	{
		vec3 vVector = (centre - worldPos)*x;	// Get the view vector
		centre.x = vVector.x;
		centre.y = vVector.y;
		//cout << centre.y << ":" << centre.x << endl;
	}
	void rotateY(float y)
	{
		vec3 rotateDirection = forward * y;
	}
};

Camera camera;

// initialises modules
bool Init();

//Deallocates memory and calls SDL closing functions
void Close();

Transform calculateTransform(Camera*);


int loadObject(const char* filename, vector<face> &faces, vector<vec3> &vertex, vector<vec3> &normals, vector<string*> coord)
{
	std::ifstream in(filename);     //open the .obj file
	if (!in.is_open())       //if not opened, exit with -1
	{
		std::cout << "Nor oepened" << std::endl;
		return -1;
	}
	char buf[256];
	//read in every line to coord
	while (!in.eof())
	{
		in.getline(buf, 256);
		coord.push_back(new std::string(buf));
	}
	//go through all of the elements of coord, and decide what kind of element is that
	for (int i = 0; i<coord.size(); i++)
	{
		if (coord[i]->c_str()[0] == '#')   //if it is a comment (the first character is #)
			continue;       //we don't care about that
		else if (coord[i]->c_str()[0] == 'v' && coord[i]->c_str()[1] == ' ') //if vector
		{
			float tmpx, tmpy, tmpz;
			sscanf(coord[i]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz);       //read in the 3 float coordinate to tmpx,tmpy,tmpz
			vertex.push_back(vec3(tmpx, tmpy, tmpz));       //and then add it to the end of our vertex list
		}
		else if (coord[i]->c_str()[0] == 'v' && coord[i]->c_str()[1] == 'n')        //if normal vector
		{
			float tmpx, tmpy, tmpz;   //do the same thing
			sscanf(coord[i]->c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz);
			normals.push_back(vec3(tmpx, tmpy, tmpz));
		}
		else if (coord[i]->c_str()[0] == 'f')     //if face
		{
			int a, b, c, d, e;
			if (count(coord[i]->begin(), coord[i]->end(), ' ') == 3)     //if it is a triangle (it has 3 space in it)
			{
				sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b);
				faces.push_back(face(b, a, c, d));     //read in, and add to the end of the face list
			}
			else {
				sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b, &e, &b);
				faces.push_back(face(b, a, c, d, e));   //do the same, except we call another constructor, and we use different pattern
			}
		}
	}
	return 1;
}

