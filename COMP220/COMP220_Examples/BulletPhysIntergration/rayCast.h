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
		/**Defualt constructor for the ray cast class
		*phySim is a reference to the physics simulation class used, to perform the hit detection for the ray cast.
		*cam is a reference to the camera, used for the constructors of the MVPTRansform class
		*start is a reference to the start vector of the raycast
		* direction is the direction in which the raycast should travel 
		* length is how long the raycast should be
		*programID is the ID of the shader program to use in when drawing the raycast
		* colour is the colour the raycast should be
		*/
		RayCast(PhysicsSimulation& physSim, Camera &cam, vec3& start, vec3& direction, int length, GLuint programID, vec4& colour);

		/**
		*Default deconstructor for the raycast class
		*/
		~RayCast();

		/**
		*draws the raycast
		*/	
		void draw();

	private:		
		/**
		*copies the buffer data onto the GPU
		*/
		void copyBufferData();

		//world transform of the raycast
		Transform worldTransform;

		//shader used to draw the raycast
		GLuint LineShader;
		
		//vector storing the vertex data for the raycast
		vector<Vertex> lineVerts;
		
		//the arrayBuffer
		GLuint lineBuff;
		
		//MVP matrix
		MVPTransform MVPMatrix;

		//MVP line  shader locations
		MVP MVPLineShaderLoc;

};
