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
#include "MVPTransform.h"

using namespace std;
using glm::vec3;
using glm::vec4;

class RayCast
{
	public:
		RayCast(Camera &cam, vec3& start, vec3& direction, int length, GLuint programID, vec4& colour);
		void draw();
		~RayCast();
		void copyBufferData();
	private:
		Transform worldTransform;
		Camera& camera;
		GLuint LineShader;
		vector<LineVertex> lineVerts;
		MVPTransform MVPMatrix;
		GLuint lineBuff;
		MVP MVPLineShaderLoc;

};
