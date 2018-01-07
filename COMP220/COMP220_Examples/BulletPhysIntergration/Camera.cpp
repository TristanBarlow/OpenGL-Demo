#include "Camera.h"
Camera::Camera(float aspect, vec3& worldPos, vec3& centre, vec3& up)
{
	//initialising variables
	aspectRatio = aspect;
	this->worldPos = worldPos; 
	this->centre = centre;  
	this->up = up;
	
	//creating the cameraMatrix
	mat4 cameraMatrix = lookAt(worldPos, centre, up);

	//First update after intialistion incase setups of other classes need accurate camera variables
	update();
}
void  Camera::strafe(float x)
{

	//create the vector to change 
	vec3 strafeDirection = right*x;

	//update world pos
	worldPos.x += strafeDirection.x;
	worldPos.z += strafeDirection.z;

	//update centre pos
	centre.x += strafeDirection.x;
	centre.z += strafeDirection.z;

	update();
};

void Camera:: move(float z)
{
	//create the translation vector
	vec3 moveDirection = forward*z;

	//update the world pos
	worldPos.x += moveDirection.x;
	worldPos.z += moveDirection.z;

	//update the centre pos
	centre.z += moveDirection.z;
	centre.x += moveDirection.x;

	update();
}
void Camera::lift(float y)
{
	//create the translation vector
	vec3 liftDirection = up * y;

	//update the world position
	worldPos.y += liftDirection.y;

	//update the centre location
	centre.y += liftDirection.y;

	update();
}
void Camera::rotate(float x, float y)
{
	//update the turn from origin
	TurnDegreesFromOriginX +=  x/ mouseSens;
	TurnDegreesFromOriginY -= tan(y/ mouseSens);

	//check to see if the Y is looking up or down too much. if so cap it
	if (TurnDegreesFromOriginY > 2.0f)  TurnDegreesFromOriginY = 2.0f;
	if (TurnDegreesFromOriginY < -2.0f)  TurnDegreesFromOriginY = -2.0f;

	// Move camera lookatpoint to a trigonometry calculated position, CameraDistance far away, relative to the camera position
	centre = worldPos + length * vec3(cos(TurnDegreesFromOriginX), TurnDegreesFromOriginY, sin(TurnDegreesFromOriginX));
	
	update();
}


void Camera::update()
{
	forward = normalize(centre - worldPos);
	right = cross(up, forward);
	cameraMatrix = lookAt(worldPos, centre, up);
}
