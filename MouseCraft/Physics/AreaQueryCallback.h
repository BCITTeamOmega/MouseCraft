#include <Box2D/Box2D.h>
#include <vector>

class AreaQueryCallback : public b2QueryCallback
{
public:
	std::vector<b2Body*> foundBodies;

	bool ReportFixture(b2Fixture* fixture)
	{
		foundBodies.push_back(fixture->GetBody());
		return true; //continues checking for more fixtures if it returns true
	}
};