#pragma once

#include <map>
#include <string>
#include <iostream>

template<class T>
class ResourceCache
{
// singleton 
public:
	static ResourceCache& Instance()
	{
		static ResourceCache<T> _instance;
		return _instance;
	}
	ResourceCache(ResourceCache const&) = delete;
	void operator=(ResourceCache const&) = delete;
private:
	ResourceCache() {};
	~ResourceCache() {};

public:
	T* Get(std::string key)
	{
		if (_cache.find(key) != _cache.end())
		{
			std::cout << "Resource cache found key: " << key << std::endl;
			return _cache[key];
		}
		else
		{
			std::cout << "Resource cache could not find key: " << key << std::endl;
			return nullptr;
		}
	}

	void Add(std::string key, T* value)
	{
		_cache[key] = value;
	}
	
	void Clear()
	{
		for (auto& kvp : _cache)
			delete(kvp.second);
		_cache.clear();
	}

private:
	std::map<std::string, T*> _cache;
};

