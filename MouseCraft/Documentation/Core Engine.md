# Omega Engine - Usage
1. [Quick Start](#Quick-Start)
2. [Creating a System](#Creating-a-System)
3. [Creating an Updatable (Systemless) Component](#Creating-an-updatable-component)

For full details on each classes functionality, checkout the header file (or hover over the function in VS):
- [Entity.h](Core/Entity.h)
- [Component.h](Core/Component.h)
- [UpdatableComponent.h](Core/UpdatableComponent.h)
- [System.h](Core/System.h)
- [Scene.h](Core/Scene.h)
- [ComponentManager.h](Core/ComponentManager.h)
- [Entitymanager.h](Core/Entitymanager.h)
- [OmegaEngine.h](Core/OmegaEngine.h)

## Quick Start
```c++
// Call once for your game 
OmegaEngine::instance().Initialize();

// Creating an Entity. 
Entity* e1 = EntityManager::instance().Create(); 
Entity* e2 = EntityManager::instance().Create();
e1->addChild(e2);

// Creating a component 
MyComponent* c1 = ComponentManager<MyComponent>::instance().Create<MyComponent>();
MyDerivedComponent* c2 = ComponentManager<MyComponent>::instance().Create<MyDerivedComponent>();
e1->addComponent(c1);
e2->addComponent(c2);

// Creating a system 
MySystem s1 = new MySystem();
OmegaEngine::instance().addSystem(s1);

// Load a scene  
OmegaEngine::instance().transitionScene<MainScene>();

// add your entity to active scene 
OmegaEngine::instance().addEntity(e1);

/* 
// alternatively (not recommended)
Scene scene = new MainScene();
scene->root->addChild(e1);
OmegaEngine::instance().transitionScene(scene);
*/

// start your game!
OmegaEngine::instance().loop();
```

## Creating a System 
You want to create behaviour that affects **many entities** or requires knowledge of **all other similar components**. 

For example, lets consider a basic physics system.
```c++
// Create a "data" component 
class PhysicsComponent : public Component 
{
    glm::vec3 velocity;
    float mass;
    float drag;
    // etc.
}

// Create your system 
class PhysicsSystem : public System
{
    void Update(float deltaSeconds) override 
    {
        auto components = ComponentManager<PhysicsComponent>::Instance().All();    
        for (auto& from : components)
        {
            if (!c.isActive()) continue;
            from->GetEntity()->transform.translate(from->velocity * deltaSeconds);
        }
    }
}

// Give it a spin 
int main()
{
    ... 
    
    // add system to update list 
    OmegaEngine::Instance().AddSystem(new PhysicsSystem());
    
    // make some components 
    Entity e1, e2, e3;  // for demonstration - don't do this
    e1.addComponent( ComponentManager<PhysicsComponent>::Instance().Create() );
    e2.addComponent( ComponentManager<PhysicsComponent>::Instance().Create() );
    e3.addComponent( ComponentManager<PhysicsComponent>::Instance().Create() );
    
    // you're done!
}

```

## Creating an Updatable Component 
You want to create behaviour that exists in **only one or a few entities** that only **affects itself or a few other elements** without the burden of creating an entire system.

For example you want an item to explode after a set duration, you could do something like:
```c++
// Create the component 
class DynamiteComponent : public UpdatableComponent // Not just Component!
{
public:
    DynamiteComponent(float delay) : delay(delay) {}
    
    // only called if component isActive()    
    void Update(float deltaSeconds) override 
    {
        counter += deltaSeconds;
        if (counter >= delay)
        {
            vfx->Play();
            sfx->Play();
            GetEntity()->Destroy();
        }
    }
    
    void onInitialized() override 
    {
        // called once when added to the active scene 
        vfx = GetEntity()->GetComponent<VfxComponent>();
        sfx = GetEntity()->GetComponent<SfxComponent>();
    }
    
private:
    float delay;
    float counter; 
    // pretend these exist!
    VfxComponent* vfx;
    SfxComponent* sfx;
}

// Give er a go
int main()
{
    Entity* myBrain = EntityManager::instance().Create();
    
    // Always use the UpdatableComponent manager!
    auto dc = ComponentManager<UpdatableComponent>::Instance() 
                    .Create<DynamiteComponent>(10); // throw in your arg(s)
    
    myBrain->AddComponent(dc);
    
    OmegaEngine::Instance().AddEntity(myBrain);
}
```

