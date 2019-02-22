#include "ModelGen.h"
#include <vector>

using std::vector;

Model* ModelGen::makeQuad(ModelGen::Axis facing, float width, float height) {
	float halfwidth = width / 2.0f;
	float halfheight = height / 2.0f;

	vector<GLuint> indices = {
		0, 1, 3,
		0, 3, 2
	};

	Geometry* g = new Geometry();
	g->setIndices(indices);

	switch (facing) {
	case ModelGen::Axis::X:
	{
		vector<GLfloat> vertices = {
			0, -halfheight, -halfwidth,
			0, -halfheight, halfwidth,
			0, halfheight, -halfwidth,
			0, halfheight, halfwidth
		};
		vector<GLfloat> normals = {
			1, 0, 0,
			1, 0, 0,
			1, 0, 0,
			1, 0, 0
		};
		vector<GLfloat> texCoord = {
			0, 0,
			0, 1,
			1, 0,
			1, 1
		};
		g->setVertexData(vertices);
		g->setNormalData(normals);
		g->setTexCoordData(texCoord);
		break;
	}
	case ModelGen::Axis::Y:
	{
		vector<GLfloat> vertices = {
			-halfwidth, 0, -halfheight,
			-halfwidth, 0, halfheight,
			halfwidth, 0, -halfheight,
			halfwidth, 0, halfheight
		};
		vector<GLfloat> normals = {
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
			0, 1, 0
		};
		vector<GLfloat> texCoord = {
			0, 0,
			0, 1,
			1, 0,
			1, 1
		};
		g->setVertexData(vertices);
		g->setNormalData(normals);
		g->setTexCoordData(texCoord);
		break;
	}
	case ModelGen::Axis::Z:
	{
		vector<GLfloat> vertices = {
			-halfwidth, -halfheight, 0,
			halfwidth, -halfheight, 0,
			-halfwidth, halfheight, 0,
			halfwidth, halfheight, 0
		};
		vector<GLfloat> normals = {
			0, 0, 1,
			0, 0, 1,
			0, 0, 1,
			0, 0, 1
		};
		vector<GLfloat> texCoord = {
			0, 0,
			0, 1,
			1, 0,
			1, 1
		};
		g->setVertexData(vertices);
		g->setNormalData(normals);
		g->setTexCoordData(texCoord);
		break;
	}
	}

	return new Model(g);
}

Model* ModelGen::makeCube(float width, float height, float depth) {
	float halfwidth = width / 2.0f;
	float halfheight = height / 2.0f;
	float halfdepth = depth / 2.0f;

	const float sqrt3 = sqrt(3);

	vector<GLfloat> vertices = {
		-halfwidth, -halfheight, halfdepth,
		halfwidth, -halfheight, halfdepth,
		-halfwidth, halfheight, halfdepth,
		halfwidth, halfheight, halfdepth,
		-halfwidth, -halfheight, -halfdepth,
		halfwidth, -halfheight, -halfdepth,
		-halfwidth, halfheight, -halfdepth,
		halfwidth, halfheight, -halfdepth
	};
	vector<GLfloat> normals = {
		-sqrt3, -sqrt3, -sqrt3,
		-sqrt3, -sqrt3, sqrt3,
		-sqrt3, sqrt3, -sqrt3,
		-sqrt3, sqrt3, sqrt3,
		sqrt3, -sqrt3, -sqrt3,
		sqrt3, -sqrt3, sqrt3,
		sqrt3, sqrt3, -sqrt3,
		sqrt3, sqrt3, sqrt3
	};
	vector<GLfloat> texCoord = {
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0
	};
	vector<GLuint> indices = {
		0, 3, 1,
		0, 2, 3,
		0, 4, 2,
		2, 4, 6,
		4, 7, 6,
		4, 5, 7,
		3, 7, 5,
		1, 3, 5,
		0, 1, 4,
		1, 5, 4,
		2, 6, 3,
		3, 6, 7
	};

	Geometry* g = new Geometry();
	g->setVertexData(vertices);
	g->setNormalData(normals);
	g->setTexCoordData(texCoord);
	g->setIndices(indices);

	return new Model(g);
}