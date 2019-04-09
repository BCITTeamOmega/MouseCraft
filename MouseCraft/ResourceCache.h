#pragma once

#include <map>
#include <string>

template<class T>
class ResourceCache
{
public:
	static T* Get(std::string key);
	static void Add(std::string key, T* value);

private:
	static std::map<std::string, T*> _cache;
};

