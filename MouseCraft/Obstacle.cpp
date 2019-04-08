#include "Obstacle.h"

#include "HealthComponent.h"

Obstacle::Obstacle() :
	HandleOnDeath(this, &Obstacle::DestroyedByMouse)
{
}

Obstacle::~Obstacle()
{
}

void Obstacle::OnInitialized()
{
	GetEntity()->GetComponent<HealthComponent>()->OnDeath.Attach(HandleOnDeath);
}
