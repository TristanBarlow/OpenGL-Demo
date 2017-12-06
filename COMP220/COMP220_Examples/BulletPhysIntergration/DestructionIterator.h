#pragma once
#include<vector>
#include "rayCast.h"
#include "WorldObject.h"

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