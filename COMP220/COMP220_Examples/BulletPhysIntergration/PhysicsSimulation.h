#pragma once

#include "btBulletDynamicsCommon.h"


class PhysicsSimulation
{
public:
	PhysicsSimulation();
	~PhysicsSimulation();
	btRigidBody* creatRigidBodyCube(btVector3& = btVector3(btScalar(50.), btScalar(1.), btScalar(50.)), btScalar = btScalar(0.), btVector3& = btVector3(0, -56, 0));
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
private:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;

};
