#pragma once

#include "WorldObject.h"
#include "Grid.h"

class Renderer
{
public:
	/** Renders a Mesh
	* the mesh thats wants to be rendered. 
	*/
	void renderMesh(Mesh & mesh);
	
	/**Renders a Grid
	*the grid that wants to be rendered.
	*/
	void renderGrids(Grid & grid);

	/**Renders an object from world object
	*the object to be rendered 
	*/
	void renderWorldObjects(WorldObject & object);
};