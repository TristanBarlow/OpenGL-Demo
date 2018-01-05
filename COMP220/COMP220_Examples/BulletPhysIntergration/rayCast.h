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
#include "PhysicsSimulation.h"

using namespace std;
using glm::vec3;
using glm::vec4;

class RayCast
{
	public:
		RayCast(PhysicsSimulation& physSim, Camera &cam, vec3& start, vec3& direction, int length, GLuint programID, vec4& colour);
		void draw();
		~RayCast();

	private:		
		void copyBufferData();
		void checkForHit();
		Transform worldTransform;
		Camera& camera;
		GLuint LineShader;
		vector<Vertex> lineVerts;
		MVPTransform MVPMatrix;
		GLuint lineBuff;
		MVP MVPLineShaderLoc;

};
