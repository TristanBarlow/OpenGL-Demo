#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Transform.h"

using namespace glm;

struct MVPTransform
{
	/**Defualt constructor Also calculates and stores the projection matrix. This is done once per class, could be once and have all the instances refering to it.
	*cam use to get a refernce to the camera matrix
	*aspectRatio is use in the calculation of the projection matrix.
	*/
	MVPTransform(Camera&cam, float aspectRatio) :viewMatrix(cam.cameraMatrix)
	{
		projectionMatrix = perspective(radians(90.0f), aspectRatio, 0.1f, 1000.0f);
	};

	mat4 modelMatrix;
	mat4& viewMatrix;
	mat4 projectionMatrix;

	/**Calculates the MVPTransform and updates the value of the model matrix on the new value. 
	*transform is the trasform that needs to be converted into the new model matrix. 
	*/
	void calculateTransform(Transform& transform)
	{

		vec3 worldPosition = transform.getWorldLocation();
		mat4 translationMatrix = translate(worldPosition);
		//create rotation matrix
		vec3 worldRotation = transform.getWorldRotation();
		mat4 rotationXMatrix = rotate(worldRotation.x, vec3(1.0f, 0.0f, 0.0f));
		mat4 rotationYMatrix = rotate(worldRotation.y, vec3(0.0, 1.0f, 0.0f));
		mat4 rotationZMatrix = rotate(worldRotation.z, vec3(0.0, 0.0f, 1.0f));
		mat4 rotationMatix = rotationZMatrix*rotationYMatrix*rotationXMatrix;

		//create scaling matrix
		vec3 scaleVec = transform.getWorldScale();
		mat4 ScalingMatrix = scale(scaleVec);
		this->modelMatrix = translationMatrix*rotationMatix*ScalingMatrix;
	}

};
