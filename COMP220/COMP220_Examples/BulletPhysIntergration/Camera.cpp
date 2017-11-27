#include "Camera.h"
Camera::Camera(float aspect, vec3& worldPos, vec3& centre, vec3& up)
{
	aspectRatio = aspect;
	this->worldPos = worldPos; 
	this->centre = centre;  
	this->up = up;
	mat4 cameraMatrix = lookAt(worldPos, centre, up);
	update();
}
void  Camera::strafe(float x)
{
	update();
	vec3 strafeDirection = right*x;
	worldPos.x += strafeDirection.x;
	worldPos.z += strafeDirection.z;
	centre.x += strafeDirection.x;
	centre.z += strafeDirection.z;
	
};

void Camera:: move(float z)
{
	update();
	vec3 moveDirection = forward*z;
	worldPos.x += moveDirection.x;
	worldPos.z += moveDirection.z;
	centre.z += moveDirection.z;
	centre.x += moveDirection.x;
}
void Camera::lift(float y)
{
	update();
	vec3 liftDirection = up * y;
	worldPos.y += liftDirection.y;
	centre.y += liftDirection.y;
}
void Camera::rotate(float x, float y)
{
	update();
	TurnDegreesFromOriginX +=  x/ mouseSens;
	TurnDegreesFromOriginY -= tan(y/ mouseSens);
	if (TurnDegreesFromOriginY > 1.0f)  TurnDegreesFromOriginY = 1.0f;
	if (TurnDegreesFromOriginY < -1.0f)  TurnDegreesFromOriginY = -1.0f;
	// Move camera lookatpoint to a trigonometry calculated position, CameraDistance far away, relative to the camera position
	centre = worldPos + length * vec3(cos(TurnDegreesFromOriginX), TurnDegreesFromOriginY, sin(TurnDegreesFromOriginX));
}

void Camera::update()
{
	forward = normalize(centre - worldPos);
	right = cross(up, forward);
	cameraMatrix = lookAt(worldPos, centre, up);
}
