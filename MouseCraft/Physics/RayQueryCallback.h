#include <Box2D/Box2D.h>
#include <vector>
#include "PhysObjectType.h"
#include "PhysicsComponent.h"

class RayQueryCallback : public b2RayCastCallback
{
public:
	std::vector<b2Body*> hitBodies;
	std::vector<float32> fractions;

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
	{
		hitBodies.push_back(fixture->GetBody());
		fractions.push_back(fraction);

		return 1; //check the entire ray for things
	}
};