#pragma once
#include "Geometry.h"
#include <vector>;
class CombinedGeometry : public Geometry {
public:
	int getIndexStart(int objectIndex) {
		return indexIndices[objectIndex];
	}
	int getIndexEnd(int objectIndex) {
		if (objectIndex == indexIndices.size() - 1) {
			return getIndices().size() - 1;
		}
		return indexIndices[objectIndex + 1] - 1;
	}
	int getVertexStart(int objectIndex) {
		return vertexIndices[objectIndex];
	}
	int getVertexEnd(int objectIndex) {
		if (objectIndex == indexIndices.size() - 1) {
			return (getVertexData().size() / 3) - 1;
		}
		return vertexIndices[objectIndex + 1] - 1;
	}
	std::vector<int>& getIndexIndices() {
		return indexIndices;
	}
	std::vector<int>& getVertexIndices() {
		return vertexIndices;
	}
	void clear() {
		getVertexData().clear();
		getTexCoordData().clear();
		getNormalData().clear();
		getIndices().clear();
		indexIndices.clear();
		vertexIndices.clear();
	}
private:
	std::vector<int> vertexIndices;
	std::vector<int> indexIndices;
};