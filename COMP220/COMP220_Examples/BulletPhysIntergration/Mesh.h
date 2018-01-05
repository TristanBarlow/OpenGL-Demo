#pragma once


#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <string>
#include <vector>
#include <iostream>

#include <GL\glew.h>
#include <SDL_opengl.h>

#include "Vertex.h"
#include "subMesh.h"

using namespace std;
class Mesh
{
public:
	/**
	*Default deconstructor of the Mesh class
	*/
	~Mesh();

	/**
	*Defualt constructor of the Mesh class
	*/
	Mesh();

	/**Initialises the mesh#
	* meshFileName used to find the file that the mesh is loaded from
	*/
	void init(const std::string& meshFileName);

	/** goes through each submesh and copies the buffer data
	*	
	*/
	void copyBufferData();

	/** 
	*Goes through each submesh and calles draw element arrays after setting up attrib pointers then the draw call
	*/
	void render();

	/** returns the reference to the vector of submesh pointers
	*
	*/
	vector<subMesh*>& getSubMehses(){return this->subMeshes;};

private:

	/** Loads the mesh and stores the output submeshes returns false if it failes to load
	*fileName is the name of the file to load the mesh from
	*subMeshes is the vector which the pointers to the submeshes loaded will be stored
	*/
	bool loadMeshFromFile(const std::string& fileName, std::vector<subMesh*>& subMeshes);

	/**checks the max and min co ords of the submesh used currently just to crate compound shapes from
	*pSubmesh POinter of the submesh to do the maxmin check on
	* currentVector the vector t to check to see if it is greater than the current stored one.
	*/
	void maxMinCheck(subMesh* pSubmesh,vec3 & currentVector);
	
	vector<subMesh*> subMeshes;
};

