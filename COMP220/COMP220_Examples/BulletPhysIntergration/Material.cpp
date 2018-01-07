#pragma once

#include "Material.h"

Material::Material()
{
	//defualt values for all material variables
	m_AmbientColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DiffuseColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpecularColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpecularPower = 25.0f;
}

Material::~Material()
{
}

void MaterialLocation::updateLocations(GLuint programToUse)
{
	//uniform gets
	m_SpecularColourLoc = glGetUniformLocation(programToUse, "specularMaterialColour");
	m_DiffuseColourLoc = glGetUniformLocation(programToUse, "diffuseMaterialColour");
	m_AmbientColourLoc = glGetUniformLocation(programToUse, "diffuseLightColour");
	m_SpecularPowerLoc = glGetUniformLocation(programToUse, "specularIntensity");
}

void MaterialLocation::sendUniforms(Material & material)
{
	//uniform sends
	glUniform4fv(m_SpecularColourLoc, 1, value_ptr(material.getSpecularColour()));
	glUniform1f(m_SpecularPowerLoc,	material.getSpecularPower());
	glUniform4fv(m_DiffuseColourLoc, 1, value_ptr(material.getDiffuseColour()));
	glUniform4fv(m_AmbientColourLoc, 1, value_ptr(material.getAmbientColour()));
}

