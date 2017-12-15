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
		worldTransform.getWorldLocation() = vec3(myWorldPos.x(), myWorldPos.y(), myWorldPos.z());

		btQuaternion qRot = trans.getRotation();
		quat myRotation = quat(qRot.x(), qRot.y(), qRot.z(), qRot.w());
		worldTransform.getWorldRotation() = eulerAngles(myRotation);
	}
	mesh->setProgramToUse(programToUse);
	
	mesh->m_Transform = worldTransform;
	
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
	btQuaternion finalOrientation = calculateQuat();
	rigidBody = physSim.creatRigidBodyCube(size, mass, btVector3(worldTransform.getWorldLocation().x, worldTransform.getWorldLocation().y, worldTransform.getWorldLocation().z),finalOrientation );
}

void WorldObject::addCompoundBody(PhysicsSimulation & physSim)
{
	btScalar foo(worldTransform.getWorldLocation().x);
	btQuaternion finalOrientation = calculateQuat();
	rigidBody = physSim.createCompoundBody(mesh->subMeshes, 1.0, btVector3(worldTransform.getWorldLocation().x, worldTransform.getWorldLocation().y, worldTransform.getWorldLocation().z), finalOrientation,foo );

}

btQuaternion & WorldObject::calculateQuat()
{
	btQuaternion QuatAroundX = btQuaternion(btVector3(1.0, 0.0, 0.0), worldTransform.getWorldRotation().x);
	btQuaternion QuatAroundY = btQuaternion(btVector3(0.0, 1.0, 0.0), worldTransform.getWorldRotation().y);
	btQuaternion QuatAroundZ = btQuaternion(btVector3(0.0, 0.0, 1.0), worldTransform.getWorldRotation().z);
	btQuaternion finalOrientation = QuatAroundX * QuatAroundY * QuatAroundZ;
	return finalOrientation;
}
