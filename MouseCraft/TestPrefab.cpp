#include "TestPrefab.h"



TestPrefabComponent1::TestPrefabComponent1(json j)
{
	iunno = j["iunno"].get<int>();
	dawg = j["dawg"].get<double>();
}
TestPrefabComponent1::TestPrefabComponent1()
{
}
// !!! which key you want to load 
PrefabRegistrar<TestPrefabComponent1> TestPrefabComponent1::reg("TestComponent1");




TestPrefabComponent2::TestPrefabComponent2(json j)
{
	bic = j["bic"].get<bool>();
	boi = j["boi"].get<bool>();
}
TestPrefabComponent2::TestPrefabComponent2()
{
}
PrefabRegistrar<TestPrefabComponent2> TestPrefabComponent2::reg("TestComponent2");