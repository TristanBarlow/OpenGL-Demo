#pragma once

#include <vector>

#include "Mesh.h"
#include "PhysicsSimulation.h"
#include <glm/gtc/type_ptr.hpp>

class WorldObject
{
public:
	void draw(vec3& = vec3(0.0f, 0.0f, 0.0f));
	void update();
	void init(Mesh &meshAd);
	void addRigidBody(PhysicsSimulation&, btVector3 = btVector3(btScalar(50.), btScalar(1.), btScalar(50.)), btScalar = (0.));
	void addCompoundBody(PhysicsSimulation&);

	/*Sets the private variable WorldPos to the argument
	* newWorldPos is the new position to set to
	*/
	void setWorldLocation(vec3& newWorldLocation) { worldLocation = newWorldLocation; };

	/*gets the private variable WorldPos
	* returns a reference to the worldPos
	*/
	vec3& getWorldLocation() { return worldLocation; };

	/*Sets the private variable WorldRotation to the argument
	* newWorldRotation is the new rotation to set to
	*/
	void setWorldRotation(vec3& newWorldRotation) { worldRotation = newWorldRotation; };

	/*gets the private variable WorldRotation
	* returns a reference to the WorldRotation
	*/
	vec3& getWorldRotation() { return worldRotation; };

	/*Sets the private variable WorldScale to the argument
	* newWorldScale is the new size to set to
	*/
	void setWorldScale(vec3& newWorldScale) { worldScale = newWorldScale; };

	/*gets the private variable WorldScale
	* returns a reference to the WorldScale
	*/
	vec3& getWorldScale() { return worldScale; };

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
	vec3 worldLocation = vec3(0.0f, 0.0f, 0.0f);
	vec3 worldRotation = vec3(0.0f, 90.0f, 0.0f);
	vec3 worldScale = vec3(1.0f, 1.0f, 1.0f);

	vec4 noTextureColour = vec4(0.3, 0.3, 0.3, 1.0);
	bool isLitt = false;

	Mesh* mesh;
	btRigidBody* rigidBody = NULL;

	GLuint programToUse;
	MVP MVPLocToUse;
	
};