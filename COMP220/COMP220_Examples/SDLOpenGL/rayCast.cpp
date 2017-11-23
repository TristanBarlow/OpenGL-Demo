#include "rayCast.h"

RayCast::RayCast(Camera &cam,vec3& start, vec3& direction, int length, GLuint programID, vec4& colour):camera(cam)
{
	LineShader = programID;
	MVPLineShaderLoc = { glGetUniformLocation(LineShader, "modelMatrix"),
		glGetUniformLocation(LineShader, "viewMatrix"),
		glGetUniformLocation(LineShader, "projectionMatrix")
	};
	glGenBuffers(1, &lineBuff);
	vec3 lineVertStart = start;
	vec3 lineVertEnd = vec3(direction.x * length, direction.y * length, direction.z * length);
	vec4 vertColour = colour;
	lineVerts.push_back(LineVertex(lineVertStart, vertColour));
	lineVerts.push_back(LineVertex(lineVertEnd, vertColour));
	copyBufferData();
}

void RayCast::draw(float aspectRatio)
{
	glUseProgram(LineShader);

	glLineWidth(4);
	glPolygonMode(GL_FRONT, GL_LINE);

	glStencilFunc(GL_ALWAYS, 1, -1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	MVPMatrix = calculateTransform(camera, aspectRatio);
	glUniformMatrix4fv(MVPLineShaderLoc.modelMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.modelMatrix));
	glUniformMatrix4fv(MVPLineShaderLoc.viewMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.viewMatrix));
	glUniformMatrix4fv(MVPLineShaderLoc.projectionMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.projectionMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, lineBuff);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), ((void*)offsetof(LineVertex, vertexCol)));
	glDrawArrays(GL_LINE_STRIP, 0, lineVerts.size());
}

void RayCast::copyBufferData()
{
	glBindBuffer(GL_ARRAY_BUFFER, lineBuff);
	glBufferData(GL_ARRAY_BUFFER, lineVerts.size() * sizeof(LineVertex), &lineVerts[0], GL_STATIC_DRAW);
}

RayCast::~RayCast()
{
	glDeleteBuffers(1, &lineBuff);
}