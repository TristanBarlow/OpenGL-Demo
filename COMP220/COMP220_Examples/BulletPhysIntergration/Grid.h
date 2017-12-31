#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Vertex.h"
#include "MVP.h"
#include "MVPTransform.h"
#include "Camera.h"
#include <iostream>
#include "ShaderLoader.h"
#include "Transform.h"

using namespace std;
using glm::vec3;
using glm::vec4;

class Grid
{
	public:
		
		Transform transform;
		/**Default constructor for grid class.
		*Takes a camera reference to get the camera view matrix. Could Just pass a red to the matrix but having a camera ref may be useful. 
		*/
		Grid(Camera&);

		/**Draws the current vertext array bound in the instance of the class. If you want to stop the grids do not call this function
		*		
		*/
		void draw();

		/**Sets up the shader program to use, generates grid buffers and creates the vertex array used to draw grid
		*numberX the number of lines to draw on the X axis
		*numberY the number of lines to draw on the Y axis
		*programID the shader program the grid will be using
		*/
		void createGridVec(int numberX, int numberY, GLuint programID);

		/**
		*Defualt Deconstructor
		*/
		~Grid();

		/**
		*Copies buffer data of verts
		*/
		void copyBufferData();

	private:

		//colour of the standard line
		vec4 xVertColour =  vec4(0.3, 0.3, 0.3, 1.0);
		vec4 yVertColour =  vec4(0.3, 0.3, 0.3, 1.0);

		//Colour of the tenth lines respectively
		vec4 xTenthColour = vec4(1.0, 0.0, 0.0, 1.0);
		vec4 yTenthColour = vec4(0.0, 1.0, 0.0, 1.0);

		Camera& camera;
		GLuint LineShader;
		
		//Line vertex and line buffer variables
		vector<Vertex> lineVerts;
		GLuint lineBuff;
		
		//MVP transform and Mvp locations
		MVPTransform MVPMatrix;
		MVP MVPLineShaderLoc;	
};
