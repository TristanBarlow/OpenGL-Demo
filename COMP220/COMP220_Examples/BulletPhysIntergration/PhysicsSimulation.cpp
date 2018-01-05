#include "PhysicsSimulation.h"

PhysicsSimulation::PhysicsSimulation()
{
	///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

	///-----initialization_end-----
}

PhysicsSimulation::~PhysicsSimulation()
{
	int i;
	for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		if (obj != NULL)
		{
			dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;
}

btRigidBody* PhysicsSimulation::creatRigidBodyCube(btVector3& size, btScalar objMass,  btVector3& location, btQuaternion& rotation)
{
	// physics shtuff
	btCollisionShape* newShape = new btBoxShape(size);

	btTransform newTransform;
	newTransform.setIdentity();
	newTransform.setOrigin(location);
	newTransform.setRotation(rotation);

	btScalar mass(objMass);
	btVector3 localInertia(0,0,0);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(newTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newShape, localInertia);
	btRigidBody* newBody = new btRigidBody(rbInfo);

	collisionShapes.push_back(newShape);

	
	newBody->setActivationState(DISABLE_DEACTIVATION);
	//add the body to the dynamics world
	dynamicsWorld->addRigidBody(newBody);
	return newBody;
}

btRigidBody* PhysicsSimulation::createCompoundBody(vector <subMesh*>& subMeshref, btScalar objMass,Transform& transform, btQuaternion& rotation)
{
	btCompoundShape* newCompoundShape = new btCompoundShape();

	// physics shtuff
	for (int i = 0; i < subMeshref.size(); i++)
	{

		btVector3 tempTrans(btScalar(subMeshref[i]->minXYZ.x), btScalar(subMeshref[i]->minXYZ.y), btScalar(subMeshref[i]->minXYZ.z));
		btVector3 tempVec(btScalar(subMeshref[i]->xSize*transform.getWorldScale().x/2), 
						  btScalar(subMeshref[i]->ySize*transform.getWorldScale().y/2), 
						  btScalar(subMeshref[i]->zSize*transform.getWorldScale().z/2));


		btTransform newTransform;
		newTransform.setIdentity();
		newTransform.setOrigin(tempVec);

		btCollisionShape* newShape = new btBoxShape(tempVec);
		collisionShapes.push_back(newShape);

		newCompoundShape->addChildShape(newTransform, newShape);
	}

	vec3 tempVec = transform.getWorldLocation();
	btTransform newTransform;
	newTransform.setIdentity();
	newTransform.setOrigin(btVector3(tempVec.x, tempVec.y, tempVec.z));
	newTransform.setRotation(rotation);

	btScalar mass(objMass);
	btVector3 localInertia (0,0,0);
	newCompoundShape->calculateLocalInertia(mass,localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(newTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newCompoundShape, localInertia);
	btRigidBody* newBody = new btRigidBody(rbInfo);

	collisionShapes.push_back(newCompoundShape);
	newBody->setActivationState(DISABLE_DEACTIVATION);
	newBody->setRestitution(1.0);
	//add the body to the dynamics world
	dynamicsWorld->addRigidBody(newBody);
	return newBody;

}

void PhysicsSimulation::enableDebug(Camera & camera, GLuint programID)
{
	for (int i = 0; dynamicsWorld->getNumCollisionObjects(); )
	{
	}
}

void PhysicsSimulation::debugPhysicsRender()
{

}
