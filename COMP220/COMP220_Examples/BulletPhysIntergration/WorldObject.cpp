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
	mesh->setProgramToUse(programToUse);
	
	mesh->worldPos = worldLocation;
	mesh->worldRot = worldRotation;
	mesh->worldScale = worldScale;
	
	mesh->setNoTextureLightColour(noTextureColour);
	mesh->setIsLitt(isLitt);
	mesh->setMVPLocation(MVPLocToUse);
}

void WorldObject::init(Mesh& meshAd)
{
	//getting defualt values
	mesh = &meshAd;
	isLitt = meshAd.getIsLitt();
	programToUse = meshAd.getProgramToUse();
	noTextureColour = meshAd.getNoTextureLightColour();
	MVPLocToUse = meshAd.getMVPLocation();
}

void WorldObject::addRigidBody(PhysicsSimulation& physSim, btVector3 size, btScalar mass)
{
	float converter = 3.12 / 180;
	btQuaternion QuatAroundX = btQuaternion(btVector3(1.0, 0.0, 0.0), worldRotation.x);
	btQuaternion QuatAroundY = btQuaternion(btVector3(0.0, 1.0, 0.0), worldRotation.y);
	btQuaternion QuatAroundZ = btQuaternion(btVector3(0.0, 0.0, 1.0), worldRotation.z);
	btQuaternion finalOrientation = QuatAroundX * QuatAroundY * QuatAroundZ;
	rigidBody = physSim.creatRigidBodyCube(size, mass, btVector3(worldLocation.x, worldLocation.y, worldLocation.z),finalOrientation );
}

void WorldObject::addCompoundBody(PhysicsSimulation & physSim)
{
	worldRotation = vec3(rand() % 90, rand() % 90, rand() % 90);
	btQuaternion QuatAroundX = btQuaternion(btVector3(1.0, 0.0, 0.0), worldRotation.x);
	btQuaternion QuatAroundY = btQuaternion(btVector3(0.0, 1.0, 0.0), worldRotation.y);
	btQuaternion QuatAroundZ = btQuaternion(btVector3(0.0, 0.0, 1.0), worldRotation.z);
	btQuaternion finalOrientation = QuatAroundX * QuatAroundY * QuatAroundZ;
	btScalar foo(worldScale.x);
	rigidBody = physSim.createCompoundBody(mesh->subMeshes, 1.0, btVector3(worldLocation.x, worldLocation.y, worldLocation.z), finalOrientation,foo );

}

