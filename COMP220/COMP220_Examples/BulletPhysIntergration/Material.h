
#pragma once
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

/**
* CREDIT FOR THIS CLASS RICH https://github.com/Stompyy/bsc-live-coding/blob/2017-18/COMP220/COMP220_Examples/92_Refactor/Material.h
*/
class Material
{
public:
	Material();
	~Material();

	void setAmbientColour(const float r, const float g, const float b, const float a) { m_AmbientColour = vec4(r, g, b, a); }
	vec4& getAmbientColour() { return m_AmbientColour; }

	void setDiffuseColour(const float r, const float g, const float b, const float a) { m_DiffuseColour = vec4(r, g, b, a); }
	vec4& getDiffuseColour() { return m_DiffuseColour; }

	void setSpecularColour(const float r, const float g, const float b, const float a) { m_SpecularColour = vec4(r, g, b, a); }
	vec4& getSpecularColour() { return m_SpecularColour; }

	void setAmbientColour(const float r, const float g, const float b) { m_AmbientColour = vec4(r, g, b, 1.0f); }

	void setDiffuseColour(const float r, const float g, const float b) { m_DiffuseColour = vec4(r, g, b, 1.0f); }

	void setSpecularColour(const float r, const float g, const float b) { m_SpecularColour = vec4(r, g, b, 1.0f); }
	
	void setSpecularPower(const float newSpecularPower) { m_SpecularPower = newSpecularPower; }
	float getSpecularPower() { return m_SpecularPower; }

private:
	vec4 m_AmbientColour;
	vec4 m_DiffuseColour;
	vec4 m_SpecularColour;
	float m_SpecularPower;
};

/**This class is just for the GLuinform variable locations
*
*/
class MaterialLocation
{
public:

	void updateLocations(GLuint programToUse);

	void sendUniforms(Material & material);

	GLint m_SpecularColourLoc;
	GLint m_DiffuseColourLoc;
	GLint m_AmbientColourLoc;
	GLint m_SpecularPowerLoc;
};