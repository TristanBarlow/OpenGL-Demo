#include "Camera.h"
void  Camera::strafe(float x)
{
	vec3 forward = normalize(centre - worldPos);
	right = cross(up, forward);
	vec3 strafeDirection = right*x;
	worldPos.x += strafeDirection.x;
	worldPos.z += strafeDirection.z;
	centre.x += strafeDirection.x;
	centre.z += strafeDirection.z;
};

void Camera:: move(float z)
{
	vec3 forward = normalize(centre - worldPos);
	vec3 moveDirection = forward*z;
	worldPos.x += moveDirection.x;
	worldPos.z += moveDirection.z;
	centre.z += moveDirection.z;
	centre.x += moveDirection.x;
}
void Camera::lift(float y)
{
	vec3 liftDirection = up * y;
	worldPos.y += liftDirection.y;
	centre.y += liftDirection.y;
}
void Camera::rotate(float x, float y)
{
	TurnDegreesFromOriginX +=  x/ mouseSens;
	TurnDegreesFromOriginY -= y/ mouseSens;
	float TanTurnFromOriginY = tan(TurnDegreesFromOriginY);
	std::cout << TanTurnFromOriginY << std::endl;
	if (TanTurnFromOriginY > 5.0f)  TanTurnFromOriginY = 5.0f;
	if (TanTurnFromOriginY < -5.0f)  TanTurnFromOriginY = -5.0f;
	std::cout << TanTurnFromOriginY << std::endl;
	// Move camera lookatpoint to a trigonometry calculated position, CameraDistance far away, relative to the camera position
	centre = worldPos + length * vec3(cos(TurnDegreesFromOriginX), TanTurnFromOriginY, sin(TurnDegreesFromOriginX));
}