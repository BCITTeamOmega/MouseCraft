#include <Windows.h>
#include "Game.h"
#include "Core/OmegaEngine.h"
#include "Core/Entity.h"
#include "Core/Example/TestComponent.h"
#include "Core/Example/TestDerivedComponent.h"
#include "MainScene.h"

void Test_ECS()
{
	// NOTE: Use SDL_assert b/c SDL2 is manhandling everything.

	/*
	Test:
	Entity should have instant action when not in active scene
	- add/delete/move/enable/disable

	Entity actions should propogate
	- add (must call initialize())
	- delete
	- enable/disable

	Entity should have deferred action when interacting with active scene
	- add/delete/move/enable/disable

	Special case
	- non-initialized added onto initialized

	Should be able to retrieve specific type of components

	Component initialize test
	- should be able to get reference
	*/

	// TESTS: transparent instant execution
	Entity* parent1 = new Entity();
	Entity* child1 = new Entity();
	Entity* parent2 = new Entity();
	Entity* child2 = new Entity();

	// test 
	parent1->addChild(child1);

	SDL_assert(parent1->getChildren().size() == 1, "Instant add failed.");

	child1->setParent(parent2);

	SDL_assert(parent1->getChildren().size() == 0, "Instant move failed (was not removed from existing parent).");
	SDL_assert(parent2->getChildren().size() == 1, "Instant move failed (was not moved to new parent).");

	parent2->setEnabled(false);

	SDL_assert(parent2->getEnabled() == false, "Instant disable failed");

	// cleanup 
	delete(parent1);
	delete(child1);
	delete(parent2);
	delete(child2);

	// TESTS: deferred execution 
	
	// init 
	OmegaEngine::instance().initialize();

	Scene* s = new MainScene();

	OmegaEngine::instance().changeScene(s);	// use fast transition

	parent1 = new Entity();
	parent2 = new Entity();
	child1 = new Entity();
	child2 = new Entity();

	auto testComponent = new TestComponent(parent2);
	auto derivedComponent = new TestDerivedComponent();

	parent1->addComponent(testComponent);
	parent2->addComponent(derivedComponent);

	// test
	SDL_assert(s->root.getChildren().size() == 0, "Deferred execution initial state failed.");

	auto shouldBeNull = parent1->getComponentReal<TestDerivedComponent>();
	auto shouldBeOkay = parent2->getComponentReal<TestComponent>();

	SDL_assert(shouldBeNull == nullptr, "Component retrieval failed (1)");
	SDL_assert(shouldBeOkay != nullptr, "Component retrieval failed (2)");

	// deferred add 
	s->root.addChild(parent1);
	s->root.addChild(parent2);

	SDL_assert(s->root.getChildren().size() == 0, "Deferred execution failed.");

	OmegaEngine::instance().loop();
}

int main() 
{
	Test_ECS();

	while (true) {}
}