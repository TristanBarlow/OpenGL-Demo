#include "Renderer.h"

void Renderer::renderMesh(Mesh & mesh)
{

		for (int i = 0; i < mesh.getSubMehses().size(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh.getSubMehses()[i]->m_VBO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getSubMehses()[i]->m_EBO);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((void*)offsetof(Vertex, vertexCol)));
			
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((void*)offsetof(Vertex, textureCoords)));

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((void*)offsetof(Vertex, vertexNormals)));

			glDrawElements(GL_TRIANGLES, mesh.getSubMehses()[i]->meshElementArray.size(), GL_UNSIGNED_INT, 0);
		}

}

void Renderer::renderGrids(Grid & grid)
{
}
