#pragma once
class TextureInfo {
public:
	TextureInfo() :
		id(0),
		width(1),
		height(1),
		scaled(false)
	{}
	TextureInfo(int id, bool scaled, int width, int height) :
		id(id),
		width(width),
		height(height),
		scaled(scaled)
	{}
	int id;
	bool scaled;
	int width;
	int height;
};