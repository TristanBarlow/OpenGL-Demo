#include "Light.h"

Light::Light(Camera & cam):WorldObject(cam)
{
}

void Light::moveCircle()
{
	angle += 0.005;
	this->w_Transform.getWorldLocation().x += 0.1 *sin(angle);
	this->w_Transform.getWorldLocation().y += 0.1 *cos(angle);
}
