#pragma once
#include "Grid.h"

Grid::Grid(Camera& cam):camera(cam),MVPMatrix(cam, cam.aspectRatio)
{
}

void Grid::createGridVec(int numberX, int numberY, GLuint programID)
{
	// set program to use and generate buffer
	LineShader = programID;
	glGenBuffers(1, &lineBuff);

	//Get uniform locations
	MVPLineShaderLoc = { glGetUniformLocation(LineShader, "modelMatrix"),
						 glGetUniformLocation(LineShader, "viewMatrix"),
						 glGetUniformLocation(LineShader, "projectionMatrix")};
	
	// start pos for the grid vector
	int startPosX = 0 - numberX / 2;
	int startPosY = 0 - numberY / 2;

	//generation loop
	for (int i = startPosX; i < startPosX*-1; i++)
	{
		
		for (int j = startPosY; j < startPosY*-1; j++)
		{
			//vert positions
			vec3 lineVert1 = vec3(i, -1, j);
			vec3 lineVert2 = vec3(i, -1, 0);
			vec3 lineVert3 = vec3(i, -1, j);
			vec3 lineVert4 = vec3(0, -1, j);

			//defualt colours of grid
			vec4 tempColourX = xVertColour;
			vec4 tempColourY = yVertColour;

			// every 10 on the Y change the colour of the vert
			if (j%10==0 || j== startPosY)
			{
				tempColourX = xTenthColour;
			}

			// every 10 on the X change the colour of the vert
			if (i%10 == 0 || i == startPosX)
			{
				tempColourY = yTenthColour;
			}

			//creates the vertecies with the generated colour and the positions
			LineVertex lineVertex =  { lineVert1, tempColourY };
			LineVertex lineVertex2 = { lineVert2, tempColourY };
			LineVertex lineVertex3 = { lineVert3, tempColourX };
			LineVertex lineVertex4 = { lineVert4, tempColourX };

			// add created verts to vector
			lineVerts.push_back(lineVertex);
			lineVerts.push_back(lineVertex2);
			lineVerts.push_back(lineVertex3);
			lineVerts.push_back(lineVertex4);
		}

	}

	//after all is created calls the buffer data function
	copyBufferData();
}

void Grid::draw()
{
	//use shader program
	glUseProgram(LineShader);

	//set up stencil modes and the width of the lines
	glLineWidth(5);
	glPolygonMode(GL_FRONT, GL_LINE);
	glStencilFunc(GL_ALWAYS, 1, -1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//re calculates the MVP matrix
	MVPMatrix.calculateTransform(transform);

	// Send uniforms across
	glUniformMatrix4fv(MVPLineShaderLoc.modelMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.modelMatrix));
	glUniformMatrix4fv(MVPLineShaderLoc.viewMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.viewMatrix));
	glUniformMatrix4fv(MVPLineShaderLoc.projectionMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.projectionMatrix));

	//Bind the buffer with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, lineBuff);

	// enable the attrib arryas and re define them seems not to work unless I call glvertexAttribPOinter again.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), ((void*)offsetof(LineVertex, vertexCol)));

	//DRAW the verts
	glDrawArrays(GL_LINES, 0, lineVerts.size());

}

Grid::~Grid()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDeleteBuffers(1, &lineBuff);
}

void Grid::copyBufferData()
{
	glBindBuffer(GL_ARRAY_BUFFER, lineBuff);
	glBufferData(GL_ARRAY_BUFFER, lineVerts.size() * sizeof(LineVertex), &lineVerts[0], GL_STATIC_DRAW);
}
