#include "Transform.h"

Transform::Transform(mat4*viewMat, mat4*projMat)
{
	viewMatrix = viewMat;
	projectionMatrix = projMat;
}

Transform::Transform()
{
}

void Transform::calculateTransform( vec3 & spawnLocation, vec3 & spawnRotation, vec3 &spawnScale)
{
	vectorPosition = spawnLocation;
	vectorRotation = spawnRotation;
	scaleVector = spawnScale;

	mat4 translationMatrix = translate(vectorPosition);

	//create rotation matrix
	vec3 triangleRotation = spawnRotation;
	mat4 rotationXMatrix = rotate(vectorPosition.x, vec3(1.0f, 0.0f, 0.0f));
	mat4 rotationYMatrix = rotate(vectorPosition.y, vec3(0.0, 1.0f, 0.0f));
	mat4 rotationZMatrix = rotate(vectorPosition.z, vec3(0.0, 0.0f, 1.0f));
	mat4 rotationMatix = rotationZMatrix*rotationYMatrix*rotationXMatrix;

	//create scaling matrix
	vec3 scaleVec = spawnScale;
	mat4 ScalingMatrix = scale(scaleVec);


	this->modelMatrix = translationMatrix*rotationMatix*ScalingMatrix;
}

