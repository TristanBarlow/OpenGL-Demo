#pragma once

#include "btBulletDynamicsCommon.h"
#include "Mesh.h"
#include "Camera.h"

class PhysicsSimulation
{
public:
	/**Default construtor for the PhysicsSimulation class, all the initialisation of the bulletphysics is done in this
	*
	*/
	PhysicsSimulation();

	/**Deconstructor for the PhysicisSimulation class, all the dereferencing for the physics simulation is done here.
	*
	*/
	~PhysicsSimulation();
	
	/**Creates a rigidbody for collisions in the physcis world. 
	*size a vector reference that describes the size of the cube
	*mass describes the weight of the object in the physics simulation.
	*location is the world position of the physics body
	*rot is the rotation of the shape as a quat
	*/
	btRigidBody* creatRigidBodyCube(btVector3& size = btVector3(btScalar(50.), btScalar(1.), btScalar(50.)), btScalar mass = btScalar(0.), btVector3& location = btVector3(0, -56, 0), btQuaternion& rot = btQuaternion(0.0,0.0,0.0,1.0));
	
	/** Creates a compoundbody(multiple rigidbodies) for collision in the physics world
	*subMeshref the submeshes to create compound bodies around
	* objMass is the mass of the object in the physics simulation
	* location is the world position of the physics body
	*rotation is the rotation of the physcis body
	* size
	*/
	btRigidBody* createCompoundBody(vector<subMesh*>& subMeshref, btScalar objMass, btVector3& location, btQuaternion& rotation, btScalar& size);
	
	/**This is where the initialisation of the debug physics render will be
	*camera will be used to get a reference to the camera
	*programID will be the shader program used for the debug lines
	*/
	void enableDebug(Camera& camera, GLuint programID);

	/**
	*This will be where the acutal render call will be made
	*/
	void debugPhysicsRender();

	//poitner to the dynamics world
	btDiscreteDynamicsWorld* dynamicsWorld;

	//array of the pointers to the collision shapes
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

private:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;

};
