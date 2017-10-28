#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Vertex.h"
#include "MVP.h"
#include"Transform.h"
#include "Camera.h"

using namespace std;
using glm::vec3;
using glm::vec4;

class Grid
{
	public:
		void draw(MVP&, Camera&, float aspectRatio);
		void createGridVec(int, int);
		~Grid();
	private:

		vector<LineVertex> lineVerts;
		Transform MVPMatrix;
		GLuint lineBuff;
};
