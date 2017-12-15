#pragma once

#include "glm/glm.hpp"

using namespace glm;
class Transform
{
public:
	Transform()
	{
		worldLocation = vec3(0.0f, 0.0f, 0.0f);
		worldRotation = vec3(0.0f, 0.0, 0.0f);
		worldScale	  = vec3(1.0f, 1.0f, 1.0f);
	}
	/**Sets the private variable WorldPos to the argument
	* newWorldPos is the new position to set to
	*/
	void setWorldLocation(vec3& newWorldLocation) { worldLocation = newWorldLocation; };

	/**gets the private variable WorldPos
	* returns a reference to the worldPos
	*/
	vec3& getWorldLocation() { return worldLocation; };

	/**Sets the private variable WorldRotation to the argument
	* newWorldRotation is the new rotation to set to
	*/
	void setWorldRotation(vec3& newWorldRotation) { worldRotation = newWorldRotation; };

	/**gets the private variable WorldRotation
	* returns a reference to the WorldRotation
	*/
	vec3& getWorldRotation() { return worldRotation; };
	
	/**Sets the private variable WorldScale to the argument
	* newWorldScale is the new size to set to
	*/
	void setWorldScale(vec3& newWorldScale) { worldScale = newWorldScale; };

	/**gets the private memeber variable worldScale
	*returns reference to world scale
	*/
	vec3& getWorldScale() { return worldScale; };
private:
	 vec3 worldLocation;
	 vec3 worldRotation;
	 vec3 worldScale;
};