#pragma once

#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "Transform.h"
#include "Mesh.h"
#include "PhysicsSimulation.h"
#include "Camera.h"
#include "Material.h"
#include "MVP.h"
#include "Texture.h"

class WorldObject
{
public:

	/**Defualt constructor for the worldObject class
	*cam is used to create a camera refence to easily grab the camera location for the shaders
	*/
	WorldObject(Camera & cam);

	/** calls update function and then renders the mesh
	* light location is the current location of the light source, in future this could be turned into a vector to support multiple light sources.
	*/
	void draw(vec3& lightLocation = vec3(0.0f, 0.0f, 0.0f));

	/**Init should always be called when trying to use worldobject class
	*meshAd is a reference to the mesh that the current world object will use.
	*newProgramTouse is the shader program that this particular worldobject will be using
	*fileName is the name of the texture for this worldobject
	*/
	void init(Mesh &meshAd, GLuint newProgramToUse , const std::string& fileName = "");

	void addRigidBody(PhysicsSimulation&, btVector3& = btVector3(btScalar(50.), btScalar(1.), btScalar(50.)), btScalar = (0.));

	/** Adds a compound body to represent this world object in the physcs world 
	*/
	void addCompoundBody(PhysicsSimulation&);
	
	/** destroys this instance
	*/
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

	Material& getMaterial() { return w_material; };
private:
	
	/**
	* this function sets the position and rotation of the worldobject to that of the physcis objcet
	*/
	void update();

	/**Turns the world rotation of this object to quaternion
	*/
	btQuaternion& calculateQuat();
	
	/**Goes through all the required gluniformGets and assigns them. Uses the programToUse
	*/
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