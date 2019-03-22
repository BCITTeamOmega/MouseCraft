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
	std::vector<int>& getIndexIndices() {
		return indexIndices;
	}
private:
	std::vector<int> indexIndices;
};