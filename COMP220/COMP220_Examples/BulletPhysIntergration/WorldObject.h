#pragma once

#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "Transform.h"
#include "Mesh.h"
#include "PhysicsSimulation.h"

class WorldObject
{
public:
	void draw(vec3& = vec3(0.0f, 0.0f, 0.0f));
	void update();
	void init(Mesh &meshAd);
	void addRigidBody(PhysicsSimulation&, btVector3 = btVector3(btScalar(50.), btScalar(1.), btScalar(50.)), btScalar = (0.));
	void addCompoundBody(PhysicsSimulation&);
	
	Transform worldTransform;

	/*sets the defualt shader colour for the Mesh
	* newNoTextureColour is the new value it will be se to
	*/
	void setNoTextureColour(vec4& newNoTextureColour) { noTextureColour = newNoTextureColour; };

	/*sets the defualt shader colour for the Mesh
	* newNoTextureColour is the new value it will be se to
	*/
	Mesh* getMesh() { return mesh; }

	/*
	*returns the private variable pointer to the rigid body
	*/
	btRigidBody* getRigidBody() { return rigidBody; };

	/*
	*Sets wether or not the object should be litt, BECAREFUL may have to change the shader program too!
	*/
	void setIsLitt(bool newIsLitt) { isLitt = newIsLitt; };

	/*
	*Sets the new shader program that will be used when this instance of the mesh is rendered
	*then it will go through and update the gluniiforms needed for that particular new program
	*/
	void setProgamToUse(GLuint newProgram) { programToUse = newProgram; mesh->setProgramToUse(programToUse); mesh->uniformGetPass(); MVPLocToUse = mesh->getMVPLocation(); };

private:
	btQuaternion& calculateQuat();

	vec4 noTextureColour = vec4(0.3, 0.3, 0.3, 1.0);
	bool isLitt = false;

	Mesh* mesh;
	btRigidBody* rigidBody = NULL;

	GLuint programToUse;
	MVP MVPLocToUse;
	
};