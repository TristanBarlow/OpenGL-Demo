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
#include "Camera.h"
#include "MVP.h"
#include "Transform.h"
#include "subMesh.h"
#include "Texture.h"

class Mesh
{
public:
	~Mesh();
	Mesh(Camera&);
	bool loadMeshFromFile(const std::string&, std::vector<subMesh*>&);

	void initVertOutline(GLuint, vec3 = vec3(0.0f, 0.0f, 0.0f));
	void init(const std::string&, GLuint, bool = false, bool = false, const std::string& = "");

	void copyBufferData();
	void render(vec3 = vec3(0.0f, 0.0f, 0.0f));

	vec3 worldPos =vec3(0.0f, 0.0f, 0.0f);
	vec3 worldRot = vec3(0.0f, 0.0f, 0.0f);
	vec3 worldScale = vec3(1.0f, 1.0f, 1.0f);

	void uniformGetPass();

	std::vector<subMesh*> subMeshes;

	//setters getters
	vec4& getDiffuseLightColour()					         {return diffuseLightColour; }
	void setDiffuseLightColour(vec4& diffuseColour)			 { diffuseLightColour = diffuseColour; }

	vec4& getNoTextureLightColour()							 { return noTextureColour;}
	void setNoTextureLightColour(vec4& noTextureLightColour) { noTextureColour = noTextureLightColour; }

	vec4& getSpecularLightColour()							 { return specularColour; }
	void setSpecularLightColour(vec4& specularLightColour)   { specularColour = specularLightColour; }

	vec4& getSpecularIntensity()							 { return specularIntensity; }
	void setSpecularIntensity(vec4& specularIntensity1)		 { specularIntensity = specularIntensity1; }

	void setIsLitt(bool newIsLitt) { islitt = newIsLitt; };
	bool getIsLitt() { return islitt; };

	void setProgramToUse(GLuint newProgram) { programToUse = newProgram; }
	GLuint getProgramToUse() { return programToUse; };

	MVP& getMVPLocation() { return MVPLoc; };
	void setMVPLocation(MVP& newMVPLoc) { MVPLoc = newMVPLoc; };

private:

	void vertOutliner(subMesh* submesh);

	void maxMinCheck(subMesh* pSubmesh,vec3 & currentVector);

	Camera& camera;

	Transform MVPMatrix;

	MVP MVPLoc;

	GLuint programToUse;

	GLint lightDirectionLoc;
	GLint lightDistanceLoc;
	GLint cameraLocationLoc;
	GLint vertOutlinerColourLoc;
	GLint specularMaterialColourLoc;
	GLint diffuseMaterialColourLoc;
	GLint lightColourLoc;
	GLint specularIntensityLoc;

	MVP MVPLineShaderLoc;

	vec3 vertOutlinerColour;

	vec4 noTextureColour = vec4(0.5f,0.5f,0.5f,1.0f) ;
	vec4 specularColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 diffuseLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 specularIntensity = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	vec3 tempLightDir;
	float distanceToLight;

	bool islitt;
	bool hasTexture = false;
	bool vertOutlinerMe = false;

	GLuint textureID;
	GLint textureLocation;

	GLuint LineShader;

	std::string textureFilename;

};

