#include "WorldObject.h"

class WorldObject 
{
public:
	vec3 worldLocation = vec3(0.0f, 0.0f, 0.0f);
	vec3 worldRotation = vec3(0.0f, 0.0f, 0.0f);
	vec3 worldScale = vec3(0.0f, 0.0f, 0.0f);
private:
	Mesh mesh;
};