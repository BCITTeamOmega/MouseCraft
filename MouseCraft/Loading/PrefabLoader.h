#pragma once

#include "../Core/Component.h"
#include "../Core/Entity.h"
#include "../Core/EntityManager.h"
#include "../Core/ComponentManager.h"
#include "../json.hpp"
#include "../ResourceCache.h"
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using json = nlohmann::json;

// datatype for string_key -> loader()
typedef std::map<std::string, Component*(*)(json)> ComponentMap;


// Loads an entity with components and child entities specified by a json file. 
class PrefabLoader
{
public:
	PrefabLoader();
	~PrefabLoader();
	
	// Dumps all registered loaders
	static void DumpLoaders()
	{
		std::cout << "===== PREFABLOADER - REGISTERED LOADERS =====" << std::endl;
		for (auto& kvp : *getMap())
		{
			std::cout << kvp.first << std::endl;
		}
		std::cout << "===== END =====" << std::endl;
	}

	// Load an entity with a json file
	static Entity* LoadPrefab(std::string path)
	{
		std::string* data = ResourceCache<std::string>::Instance().Get(path);
		json json;

		// check if prefab was loaded before (avoid accessing disk)
		if (data == nullptr)
		{
			// open file and parse json
			std::ifstream ifs(path);
			if (!ifs.good())
			{
				std::cerr << "ERROR: PrefabLoader could not find file: " << path << std::endl;
				return nullptr;
			}
			std::stringstream ss;
			ss << ifs.rdbuf();
			ResourceCache<std::string>::Instance().Add(path, new std::string(ss.str()));
			
			json = json::parse(ss.str());
		}
		else
		{
			json = json::parse(*data);
		}
		
		// load prefab 
		auto parent = Load(json, nullptr);

		// return result
		return parent;
	};

	static Component* LoadComponent(std::string path)
	{
		std::string* data = ResourceCache<std::string>::Instance().Get(path);
		json json;

		// check if prefab was loaded before (avoid accessing disk)
		if (data == nullptr)
		{
			// open file and parse json
			std::ifstream ifs(path);
			if (!ifs.good())
			{
				std::cerr << "ERROR: PrefabLoader could not find component file: " << path << std::endl;
				return nullptr;
			}
			std::stringstream ss;
			ss << ifs.rdbuf();
			ResourceCache<std::string>::Instance().Add(path, new std::string(ss.str()));

			json = json::parse(ss.str());
		}
		else
		{
			json = json::parse(*data);
		}

		// use custom loader 
		auto loader = getMap()->find(json["type"]);
		if (loader != getMap()->end())
		{
			auto c = loader->second(json);
			if (json.find("enabled") != json.end())
			{
				c->SetEnabled(json["enabled"].get<bool>());
			}
			return c;
		}
		else
		{
			throw "ERROR: PrefabLoader component type not registered";
		}
	}

private:
	// Recursive helper function to load an entity
	static Entity* Load(json json, Entity* parent)
	{
		// entity & transform 
		auto e = EntityManager::Instance().Create();
		
		auto pos = glm::vec3(json["pos"][0].get<double>(), json["pos"][1].get<double>(), json["pos"][2].get<double>());
		auto rot = glm::vec3(json["rot"][0].get<double>(), json["rot"][1].get<double>(), json["rot"][2].get<double>());
		auto scl = glm::vec3(json["scl"][0].get<double>(), json["scl"][1].get<double>(), json["scl"][2].get<double>());
		e->transform.setLocalPosition(pos);
		e->transform.setLocalRotation(rot);
		e->transform.setLocalScale(scl);

		// components 
		auto components = json["components"];
		for (auto& j : components)
		{
			// use custom loader 
			auto loader = getMap()->find(j["type"]);
			if (loader != getMap()->end())
			{
				e->AddComponent(loader->second(j));
			}
			else
			{
				throw "ERROR: PrefabLoader component type not registered";
			}
		}

		// children 
		auto children = json["entities"];
		for (auto& j : children)
		{
			e->AddChild(Load(j, e));
		}

		return e;
	}

protected:
	static ComponentMap* getMap()
	{
		if (!componentMap)
			componentMap = new ComponentMap;
		return componentMap;
	}
private:
	static ComponentMap* componentMap;
};

// Helper class to automatically register a Component class to the PrefabLoader 
struct PrefabRegistrar : PrefabLoader
{
// note: Doesn't actually inherit from PrefabLoader just need access to the ComponentMap
public:
	PrefabRegistrar(const std::string& jsonKey, Component*(*func)(json))
	{
		getMap()->insert(std::make_pair(jsonKey, func));
	};
};

/* Below doesn't work as we use CM's. With the updated ComponentFactory it would work.

// used for generic component ctor (doesn't work as we use CM's)
template<typename T> Component* CreateComponent(json json)
{
	return new T(json);
}

// Helper class to automatically register a Component class to the PrefabLoader 
template<typename T>
struct PrefabRegistrar : PrefabLoader
{
	// note: Doesn't actually inherit from PrefabLoader just need access to the ComponentMap
public:
	PrefabRegistrar(const std::string& jsonKey)
	{
		getMap()->insert(std::make_pair(jsonKey, &CreateComponent<T>));
	};
};
*/