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
	~Mesh();
	Mesh();

	bool loadMeshFromFile(const std::string&, std::vector<subMesh*>&);

	void init(const std::string& meshFileName);

	void copyBufferData();

	void render(vec3 = vec3(0.0f, 0.0f, 0.0f));

	vector<subMesh*>& getSubMehses(){return this->subMeshes; };
private:
	void maxMinCheck(subMesh* pSubmesh,vec3 & currentVector);
	
	vector<subMesh*> subMeshes;
};

