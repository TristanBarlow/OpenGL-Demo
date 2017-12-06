#pragma once

#include "subMesh.h"

void subMesh::init()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
}

void subMesh::calcSizes()
{
	xSize = maxXYZ.x - minXYZ.x;
	ySize = maxXYZ.y - minXYZ.y;
	zSize = maxXYZ.z - minXYZ.z;
	minXYZ = vec3(minXYZ.x - (xSize / 2), minXYZ.y - (ySize / 2), minXYZ.z - (zSize / 2));
	dir = maxXYZ - minXYZ;
}
