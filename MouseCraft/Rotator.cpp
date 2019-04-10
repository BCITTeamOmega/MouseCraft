#include "Rotator.h"

#include "Core/Entity.h"

Rotator::Rotator()
{
}

Rotator::~Rotator()
{
}

void Rotator::Update(float deltaTime)
{
	GetEntity()->transform.rotate(rotationSpeed * deltaTime);
}

Component * Rotator::Create(json json)
{
	auto c = ComponentManager<UpdatableComponent>::Instance().Create<Rotator>();
	c->rotationSpeed = glm::vec3(
		json["speed"][0].get<float>(),
		json["speed"][1].get<float>(),
		json["speed"][2].get<float>());
	return c;
}

PrefabRegistrar Rotator::reg("Rotator", &Rotator::Create);