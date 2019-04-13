# Component Factory
Below is a proposed structure of creating, deleting and managing Components. 

The main intent of this structure is for the COMPONENT_CREATED/DESTROYED event (which is not supported right now).

Another reason is to simplify the syntax.

However, this would obviously require refactoring the current component management which could take some time. 

## New Classes and Events
- ComponentFactory : To create and delete Components
- ComponentManager<T> : Stores and manages memory for a specific component type
- ComponentList<T> : Stores all components of base type. 
- Component : No change
- UpdatableComponent : No change
- EventName::COMPONENT_CREATED : Param is a pointer to the component
- EventName::COMPONENT_DESTROYED : Param is a pointer to the component

## Usage
```c++
// Create Components through the ComponentFactory
auto c1 = ComponentFactory::Create<ExampleComponent>();
auto c2 = ComponentFactory::Create<ExampleComponent>();
auto c3 = ComponentFactory::Create<ExampleComponent>();

auto ca = ComponentFactory::Create<ExampleDerivedComponent>();
auto cb = ComponentFactory::Create<ExampleDerivedComponent>();
auto cc = ComponentFactory::Create<ExampleDerivedComponent>();

// Managers allow direct access to component of type ONLY. 
// Note: Still of pointer type to prevent pointer invalidation.
auto contains_c_123 = ComponentManager<ExampleComponent>::Instance().All();
auto contains_c_abc = ComponentManager<ExampleDerivedComponent>::Instance().All();

// Lists allow convenient access to component (pointers) of type 
auto contains_c_123abc = ComponentList<ExampleComponent>::Instance().All();
auto contains_c_sameAsAbove = ComponentList<Component>::Instance().All();

// Delete Components through the ComponentFactory
ComponentFactory::Destroy(c1);
// don't do this! delete(c1);
```