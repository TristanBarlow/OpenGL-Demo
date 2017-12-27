#include "WorldObject.h"



WorldObject::WorldObject(Camera & cam):w_MVPTransform(cam, 4/3), camera(cam)
{
}

void WorldObject::draw(vec3& lightLocation )
{
	update();

	glUseProgram(programToUse);

	//send uniforms
	w_MVPTransform.calculateTransform(w_Transform);
	w_MVPLocToUse.sendMVPuniforms(w_MVPTransform);
	w_materialLocation.sendUniforms(w_material);

	glUniform3fv(lightDirectionLoc, 1, value_ptr(vec3(0.0, 0.0, 0.0)));
	glUniform3fv(cameraLocationLoc, 1, value_ptr(this->w_Transform.getWorldLocation() - camera.getWorldPos()));

	glUniform1i(textureLocation, 0);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, textureID);

	//render
	w_mesh->render();
}

void WorldObject::update()
{
	if (w_rigidBody != NULL)
	{
		btTransform trans;
		w_rigidBody->getMotionState()->getWorldTransform(trans);
		btVector3 myWorldPos =  trans.getOrigin();
		w_Transform.getWorldLocation() = vec3(myWorldPos.x(), myWorldPos.y(), myWorldPos.z());

		btQuaternion qRot = trans.getRotation();
		quat myRotation = quat(qRot.x(), qRot.y(), qRot.z(), qRot.w());
		w_Transform.getWorldRotation() = eulerAngles(myRotation);
	}
	w_mesh->m_Transform = w_Transform;

}

void WorldObject::init(Mesh& meshAd, GLuint newProgramToUse, const std::string& fileName)
{
	//getting defualt values
	w_mesh = &meshAd;
	programToUse = newProgramToUse;

	if (fileName != "")
	{
		textureID = loadTexture(fileName);
		textureLocation = glGetUniformLocation(programToUse, "baseTexture");
	}

	setUniformLoctions();
}

void WorldObject::addRigidBody(PhysicsSimulation& physSim, btVector3 size, btScalar mass)
{
	float converter = 3.12 / 180;
	btQuaternion finalOrientation = calculateQuat();
	w_rigidBody = physSim.creatRigidBodyCube(size, mass, btVector3(w_Transform.getWorldLocation().x, w_Transform.getWorldLocation().y, w_Transform.getWorldLocation().z),finalOrientation );
}

void WorldObject::addCompoundBody(PhysicsSimulation & physSim)
{
	w_Transform.setWorldRotation(vec3(rand() % 90, rand() % 90, rand() % 90));
	btScalar foo(w_Transform.getWorldLocation().x);
	btQuaternion finalOrientation = calculateQuat();
	w_rigidBody = physSim.createCompoundBody(w_mesh->getSubMehses() , 1.0, btVector3(w_Transform.getWorldLocation().x, w_Transform.getWorldLocation().y, w_Transform.getWorldLocation().z), finalOrientation, foo);

}

void WorldObject::Destroy()
{
	glDeleteTextures(1, &textureID);
	
}

btQuaternion & WorldObject::calculateQuat()
{
	btQuaternion QuatAroundX = btQuaternion(btVector3(1.0, 0.0, 0.0), w_Transform.getWorldRotation().x);
	btQuaternion QuatAroundY = btQuaternion(btVector3(0.0, 1.0, 0.0), w_Transform.getWorldRotation().y);
	btQuaternion QuatAroundZ = btQuaternion(btVector3(0.0, 0.0, 1.0), w_Transform.getWorldRotation().z);
	btQuaternion finalOrientation = QuatAroundX * QuatAroundY * QuatAroundZ;
	return finalOrientation;
}

void WorldObject::setUniformLoctions()
{
	w_MVPLocToUse.getMVPuniforms(programToUse);
	w_materialLocation.updateLocations(programToUse);
	lightDirectionLoc = glGetUniformLocation(programToUse, "lightLocation");
	lightDistanceLoc = glGetUniformLocation(programToUse, "lightDistance");
	cameraLocationLoc = glGetUniformLocation(programToUse, "cameraLocation");
}
