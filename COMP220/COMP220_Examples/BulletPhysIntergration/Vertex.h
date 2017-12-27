#pragma once
#include <glm/glm.hpp>

using namespace glm;
struct Vertex
{
	Vertex(vec3 vertPos, vec4 vertColour) { vertexPos = vertPos; vertexCol = vertColour; textureCoords = vec2(0.0, 0.0), vertexNormals = vec3(0.0, 0.0, 0.0); };
	Vertex() {};																		 
	vec3 vertexPos;
	vec4 vertexCol;
	vec2 textureCoords;
	vec3 vertexNormals;
};
struct LineVertex
{
	
	vec3 vertexPos;
	vec4 vertexCol;
	LineVertex(vec3&, vec4&);
};