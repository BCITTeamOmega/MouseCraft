#pragma once
#include <vector>
#include "SimpleParser.h"
#include "../Graphics/Geometry.h"

using std::vector;
using std::tuple;

class ObjFileParser :
	public SimpleParser<Geometry*>
{
public:
	explicit ObjFileParser();
	virtual ~ObjFileParser() override;
protected:
	Geometry* build() override;
	void cleanup() override;
private:
	vector<float> vertData;
	vector<float> textureCoordData;
	vector<float> vertexNormData;
	vector<std::tuple<size_t, size_t, size_t>> faceData;
	std::tuple<size_t, size_t, size_t> parseFace(string face);
};

