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

	vec3 strafeDirection = right*x;

	worldPos.x += strafeDirection.x;
	worldPos.z += strafeDirection.z;

	centre.x += strafeDirection.x;
	centre.z += strafeDirection.z;

	update();
};

void Camera:: move(float z)
{
	vec3 moveDirection = forward*z;
	worldPos.x += moveDirection.x;
	worldPos.z += moveDirection.z;
	centre.z += moveDirection.z;
	centre.x += moveDirection.x;

	update();
}
void Camera::lift(float y)
{
	
	vec3 liftDirection = up * y;
	worldPos.y += liftDirection.y;
	centre.y += liftDirection.y;

	update();
}
void Camera::rotate(float x, float y)
{
	
	TurnDegreesFromOriginX +=  x/ mouseSens;
	TurnDegreesFromOriginY -= tan(y/ mouseSens);
	if (TurnDegreesFromOriginY > 1.0f)  TurnDegreesFromOriginY = 1.0f;
	if (TurnDegreesFromOriginY < -1.0f)  TurnDegreesFromOriginY = -1.0f;

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
