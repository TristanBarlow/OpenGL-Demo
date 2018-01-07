#pragma once

#include "Mesh.h"

Mesh::~Mesh()
{
	//itterates through the submeshes and deletes pointers
	auto iter = subMeshes.begin();
	while (iter != subMeshes.end())
	{
		if ((*iter))
		{
			delete (*iter);
			iter = subMeshes.erase(iter); 
		}
		else
		{
			iter++;
		}
	}
	subMeshes.clear();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

Mesh::Mesh()
{

}

void Mesh::init(const std::string& filename)
{
	loadMeshFromFile(filename, subMeshes);
	copyBufferData();
} 

void Mesh::render() 
{
	//itterate through all submeshes and send data
	for (int i = 0; i < subMeshes.size(); i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, subMeshes[i]->m_VBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMeshes[i]->m_EBO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((void*)offsetof(Vertex, vertexCol)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((void*)offsetof(Vertex, textureCoords)));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((void*)offsetof(Vertex, vertexNormals)));

		glDrawElements(GL_TRIANGLES, subMeshes[i]->meshElementArray.size(), GL_UNSIGNED_INT, 0);
	}
}

void Mesh::copyBufferData()
{
	//send data to gpu
	for (int i = 0; i < subMeshes.size(); i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, subMeshes[i]->m_VBO);
		glBufferData(GL_ARRAY_BUFFER, subMeshes[i]->meshVertex.size() * sizeof(Vertex), &subMeshes[i]->meshVertex[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMeshes[i]->m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, subMeshes[i]->meshElementArray.size() * sizeof(int), &subMeshes[i]->meshElementArray[0], GL_STATIC_DRAW);

	}
}

void Mesh::maxMinCheck(subMesh * pSubmesh, vec3 & currentVector)
{
	//checks
	if (pSubmesh->maxXYZ.x < currentVector.x)
	{
		pSubmesh->maxXYZ.x = currentVector.x;
	}
	if (pSubmesh->maxXYZ.y < currentVector.y)
	{
		pSubmesh->maxXYZ.y = currentVector.y;
	}
	if (pSubmesh->maxXYZ.z < currentVector.z)
	{
		pSubmesh->maxXYZ.z = currentVector.z;
	}

	if (pSubmesh->minXYZ.x > currentVector.x)
	{
		pSubmesh->minXYZ.x = currentVector.x;
	}
	if (pSubmesh->minXYZ.y > currentVector.y)
	{
		pSubmesh->minXYZ.y = currentVector.y;
	}
	if (pSubmesh->minXYZ.z > currentVector.z)
	{
		pSubmesh->minXYZ.z = currentVector.z;
	}

}

bool Mesh::loadMeshFromFile(const std::string& filename, std::vector<subMesh*> &meshes)
{

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);

	if (!scene)
	{
		printf("Model Loading Error - %s\n", importer.GetErrorString());
		return false;
	}


	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh *currentMesh = scene->mMeshes[i];
		subMesh *pSubMesh = new subMesh();
		pSubMesh->init();
		Vertex currentVertex;
		for (int v = 0; v < currentMesh->mNumVertices; v++)
		{
			if (currentMesh->mVertices[v].x != NULL)
			{
				aiVector3D currentModelVertex = currentMesh->mVertices[v];

				currentVertex.vertexPos = { currentModelVertex.x, currentModelVertex.y, currentModelVertex.z };
				maxMinCheck(pSubMesh, currentVertex.vertexPos);
				currentVertex.vertexCol = { 1.0f, 1.0f, 1.0f, 1.0f };
			}

			if (currentMesh->HasNormals())
			{
			aiVector3D currentNormals = currentMesh->mNormals[v];
			currentVertex.vertexNormals = { currentNormals.x, currentNormals.y,currentNormals.z };
			}

			if (currentMesh->HasTextureCoords(0))
			{
				aiVector3D currentTextureCoordinates = currentMesh->mTextureCoords[0][v];
				currentVertex.textureCoords = { currentTextureCoordinates.x, currentTextureCoordinates.y };
			}
			else 
			{
				currentVertex.vertexCol = {1.0f,1.0f,(rand()%10/100) , 1.0f };
				
			}
			pSubMesh->meshVertex.push_back(currentVertex);
		}
		for (int f = 0; f < currentMesh->mNumFaces; f++)
		{
			aiFace currentModelFace = currentMesh->mFaces[f];
			pSubMesh->meshElementArray.push_back(currentModelFace.mIndices[2]);
			pSubMesh->meshElementArray.push_back(currentModelFace.mIndices[0]);
			pSubMesh->meshElementArray.push_back(currentModelFace.mIndices[1]);
		}
		pSubMesh->calcSizes();
		meshes.push_back(pSubMesh);
		vertices.clear();
		indices.clear();
	}
	return true;
}


