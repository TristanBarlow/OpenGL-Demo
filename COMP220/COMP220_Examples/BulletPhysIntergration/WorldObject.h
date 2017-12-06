#pragma once

#include <vector>

#include "Mesh.h"
#include "PhysicsSimulation.h"
#include <glm/gtc/type_ptr.hpp>

class WorldObject
{
public:
	vec3 worldLocation = vec3(0.0f, 0.0f, 0.0f);
	vec3 worldRotation = vec3(0.0f, 0.0f, 0.0f);
	vec3 worldScale = vec3(1.0f, 1.0f, 1.0f);
	float speedOfTravel;
	float mass;
	bool hasGravity = true;
	void draw(vec3& = vec3(0.0f,0.0f,0.0f));
	void update();
	void init(Mesh &meshAd);
	void addRigidBody(PhysicsSimulation&, btVector3 = btVector3(btScalar(50.), btScalar(1.), btScalar(50.)), btScalar = (0.) );
	void addCompoundBody(PhysicsSimulation&);
	Mesh* mesh;
	btRigidBody* rigidBody = NULL;
private:

	
	
};