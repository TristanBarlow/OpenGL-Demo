#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "Camera.h"
using namespace glm;

struct Transform
{
	Transform(Camera&cam, float aspectRatio) :viewMatrix(cam.cameraMatrix)
	{
		projectionMatrix = perspective(radians(90.0f), aspectRatio, 0.1f, 1000.0f);
	};

	mat4 modelMatrix;
	mat4& viewMatrix;
	mat4 projectionMatrix;

	void calculateTransform(vec3 &spawnLocation = vec3(0.0f, 0.0f, 0.0f), vec3 &spawnRotation = vec3(0.0f, 0.0f, 0.0f), vec3 &spawnScale = vec3(1.0f, 1.0f, 1.0f))
	{

		vec3 trianglePosition = spawnLocation;
		mat4 translationMatrix = translate(trianglePosition);
		//create rotation matrix
		vec3 trianglRotation = spawnRotation;
		mat4 rotationXMatrix = rotate(trianglRotation.x, vec3(1.0f, 0.0f, 0.0f));
		mat4 rotationYMatrix = rotate(trianglRotation.y, vec3(0.0, 1.0f, 0.0f));
		mat4 rotationZMatrix = rotate(trianglRotation.z, vec3(0.0, 0.0f, 1.0f));
		mat4 rotationMatix = rotationZMatrix*rotationYMatrix*rotationXMatrix;

		//create scaling matrix
		vec3 scaleVec = spawnScale;
		mat4 ScalingMatrix = scale(scaleVec);
		this->modelMatrix = translationMatrix*rotationMatix*ScalingMatrix;
	}

};
