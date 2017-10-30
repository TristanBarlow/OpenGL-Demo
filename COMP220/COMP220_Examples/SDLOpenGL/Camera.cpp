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
void Camera::rotateX(float x)
{
	centre.z = (float)(centre.z + sin(x)*forward.x + cos(x)*forward.z);
	centre.x = (float)(centre.x + cos(x)*forward.x - sin(x)*forward.z);
}
void Camera::rotateY(float y)
{
	glRotatef(-y, centre.x, centre.y, centre.z);

}