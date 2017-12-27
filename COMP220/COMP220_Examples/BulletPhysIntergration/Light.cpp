#include "Light.h"

void Light::moveCircle()
{
	angle += 0.005;
	this->w_Transform.getWorldLocation().x += 0.1 *sin(angle);
	this->w_Transform.getWorldLocation().y += 0.1 *cos(angle);
}
