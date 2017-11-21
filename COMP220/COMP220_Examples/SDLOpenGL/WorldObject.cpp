#include "WorldObject.h"



void WorldObject::draw(vec3& lightLocation )
{
	update();
	mesh->render(lightLocation);
}

void WorldObject::update()
{
	mesh->worldPos = worldLocation;
	mesh->worldRot = worldRotation;
	mesh->worldScale = worldScale;
}

void WorldObject::init(Mesh& meshAd)
{
	hasGravity = true;
	directionOfTravel = vec3(0.0f, -1.0f, 0.0f);
	mesh = &meshAd;
}
