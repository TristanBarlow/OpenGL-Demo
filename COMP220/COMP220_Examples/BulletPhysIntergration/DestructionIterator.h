#pragma once
#include<vector>
#include "rayCast.h"
#include "WorldObject.h"
// This header is used to store destroy functions to make other files look a little cleaner

/*RAYCAST DESTROYER!!!! 
* vec RAYCAST VECTOR TO DESTROY!!!!
*/
void destroyRaycast(std::vector<RayCast*>& vec)
{
	auto iter = vec.begin();
	while (iter != vec.end())
	{
		if ((*iter))
		{
			delete (*iter);
			iter = vec.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

/*WORLD OBJECT DESTROYER!!!!
* vec WORLD OBJECT VECTOR TO DESTROY!!!!
*/
void destroyWorldObjects(std::vector<WorldObject*>& vec)
{
	auto iter = vec.begin();
	while (iter != vec.end())
	{
		if ((*iter))
		{
			delete (*iter);
			iter = vec.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}