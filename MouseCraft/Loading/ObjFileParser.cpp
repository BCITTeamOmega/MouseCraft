#include "ObjFileParser.h"
#include <sstream>
#include <GLEW/glew.h>
#include <iostream>

using std::tuple;
using std::make_tuple;
using std::istringstream;
using std::get;

ObjFileParser::ObjFileParser() : SimpleParser<Geometry>() {
	std::cout << "Setting up parsers" << std::endl;
	this->miniParsers["v"] = [this](string line) {
		string cmd; // Just to contain a "v"
		float x, y, z;
		istringstream stream = istringstream(line);
		if (stream >> cmd >> x >> y >> z) {
			vertData.push_back(x);
			vertData.push_back(y);
			vertData.push_back(z);
		}
		std::cout << "Vertex: " << x << ", " << y << "," << z << std::endl;
	};
	this->miniParsers["vt"] = [this](string line) {
		string cmd; // Just to contain a "v"
		float x, y;
		istringstream stream = istringstream(line);
		if (stream >> cmd >> x >> y) {
			textureCoordData.push_back(x);
			textureCoordData.push_back(y);
		}
		std::cout << "Texture Coordinate: " << x << y << std::endl;
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
		std::cout << "Normal: " << x << y << z << std::endl;
	};
	this->miniParsers["f"] = [this](string line) {
		string cmd; // Just to contain a "v"
		string p1, p2, p3;
		istringstream stream = istringstream(line);
		std::cout << "Face: " << std::endl;
		if (stream >> cmd >> p1 >> p2 >> p3) {
			faceData.push_back(parseFace(p1));
			faceData.push_back(parseFace(p2));
			faceData.push_back(parseFace(p3));
		}
	};
}

ObjFileParser::~ObjFileParser() {

}

Geometry ObjFileParser::build() {
	Geometry g = Geometry();
	return g;
}

void ObjFileParser::cleanup() {
	vertData.clear();
	textureCoordData.clear();
	vertexNormData.clear();
	faceData.clear();
}

tuple<int, int, int> ObjFileParser::parseFace(string face) {
	int vertexInd = 0;
	int normalInd = 0;
	int texCoordInd = 0;
	int* dataLocs[3] = { &vertexInd, &normalInd, &texCoordInd };
	size_t prevPos = 0;
	for (size_t num = 0, pos = 0; num < 3 && prevPos != string::npos; num++) {
		pos = face.find("/", pos);
		istringstream numStringStream = istringstream(face.substr(prevPos, pos));
		numStringStream >> *dataLocs[num];
		prevPos = pos;
	}
	std::cout << vertexInd << ", " << normalInd << ", " << texCoordInd << std::endl;
	
	return make_tuple(vertexInd, normalInd, texCoordInd);
}