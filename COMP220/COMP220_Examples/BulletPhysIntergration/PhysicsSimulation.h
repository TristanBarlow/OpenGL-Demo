#pragma once

#include "btBulletDynamicsCommon.h"


class PhysicsSimulation
{
public:
	PhysicsSimulation();
	~PhysicsSimulation();
	btRigidBody* creatRigidBodyCube(btVector3& size = btVector3(btScalar(50.), btScalar(1.), btScalar(50.)), btScalar mass = btScalar(0.), btVector3& location = btVector3(0, -56, 0), btQuaternion& rot = btQuaternion(0.0,0.0,0.0,1.0));
	btDiscreteDynamicsWorld* dynamicsWorld;
	btRigidBody* createConvexHullShape(btVector3& size, btScalar objMass, btVector3& location, btQuaternion& rotation);
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
private:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
};
