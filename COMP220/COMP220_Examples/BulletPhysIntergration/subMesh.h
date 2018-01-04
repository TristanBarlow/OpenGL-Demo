#pragma once

#include <string>
#include <vector>

#include <GL\glew.h>
#include <SDL_opengl.h>

#include "Vertex.h"

class subMesh
{
public:
	~subMesh()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
	}
	std::vector<unsigned int> meshElementArray;
	std::vector <Vertex> meshVertex;
	void init();
	void calcSizes();
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_VAO;
	vec3 maxXYZ = vec3(0.0, 0.0, 0.0);
	vec3 minXYZ = vec3(0.0, 0.0, 0.0);
	vec3 dir = vec3(0.0, 0.0, 0.0);
	float xSize = 0;
	float ySize = 0;
	float zSize = 0;
};