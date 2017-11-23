#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Vertex.h"
#include "MVP.h"
#include"Transform.h"
#include "Camera.h"
#include <iostream>
#include "ShaderLoader.h"

using namespace std;
using glm::vec3;
using glm::vec4;

class RayCast
{
	public:
		RayCast(Camera&,vec3&, vec3&, int length, GLuint, vec4& = vec4(0.0f, 1.0f, 0.0f, 1.0f));
		void draw();
		~RayCast();
		void copyBufferData();
	private:
		Camera& camera;
		GLuint LineShader;
		vector<LineVertex> lineVerts;
		Transform MVPMatrix;
		GLuint lineBuff;
		MVP MVPLineShaderLoc;

};
