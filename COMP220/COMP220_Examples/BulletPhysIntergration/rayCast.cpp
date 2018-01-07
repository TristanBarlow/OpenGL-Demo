#include "rayCast.h"

RayCast::RayCast(PhysicsSimulation& physSim ,Camera &cam,vec3& start, vec3& direction, int length, GLuint programID, vec4& colour): MVPMatrix(cam,cam.aspectRatio)
{
	LineShader = programID;
	MVPLineShaderLoc = { glGetUniformLocation(LineShader, "modelMatrix"),
						 glGetUniformLocation(LineShader, "viewMatrix"),
						 glGetUniformLocation(LineShader, "projectionMatrix")
						};
	glGenBuffers(1, &lineBuff);
	vec3 lineVertStart = vec3(start.x +0.1, start.y-0.1, start.z+0.1);
	vec3 lineVertEnd = vec3(direction.x * length, direction.y * length, direction.z * length);

	btVector3 Start = btVector3(lineVertStart.x, lineVertStart.y, lineVertStart.z);
	btVector3 End = btVector3(lineVertEnd.x, lineVertEnd.y, lineVertEnd.z);
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	physSim.dynamicsWorld->rayTest(Start,End , RayCallback);

	if (RayCallback.hasHit()) {
		End = RayCallback.m_hitPointWorld;
		cout << "hit object" << endl;
	}

	vec4 vertColour = colour;
	lineVerts.push_back(Vertex(lineVertStart, vertColour));
	lineVerts.push_back(Vertex(lineVertEnd, vertColour));
	copyBufferData();
}

void RayCast::draw()
{
	glUseProgram(LineShader);

	glLineWidth(3);
	glPolygonMode(GL_FRONT, GL_LINE);

	glStencilFunc(GL_ALWAYS, 1, -1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	MVPMatrix.calculateTransform(worldTransform);
	glUniformMatrix4fv(MVPLineShaderLoc.modelMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.modelMatrix));
	glUniformMatrix4fv(MVPLineShaderLoc.viewMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.viewMatrix));
	glUniformMatrix4fv(MVPLineShaderLoc.projectionMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.projectionMatrix));

	glBindBuffer(GL_ARRAY_BUFFER, lineBuff);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((void*)offsetof(Vertex, vertexCol)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((void*)offsetof(Vertex, textureCoords)));

	glDrawArrays(GL_LINES, 0, lineVerts.size());
}

void RayCast::copyBufferData()
{
	glBindBuffer(GL_ARRAY_BUFFER, lineBuff);
	glBufferData(GL_ARRAY_BUFFER, lineVerts.size() * sizeof(Vertex), &lineVerts[0], GL_STATIC_DRAW);
}

RayCast::~RayCast()
{
	glDeleteBuffers(1, &lineBuff);
}