#include "ObjFileParser.h"
#include <sstream>
#include <GLEW/glew.h>
#include <iostream>
#include <set>

using std::tuple;
using std::make_tuple;
using std::istringstream;
using std::get;
using std::set;

ObjFileParser::ObjFileParser() : SimpleParser<Geometry*>() {
	this->miniParsers["v"] = [this](string line) {
		string cmd; // Just to contain a "v"
		float x, y, z;
		istringstream stream = istringstream(line);
		if (stream >> cmd >> x >> y >> z) {
			vertData.push_back(x);
			vertData.push_back(y);
			vertData.push_back(z);
		}
	};
	this->miniParsers["vt"] = [this](string line) {
		string cmd; // Just to contain a "v"
		float x, y;
		istringstream stream = istringstream(line);
		if (stream >> cmd >> x >> y) {
			textureCoordData.push_back(x);
			textureCoordData.push_back(y);
		}
	};
	this->miniParsers["vn"] = [this](string line) {
		string cmd; // Just to contain a "v"
		float x, y, z;
		istringstream stream = istringstream(line);
		if (stream >> cmd >> x >> y >> z) {
			vertexNormData.push_back(x);
			vertexNormData.push_back(y);
			vertexNormData.push_back(z);
		}
	};
	this->miniParsers["f"] = [this](string line) {
		string cmd; // Just to contain a "v"
		string p1, p2, p3;
		istringstream stream = istringstream(line);
		if (stream >> cmd >> p1 >> p2 >> p3) {
			faceData.push_back(parseFace(p1));
			faceData.push_back(parseFace(p2));
			faceData.push_back(parseFace(p3));
		}
	};
}

ObjFileParser::~ObjFileParser() {

}

Geometry* ObjFileParser::build() {
	Geometry* g = new Geometry();
	// Because of the way OpenGL works, we need to reformat things a bit
	// So we can duplicate normals and such easily
	vector<GLfloat> vertices = vector<GLfloat>();
	vector<GLfloat> normals = vector<GLfloat>();
	vector<GLfloat> texcoord = vector<GLfloat>();
	vector<GLuint> indices = vector<GLuint>();
	map<tuple<size_t, size_t, size_t>, size_t> faceConvert; // Map to convert obj faces to model faces
	for (tuple<size_t, size_t, size_t> face : faceData) {
		tuple<size_t, size_t, size_t> convertedFace;
		if (faceConvert.find(face) == faceConvert.end()) {
			// We haven't modeled this face vertex yet; Need to add it to the geometry
			size_t vertInd = get<0>(face);
			size_t normInd = get<1>(face);
			size_t texInd = get<2>(face);

			tuple<GLfloat, GLfloat, GLfloat> vertAdd;
			if (vertInd == 0) {
				// BIG PROBLEM
				// Yell at the programmer
				throw "Vertex indices in file are indexed to 0";
			}
			else {
				size_t ind = vertInd - 1;
				vertAdd = make_tuple(vertData[ind * 3], vertData[ind * 3 + 1], vertData[ind * 3 + 2]);
			}

			tuple<GLfloat, GLfloat, GLfloat> normAdd;
			if (normInd == 0) {
				// If there was no normal index, just treat it like as if the normal is from the center
				// In other words just use the vertex
				normAdd = vertAdd;
			}
			else {
				size_t ind = normInd - 1;
				normAdd = make_tuple(vertexNormData[ind * 3], vertexNormData[ind * 3 + 1], vertexNormData[ind * 3 + 2]);
			}

			tuple<GLfloat, GLfloat> texAdd;
			if (texInd == 0) {
				// If there was no texture index, just treat it as 0, 0
				// Doing an algorithm for figuring out the texture coordinates is way out of scope
				texAdd = make_tuple(0.0f, 0.0f);
			}
			else {
				size_t ind = texInd - 1;
				texAdd = make_tuple(textureCoordData[ind * 2], textureCoordData[ind * 2 + 1]);
			}

			size_t newVertInd = vertices.size() / 3;
			vertices.push_back(get<0>(vertAdd));
			vertices.push_back(get<1>(vertAdd));
			vertices.push_back(get<2>(vertAdd));

			normals.push_back(get<0>(normAdd));
			normals.push_back(get<1>(normAdd));
			normals.push_back(get<2>(normAdd));

			texcoord.push_back(get<0>(texAdd));
			texcoord.push_back(get<1>(texAdd));

			size_t index = faceConvert.size();
			indices.push_back(index);
			faceConvert[face] = index;
		}
		else {
			indices.push_back(faceConvert[face]);
		}
	}
	g->setVertexData(vertices);
	g->setNormalData(normals);
	g->setTexCoordData(texcoord);
	g->setIndices(indices);
	return g;
}

void ObjFileParser::cleanup() {
	vertData.clear();
	textureCoordData.clear();
	vertexNormData.clear();
	faceData.clear();
}

tuple<size_t, size_t, size_t> ObjFileParser::parseFace(string face) {
	size_t vertexInd = 0;
	size_t normalInd = 0;
	size_t texCoordInd = 0;
	size_t* dataLocs[3] = { &vertexInd, &normalInd, &texCoordInd };
	size_t prevPos = 0;
	for (size_t num = 0, pos = 0; num < 3 && prevPos != string::npos; num++) {
		pos = face.find("/", pos);
		istringstream numStringStream = istringstream(face.substr(prevPos, pos));
		numStringStream >> *dataLocs[num];
		prevPos = pos;
	}
	
	return make_tuple(vertexInd, normalInd, texCoordInd);
}