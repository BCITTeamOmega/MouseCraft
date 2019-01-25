#include "Component.h"
#include <iostream>


Component::Component(std::type_index t) : _type(t)
{
	std::cout << "Component created" << std::endl;
}

Component::~Component()
{
	std::cout << "Component destroyed" << std::endl;
}
