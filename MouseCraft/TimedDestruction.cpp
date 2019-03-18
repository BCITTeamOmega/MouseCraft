#include "TimedDestruction.h"

#include "Core/Entity.h"

TimedDestruction::TimedDestruction()
{
}


TimedDestruction::~TimedDestruction()
{
}

void TimedDestruction::Update(float deltaTime)
{
	_counter += deltaTime;
	if (_counter >= delay)
		GetEntity()->Destroy();
}
