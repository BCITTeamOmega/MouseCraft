#pragma once

#include "Loading/PrefabLoader.h"
#include "Core/Component.h"

// !!! include json support 
#include "json.hpp"
using json = nlohmann::json;

class TestPrefabComponent1 : public Component
{
private:
	int iunno;											
	double dawg;										

	// !!! function to create component
	static Component* Create(json json);

	// !!! register self to prefab loader
	static PrefabRegistrar reg;
};



// (just another example)
class TestPrefabComponent2 : public Component
{
private:
	bool bic;
	bool boi;
	static Component* Create(json json);
	static PrefabRegistrar reg;
};