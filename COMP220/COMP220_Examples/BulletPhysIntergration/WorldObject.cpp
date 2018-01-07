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

	glUniform3fv(lightDirectionLoc, 1, value_ptr(lightLocation));
	glUniform3fv(cameraLocationLoc, 1, value_ptr(this->w_Transform.getWorldLocation() - camera.getWorldPos()));

	glUniform1i(textureLocation, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, w_textureID);

	//render
	w_mesh->render();
}

void WorldObject::update()
{
	if (w_rigidBody != NULL)
	{
		btTransform trans;
		trans =w_rigidBody->getWorldTransform();
		btVector3 myWorldPos =  trans.getOrigin();
		w_Transform.getWorldLocation() = vec3(myWorldPos.x(), myWorldPos.y(), myWorldPos.z());

		btQuaternion qRot = trans.getRotation();
		quat myRotation = quat(qRot.getW(), qRot.getX(), qRot.getY(), qRot.getZ() );
		vec3 temp = degrees(eulerAngles(myRotation));
		vec3 convertedVector = vec3(temp.x, temp.z,temp.y);
		w_Transform.setWorldRotation(convertedVector);
	}
}

void WorldObject::init(Mesh& meshAd, GLuint newProgramToUse, GLuint textureID)
{
	//getting defualt values
	w_mesh = &meshAd;
	programToUse = newProgramToUse;

	if (textureID != -1)
	{
		w_textureID = textureID;
		textureLocation = glGetUniformLocation(programToUse, "baseTexture");
	}

	setUniformLoctions();
}

void WorldObject::addRigidBody(PhysicsSimulation& physSim, btVector3& size, btScalar mass)
{
	quat qRot = quat(radians(vec3(w_Transform.getWorldRotation().x, w_Transform.getWorldRotation().z, w_Transform.getWorldRotation().y)));
	btQuaternion finalOrientation = btQuaternion(qRot.x, qRot.y, qRot.z, qRot.w);
	w_rigidBody = physSim.creatRigidBodyCube(size, mass, btVector3(w_Transform.getWorldLocation().x, w_Transform.getWorldLocation().y, w_Transform.getWorldLocation().z),finalOrientation );
}

void WorldObject::addCompoundBody(PhysicsSimulation & physSim)
{
	//btQuaternion finalOrientation = toQuaternion(w_Transform.getWorldRotation().x, w_Transform.getWorldRotation().y, w_Transform.getWorldRotation().z);
	quat qRot = quat(radians(vec3(w_Transform.getWorldRotation().x, w_Transform.getWorldRotation().z, w_Transform.getWorldRotation().y)));
	btQuaternion finalOrientation = btQuaternion(qRot.x, qRot.y, qRot.z, qRot.w);
	
	w_rigidBody = physSim.createCompoundBody(w_mesh->getSubMehses() , 10, w_Transform, finalOrientation);
}

void WorldObject::Destroy()
{
	glDeleteTextures(1, &w_textureID);
	
}

btQuaternion & WorldObject::calculateQuat()
{

	btQuaternion QuatAroundX = btQuaternion(btVector3(1.0, 0.0, 0.0), sin(radians(w_Transform.getWorldRotation().x/2)));
	btQuaternion QuatAroundY = btQuaternion(btVector3(0.0, 1.0, 0.0), sin(radians(w_Transform.getWorldRotation().y/2)));
	btQuaternion QuatAroundZ = btQuaternion(btVector3(0.0, 0.0, 1.0), sin(radians(w_Transform.getWorldRotation().z/2)));

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

