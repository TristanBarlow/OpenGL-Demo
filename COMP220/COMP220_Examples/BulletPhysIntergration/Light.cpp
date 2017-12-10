#include "Light.h"

void Light::moveCircle()
{
	angle += 0.005;
	this->getWorldLocation().x += 0.1 *sin(angle);
	this->getWorldLocation().y += 0.1 *cos(angle);
}
