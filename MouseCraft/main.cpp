#include <Windows.h>
#include "Game.h"
#include "Core/OmegaEngine.h"
#include "Core/Entity.h"
#include "MainScene.h"

/*
Test:
Entity should have instant action when not in active scene
- add/delete/move/enable/disable 

Entity actions should propogate
- add (must call initialize())
- delete
- enable/disable

Entity should have dferred action when interacting with active scene
- add/delete/move/enable/disable 

Special case 
- non-initialized added onto initialized 

Should be able to retrieve specific type of components 

Component initialize test 
- should be able to get reference 
*/
void Test_ECS()
{
	// transparent instant execution
	Entity* parent1 = new Entity();

	Entity* child1 = new Entity();

	Entity* parent2 = new Entity();

	Entity* child2 = new Entity();

	parent1->addChild(child1);

	assert(parent1->getChildren().size() == 1, "Instant add failed.");

	child1->setParent(parent2);

	assert(parent1->getChildren().size() == 0, "Instant move failed (was not removed from existing parent).");
	assert(parent2->getChildren().size() == 1, "Instant move failed (was not moved to new parent).");

	parent2->setEnabled(false);

	assert(parent2->getEnabled() == false, "Instant disable failed");

	// boot up engine 
	OmegaEngine::instance().initialize();

	Scene* s = new MainScene();

	OmegaEngine::instance().changeScene(s);	// use fast transition

	assert(s->root.getChildren().size() == 0, "Deferred execution initial state failed.");

	// deferred add 
	s->root.addChild(parent1);

	assert(s->root.getChildren().size() == 0, "Deferred execution failed.");

	OmegaEngine::instance().loop();

}

int main() 
{
	Test_ECS();

	while (true) {}
}