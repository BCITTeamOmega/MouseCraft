#include "TestPrefab.h"

#include "Core/ComponentManager.h"

TestPrefabComponent1::TestPrefabComponent1() {}

// !!! define the creator function
Component* TestPrefabComponent1::Create(json json)
{
	auto c = ComponentManager<TestPrefabComponent1>::Instance().Create<TestPrefabComponent1>();
	c->iunno = json["iunno"].get<int>();
	c->dawg = json["dawg"].get<double>();
	return c;
}

// !!! which key you want to load 
PrefabRegistrar TestPrefabComponent1::reg("TestComponent1", &TestPrefabComponent1::Create);





TestPrefabComponent2::TestPrefabComponent2() {}

Component * TestPrefabComponent2::Create(json json)
{
	auto c = ComponentManager<TestPrefabComponent2>::Instance().Create<TestPrefabComponent2>();
	c->bic = json["bic"].get<bool>();
	c->boi = json["boi"].get<bool>();
	return c;
}

PrefabRegistrar TestPrefabComponent2::reg("TestComponent2", &TestPrefabComponent2::Create);