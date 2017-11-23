#include "Light.h"

Light::Light(Camera& camera):staticMesh(camera)
{
}

void Light::init(GLuint programID)
{
	staticMesh.init("only_quad_sphere.txt", programID);
	staticMesh.worldPos = location;
}

void Light::render()
{
	staticMesh.worldPos = location;
	staticMesh.worldRot = rotation;
	staticMesh.worldScale = scale;
	staticMesh.render();
}

void Light::moveCircle()
{
	angle += 0.005;
	location.x += 0.1 *sin(angle);
	location.y += 0.1 *cos(angle);
}
