#include "SkyBox.h"


SkyBox::SkyBox(Camera& cam):s_camera(cam),s_MVPTransform(cam, cam.aspectRatio)
{
}

void SkyBox::init(Mesh & mesh, GLuint shaderProgram, GLuint texture)
{
	s_mesh = &mesh;
	s_shaderProgram = shaderProgram;
	s_texture = texture;


	s_skyboxUniform = glGetUniformLocation(s_shaderProgram, "skybox");

	s_MVPLoc.getMVPuniforms(s_shaderProgram);

}

void SkyBox::render()
{
	s_transform.setWorldLocation(s_camera.getWorldPos());

	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	glUseProgram(s_shaderProgram);

	s_MVPTransform.calculateTransform(s_transform);
	s_MVPLoc.sendMVPuniforms(s_MVPTransform);
	

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, s_texture);

	s_mesh->render();

	glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);

} 
