#include <Windows.h>
#include "Game.h"
#include "Core/OmegaEngine.h"
#include "Core/Entity.h"
#include "Core/Test/TestComponent.h"
#include "Core/Test/TestDerivedComponent.h"
#include "Core/ComponentManager.h"
#include "MainScene.h"
#include "Core/EntityManager.h"
#include "Core/Example/ExampleComponent.h"
#include "Core/Example/ExampleSystem.h"

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

	EntityManager::Instance();

	// testing componentmanager types
	ComponentManager<TestComponent>& cm = ComponentManager<TestComponent>::Instance();
	ComponentManager<UpdatableComponent>& ucm = ComponentManager<UpdatableComponent>::Instance();
	ComponentManager<TestDerivedComponent>& tdm = ComponentManager<TestDerivedComponent>::Instance();
	// ComponentManager<ExampleComponent>::Instance();	// lazy initialize
	//ComponentManager<Component>& ccm = ComponentManager<Component>::Instance();
	

	// TESTS: transparent instant execution
	Entity* parent1 = new Entity();
	Entity* child1 = new Entity();
	Entity* parent2 = new Entity();
	Entity* child2 = new Entity();
	TestComponent* tc = new TestComponent(nullptr);
	TestDerivedComponent* tdc = new TestDerivedComponent();

	auto sizeShouldBe4 = EntityManager::Instance().GetEntities();
	SDL_assert(sizeShouldBe4.size() == 4 && "EntityManager failed (1)");

	// test 
	parent1->AddChild(child1);

	SDL_assert(parent1->GetChildren().size() == 1, "Instant add failed.");

	child1->SetParent(parent2);

	SDL_assert(parent1->GetChildren().size() == 0, "Instant move failed (was not removed from existing parent).");
	SDL_assert(parent2->GetChildren().size() == 1, "Instant move failed (was not moved to new parent).");

	parent2->SetEnabled(false);

	SDL_assert(parent2->GetEnabled() == false, "Instant disable failed");

	// component test 
	parent1->AddComponent(tc);
	parent2->AddComponent(tdc);

	SDL_assert(parent1->GetComponents().size() == 1 && "Component add failed (1)");
	SDL_assert(parent2->GetComponents().size() == 1 && "Component add failed (2)");

	parent1->RemoveComponent(tc);	
	parent2->RemoveComponent<TestComponent>();	// should still work despite derived

	SDL_assert(parent1->GetComponents().size() == 0 && "Component remove failed (1)");
	SDL_assert(parent2->GetComponents().size() == 0 && "Component remove failed (2)");

	// cleanup 
	delete(parent1);
	delete(child1);
	delete(parent2);
	delete(child2);

	auto sizeShouldBe0 = EntityManager::Instance().GetEntities();
	SDL_assert(sizeShouldBe0.size() == 0 && "EntityManager failed (1)");

	// TESTS: deferred execution 
	
	// init 
	OmegaEngine::Instance().initialize();

	Scene* s = new MainScene();

	OmegaEngine::Instance().ChangeScene(s);	// use fast transition

	parent1 = new Entity();
	parent2 = new Entity();
	child1 = new Entity();
	child2 = new Entity();

	parent1->AddChild(child1);
	parent2->AddChild(child2);

	auto testComponent = new TestComponent(parent2);
	auto derivedComponent = new TestDerivedComponent();

	parent1->AddComponent(testComponent);
	parent2->AddComponent(derivedComponent);

	// test
	SDL_assert(s->root.GetChildren().size() == 0, "Deferred execution initial state failed.");

	auto shouldBeNull = parent1->GetComponent<TestDerivedComponent>();
	auto shouldBeOkay = parent2->GetComponent<TestComponent>();

	SDL_assert(shouldBeNull == nullptr, "Component retrieval failed (1)");
	SDL_assert(shouldBeOkay != nullptr, "Component retrieval failed (2)");

	// deferred add 
	s->root.AddChild(parent1);
	s->root.AddChild(parent2);

	SDL_assert(s->root.GetChildren().size() == 0, "Deferred execution failed.");

	// component manager test
	// this is how you should be creating components!
	auto* t = ComponentManager<TestComponent>::Instance().Create<TestComponent>(new Entity());
	auto* t1 = ComponentManager<TestComponent>::Instance().Create<TestDerivedComponent>(new Entity());
	// not like this lol
	TestComponent test(new Entity);
	delete t;

	auto ec1 = ComponentManager<ExampleComponent>::Instance().Create<ExampleComponent>();
	auto ec2 = ComponentManager<ExampleComponent>::Instance().Create<ExampleComponent>();
	auto ec3 = ComponentManager<ExampleComponent>::Instance().Create<ExampleComponent>();
	auto ec4 = ComponentManager<ExampleComponent>::Instance().Create<ExampleComponent>();

	parent1->AddComponent(ec1);
	parent2->AddComponent(ec2);
	child1->AddComponent(ec3);	// this is weird, don't move child too lol
	child2->AddComponent(ec4);

	ExampleSystem exampleSystem;

	OmegaEngine::Instance().AddSystem(&exampleSystem);

	OmegaEngine::Instance().Loop();
}

int main() 
{
	Test_ECS();

	while (true) {}
}