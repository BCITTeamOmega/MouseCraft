#pragma once

#include "PrefabLoader.h"
#include "Core/Component.h"

// !!! include json support 
#include "json.hpp"
using json = nlohmann::json;



// Example component 1
class TestPrefabComponent1 : public Component
{
public:
	// !!! custom json ctor 
	TestPrefabComponent1(json j);						
	TestPrefabComponent1();

private:
	int iunno;											
	double dawg;										

private:
	// !!! register self to prefab loader
	static PrefabRegistrar<TestPrefabComponent1> reg;
};



// Example component 2 (just another example)
class TestPrefabComponent2 : public Component
{
public:
	TestPrefabComponent2(json j);
	TestPrefabComponent2();

private:
	bool bic;
	bool boi;

private:
	static PrefabRegistrar<TestPrefabComponent2> reg;
};