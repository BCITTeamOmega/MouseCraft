#include "System.h"

System::System()
{
}

System::System(std::vector<std::type_index> forComponents)
{
	for (auto const& type : forComponents)
	{
		_componentMap.emplace(type, std::vector<Component*>());
	}
}

System::~System()
{
	// b/c binding is used signal is automatically disconnected when this obj is destroyed.
}
