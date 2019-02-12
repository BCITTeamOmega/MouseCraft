#pragma once
#include <vector>
#include "SimpleParser.h"
#include "../Graphics/Geometry.h"

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
	std::vector<float> vertData;
	std::vector<float> textureCoordData;
	std::vector<float> vertexNormData;
	std::vector<std::tuple<size_t, size_t, size_t>> faceData;
	std::tuple<size_t, size_t, size_t> parseFace(std::string face);
};

