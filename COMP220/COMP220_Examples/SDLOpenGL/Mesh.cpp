#pragma once

#include "Mesh.h"




void Mesh::init(const std::string& filename, GLuint programID)
{

	loadMeshFromFile(filename,subMeshes);
	MVPLoc = { glGetUniformLocation(programID, "modelMatrix"),
		glGetUniformLocation(programID, "viewMatrix"),
		glGetUniformLocation(programID, "projectionMatrix") };
}

void Mesh::render(Camera &camera, GLuint programID) 
{
	glUseProgram(programID);
	

	MVPMatrix = calculateTransform(camera, aspectRatio, worldPos, worldRot, worldScale);

	glUniformMatrix4fv(MVPLoc.modelMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.modelMatrix));
	glUniformMatrix4fv(MVPLoc.viewMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.viewMatrix));
	glUniformMatrix4fv(MVPLoc.projectionMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.projectionMatrix));

	for (int i = 0; i < subMeshes.size(); i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, subMeshes[i]->m_VBO);
		glBufferData(GL_ARRAY_BUFFER, subMeshes[i]->meshVertex.size() * sizeof(Vertex), &meshVertex[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshElementArray.size() * sizeof(int), &meshElementArray[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((void*)offsetof(Vertex, vertexCol)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((void*)offsetof(Vertex, textureCoords)));
		glDrawElements(GL_TRIANGLES, meshElementArray.size(), GL_UNSIGNED_INT, 0);
	}
}


bool loadMeshFromFile(const std::string& filename, std::vector<subMesh*> &meshes)
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

		for (int v = 0; v < currentMesh->mNumVertices; v++)
		{
			aiVector3D currentModelVertex = currentMesh->mVertices[v];

			aiVector3D currentTextureCoordinates = currentMesh->mTextureCoords[0][v];

			/*if (currentMesh->HasNormals)
			{
			aiVector3D currentNormals = currentMesh->mNormals[v];
			Vertex currentVertex = { { currentModelVertex.x,currentModelVertex.y,currentModelVertex.z },{ 1.0f,1.0f,1.0f,1.0f },{ currentTextureCoordinates.x,currentTextureCoordinates.y },{currentNormals.x, currentNormals.y, currentNormals.z} };
			}*/

			Vertex currentVertex = { { currentModelVertex.x,currentModelVertex.y,currentModelVertex.z },{ 1.0f,1.0f,1.0f,1.0f },{ currentTextureCoordinates.x,currentTextureCoordinates.y } };
			pSubMesh->meshVertex.push_back(currentVertex);
		}
		for (int f = 0; f < currentMesh->mNumFaces; f++)
		{
			aiFace currentModelFace = currentMesh->mFaces[f];
			pSubMesh->meshElementArray.push_back(currentModelFace.mIndices[0]);
			pSubMesh->meshElementArray.push_back(currentModelFace.mIndices[1]);
			pSubMesh->meshElementArray.push_back(currentModelFace.mIndices[2]);
		}
		pSubMesh->copyBufferData();

		meshes.push_back(pSubMesh);
		vertices.clear();
		indices.clear();
	}
	return true;
}

void subMesh::copyBufferData()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, meshVertex.size() * sizeof(Vertex), &meshVertex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshElementArray.size() * sizeof(int), &meshElementArray[0], GL_STATIC_DRAW);
}

void subMesh::init()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
}
