#include "WorldObject.h"



void WorldObject::draw(vec3& lightLocation )
{
	update();
	mesh->render(lightLocation);
}

void WorldObject::update()
{
	if (rigidBody != NULL)
	{
		btTransform trans;
		rigidBody->getMotionState()->getWorldTransform(trans);
		btVector3 myWorldPos =  trans.getOrigin();
		worldLocation = vec3(myWorldPos.x(), myWorldPos.y(), myWorldPos.z());
		btQuaternion qRot = trans.getRotation();
		quat myRotation = quat(qRot.x(), qRot.y(), qRot.z(), qRot.w());
		worldRotation = eulerAngles(myRotation);
	}
	
	mesh->worldPos = worldLocation;
	mesh->worldRot = worldRotation;
	mesh->worldScale = worldScale;
}

void WorldObject::init(Mesh& meshAd)
{
	mesh = &meshAd;
}

void WorldObject::addRigidBody(PhysicsSimulation& physSim, btVector3 size, btScalar mass)
{
	rigidBody = physSim.creatRigidBodyCube(size, mass, btVector3(worldLocation.x, worldLocation.y, worldLocation.z));
}
