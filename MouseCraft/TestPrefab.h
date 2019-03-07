#pragma once

#include "Loading/PrefabLoader.h"
#include "Core/Component.h"

// !!! include json support 
#include "json.hpp"
using json = nlohmann::json;



// Example component 1
class TestPrefabComponent1 : public Component
{
public:
	TestPrefabComponent1();

private:
	int iunno;											
	double dawg;										

private:
	// !!! function to create component
	static Component* Create(json json);

	// !!! register self to prefab loader
	static PrefabRegistrar reg;
};



// Example component 2 (just another example)
class TestPrefabComponent2 : public Component
{
public:
	TestPrefabComponent2();

private:
	bool bic;
	bool boi;

private:
	static Component* Create(json json);
	static PrefabRegistrar reg;
};