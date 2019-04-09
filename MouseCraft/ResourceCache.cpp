#include "ResourceCache.h"



template<class T>
T * ResourceCache<T>::Get(std::string key)
{
	if (_cache.find(key) != _cache.end())
		return _cache[key];
	else 
		return nullptr;
}

template<class T>
void ResourceCache<T>::Add(std::string key, T * value)
{
	_cache[key] = value;
}
