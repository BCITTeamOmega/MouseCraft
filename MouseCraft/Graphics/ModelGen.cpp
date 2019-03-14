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
			0, halfheight, -halfwidth,
			0, -halfheight, halfwidth,
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
			1, 0,
			0, 1,
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
			1, 0,
			0, 1,
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
			1, 0,
			0, 1,
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

	const float oneOverSqrt3 = 1.0f / sqrt(3);

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
		-oneOverSqrt3, -oneOverSqrt3, oneOverSqrt3,
		oneOverSqrt3, -oneOverSqrt3, oneOverSqrt3,
		-oneOverSqrt3, oneOverSqrt3, oneOverSqrt3,
		oneOverSqrt3, oneOverSqrt3, oneOverSqrt3,
		-oneOverSqrt3, -oneOverSqrt3, -oneOverSqrt3,
		oneOverSqrt3, -oneOverSqrt3, -oneOverSqrt3,
		-oneOverSqrt3, oneOverSqrt3, -oneOverSqrt3,
		oneOverSqrt3, oneOverSqrt3, -oneOverSqrt3
	};
	vector<GLfloat> texCoord = {
		0, 0,
		1, 0,
		0, 1,
		1, 1,
		0, -1,
		1, -1,
		0, 2,
		1, 2
	};
	vector<GLuint> indices = {
		0, 1, 3,
		0, 3, 2,
		0, 2, 4,
		2, 6, 4,
		4, 6, 7,
		4, 7, 5,
		3, 5, 7,
		1, 5, 3,
		0, 4, 1,
		1, 4, 5,
		2, 3, 6,
		3, 7, 6
	};

	Geometry* g = new Geometry();
	g->setVertexData(vertices);
	g->setNormalData(normals);
	g->setTexCoordData(texCoord);
	g->setIndices(indices);

	return new Model(g);
}