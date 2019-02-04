#pragma once
#include <vector>
#include "SimpleParser.h"
#include "../Graphics/Geometry.h"

using std::vector;

class ObjFileParser :
	public SimpleParser<Geometry>
{
public:
	explicit ObjFileParser();
	virtual ~ObjFileParser() override;
protected:
	Geometry build() override;
	void cleanup() override;
private:
	vector<float> vertData;
	vector<float> textureCoordData;
	vector<float> vertexNormData;
	vector<std::tuple<int, int, int>> faceData;
	std::tuple<int, int, int> parseFace(string face);
};
