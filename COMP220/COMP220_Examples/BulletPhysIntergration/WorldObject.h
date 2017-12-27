#pragma once

#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "Transform.h"
#include "Mesh.h"
#include "PhysicsSimulation.h"

class WorldObject
{
public:
	WorldObject(Camera & cam);

	void draw(vec3& = vec3(0.0f, 0.0f, 0.0f));

	void update();

	void init(Mesh &meshAd, GLuint newProgramToUse , const std::string& fileName = "");

	void addRigidBody(PhysicsSimulation&, btVector3 = btVector3(btScalar(50.), btScalar(1.), btScalar(50.)), btScalar = (0.));

	void addCompoundBody(PhysicsSimulation&);
	
	void Destroy();

	Transform w_Transform;

	/*sets the defualt shader colour for the Mesh
	* newNoTextureColour is the new value it will be se to
	*/
	void setNoTextureColour(vec4& newNoTextureColour) { noTextureColour = newNoTextureColour; };

	/*sets the defualt shader colour for the Mesh
	* newNoTextureColour is the new value it will be se to
	*/
	Mesh* getMesh() { return w_mesh; }

	/*
	*returns the private variable pointer to the rigid body
	*/
	btRigidBody* getRigidBody() { return w_rigidBody; };

	/*
	*Sets wether or not the object should be litt, BECAREFUL may have to change the shader program too!
	*/
	void setIsLitt(bool newIsLitt) { isLitt = newIsLitt; };


private:
	btQuaternion& calculateQuat();
	
	void setUniformLoctions();

	vec4 noTextureColour = vec4(0.3, 0.3, 0.3, 1.0);
	bool isLitt = false;

	Mesh* w_mesh = NULL;
	btRigidBody* w_rigidBody = NULL;

	GLuint programToUse;

	MVP w_MVPLocToUse;
	MVPTransform w_MVPTransform;
	
	Material w_material;
	MaterialLocation w_materialLocation;

	GLint lightDirectionLoc;
	GLint lightDistanceLoc;
	GLint cameraLocationLoc;

	GLuint textureID;
	GLint textureLocation;

	Camera & camera;
};