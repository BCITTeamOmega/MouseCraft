#include "HostScene.h"

#include "Core/ComponentManager.h"
#include "Core/EntityManager.h"
#include "Core/OmegaEngine.h"
#include "Core/UpdatableComponent.h"
#include "Loading/ModelLoader.h"
#include "Graphics/Camera.h"
#include "Graphics/Light.h"
#include "Graphics/ModelGen.h"
#include "Graphics/Renderable.h"
#include "Physics/PhysicsManager.h"
#include "Animation.h"
#include "Cat.h"
#include "GameManager.h"
#include "HealthComponent.h"
#include "Mouse.h"
#include "PickupSpawner.h"
#include "TransformAnimator.h"

void HostScene::InitScene() {
    //Make the entities
    Entity* mouse1Entity = EntityManager::Instance().Create();
    Entity* mouse2Entity = EntityManager::Instance().Create();
    Entity* mouse3Entity = EntityManager::Instance().Create();
    Entity* catEntity = EntityManager::Instance().Create();
    Entity* floorEntity = EntityManager::Instance().Create();
    floorEntity->transform.setLocalPosition(glm::vec3(50, 0, 37.5));
    Entity* counter1Entity = EntityManager::Instance().Create();
    Entity* counter2Entity = EntityManager::Instance().Create();
    Entity* islandEntity = EntityManager::Instance().Create();
    Entity* tableEntity = EntityManager::Instance().Create();
    Entity* couchEntity = EntityManager::Instance().Create();
    Entity* catstandEntity = EntityManager::Instance().Create();
    Entity* northWallEntity = EntityManager::Instance().Create();
    Entity* southWallEntity = EntityManager::Instance().Create();
    Entity* westWallEntity = EntityManager::Instance().Create();
    Entity* eastWallEntity = EntityManager::Instance().Create();
    Entity* cameraEntity = EntityManager::Instance().Create();
    cameraEntity->transform.setLocalPosition(glm::vec3(50, 30, 40));
    cameraEntity->transform.setLocalRotation(glm::vec3(-1.5f, 0, 0));
    Entity* pSpawnerEntity = EntityManager::Instance().Create();
    Entity* gmEntity = EntityManager::Instance().Create();
    Entity* light1Entity = EntityManager::Instance().Create();
    Entity* light2Entity = EntityManager::Instance().Create();

    //Make the models
    //Player Models
    Model* mouseModel = ModelLoader::loadModel("res/models/rat_tri.obj");
    Model* catModel = ModelLoader::loadModel("res/models/cat_tri.obj");
    //Map Models
    Model* floorModel = ModelGen::makeQuad(ModelGen::Axis::Y, 100, 75);
    Model* counter1Model = ModelGen::makeCube(10, 5, 40);
    Model* counter2Model = ModelGen::makeCube(50, 5, 10);
    Model* islandModel = ModelGen::makeCube(35, 5, 20);
    Model* tableModel = ModelGen::makeCube(20, 5, 35);
    Model* couchModel = ModelGen::makeCube(40, 5, 15);
    Model* catstandModel = ModelGen::makeCube(15, 5, 15);
    Model* horizWallModel = ModelGen::makeCube(110, 10, 5);
    Model* vertWallModel = ModelGen::makeCube(5, 10, 85);
    //Obstacle Models
    Model* ball = ModelLoader::loadModel("res/models/test/teapot.obj"); // ball temp
    Model* cylinder = ModelLoader::loadModel("res/models/test/Cylinder.obj"); // vase / lamp temp
    Model* box = ModelGen::makeCube(4, 4, 4);
    Model* book = ModelGen::makeCube(2, 2, 1);

    //Set the textures
    std::string* woodTex = new std::string("res/textures/wood.png");
    floorModel->setTexture(woodTex);
    horizWallModel->setTexture(woodTex);
    vertWallModel->setTexture(woodTex);

    //Create the camera
    Camera* cam = ComponentManager<Camera>::Instance().Create<Camera>();
    cam->setFOV(90.0f);
    cam->setCloseClip(0.01f);
    cam->setFarClip(100.0f);
    cameraEntity->AddComponent(cam);

    //Create the renderables, set their model and colour, and add them to their entity
    Renderable* mouse1Rend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    mouse1Rend->setModel(*mouseModel);
    mouse1Rend->setColor(Color(0.52, 0.24, 0.20));
    mouse1Entity->AddComponent(mouse1Rend);

    Renderable* mouse2Rend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    mouse2Rend->setModel(*mouseModel);
    mouse2Rend->setColor(Color(0.16, 0.18, 0.45));
    mouse2Entity->AddComponent(mouse2Rend);

    Renderable* mouse3Rend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    mouse3Rend->setModel(*mouseModel);
    mouse3Rend->setColor(Color(0.19, 0.42, 0.17));
    mouse3Entity->AddComponent(mouse3Rend);

    Renderable* catRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    catRend->setModel(*catModel);
    catRend->setColor(Color(1.0, 0.25, 0.5));
    catEntity->AddComponent(catRend);

    Renderable* floorRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    floorRend->setModel(*floorModel);
    floorRend->setColor(Color(1.0, 1.0, 1.0));
    floorEntity->AddComponent(floorRend);

    Renderable* counter1Rend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    counter1Rend->setModel(*counter1Model);
    counter1Rend->setColor(Color(1.0, 0.5, 0.0));
    counter1Entity->AddComponent(counter1Rend);

    Renderable* counter2Rend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    counter2Rend->setModel(*counter2Model);
    counter2Rend->setColor(Color(1.0, 0.5, 0.0));
    counter2Entity->AddComponent(counter2Rend);

    Renderable* islandRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    islandRend->setModel(*islandModel);
    islandRend->setColor(Color(1.0, 0.5, 0.0));
    islandEntity->AddComponent(islandRend);

    Renderable* tableRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    tableRend->setModel(*tableModel);
    tableRend->setColor(Color(1.0, 0.5, 0.0));
    tableEntity->AddComponent(tableRend);

    Renderable* couchRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    couchRend->setModel(*couchModel);
    couchRend->setColor(Color(1.0, 0.5, 0.0));
    couchEntity->AddComponent(couchRend);

    Renderable* catstandRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    catstandRend->setModel(*catstandModel);
    catstandRend->setColor(Color(1.0, 0.5, 0.0));
    catstandEntity->AddComponent(catstandRend);

    Renderable* northWallRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    northWallRend->setModel(*horizWallModel);
    northWallRend->setColor(Color(1.0, 0.5, 0.0));
    northWallEntity->AddComponent(northWallRend);

    Renderable* southWallRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    southWallRend->setModel(*horizWallModel);
    southWallRend->setColor(Color(1.0, 0.5, 0.0));
    southWallEntity->AddComponent(southWallRend);

    Renderable* westWallRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    westWallRend->setModel(*vertWallModel);
    westWallRend->setColor(Color(1.0, 0.5, 0.0));
    westWallEntity->AddComponent(westWallRend);

    Renderable* eastWallRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    eastWallRend->setModel(*vertWallModel);
    eastWallRend->setColor(Color(1.0, 0.5, 0.0));
    eastWallEntity->AddComponent(eastWallRend);

    //Create PhysicsManager and tell it how big the world is
    PhysicsManager::instance()->setupGrid(100, 75, 5);

    //Create the physics components
    PhysicsComponent* mouse1Physics = PhysicsManager::instance()->createObject(2.5, 45, 5, 5, 0, PhysObjectType::MOUSE_DOWN);
    mouse1Entity->AddComponent(mouse1Physics);
    mouse1Physics->initPosition();

    PhysicsComponent* mouse2Physics = PhysicsManager::instance()->createObject(7.5, 50, 5, 5, 0, PhysObjectType::MOUSE_DOWN);
    mouse2Entity->AddComponent(mouse2Physics);
    mouse2Physics->initPosition();

    PhysicsComponent* mouse3Physics = PhysicsManager::instance()->createObject(2.5, 55, 5, 5, 0, PhysObjectType::MOUSE_DOWN);
    mouse3Entity->AddComponent(mouse3Physics);
    mouse3Physics->initPosition();

    PhysicsComponent* catPhysics = PhysicsManager::instance()->createObject(77.5, 67.5, 8, 8, 0, PhysObjectType::CAT_UP);
    catEntity->AddComponent(catPhysics);
    catPhysics->initPosition();

    PhysicsComponent* counter1Physics = PhysicsManager::instance()->createGridObject(5, 20, 10, 40, PhysObjectType::PLATFORM);
    counter1Entity->AddComponent(counter1Physics);
    counter1Physics->initPosition();

    PhysicsComponent* counter2Physics = PhysicsManager::instance()->createGridObject(35, 5, 50, 10, PhysObjectType::PLATFORM);
    counter2Entity->AddComponent(counter2Physics);
    counter2Physics->initPosition();

    PhysicsComponent* islandPhysics = PhysicsManager::instance()->createGridObject(37.5, 30, 35, 20, PhysObjectType::PLATFORM);
    islandEntity->AddComponent(islandPhysics);
    islandPhysics->initPosition();

    PhysicsComponent* tablePhysics = PhysicsManager::instance()->createGridObject(80, 27.5, 20, 35, PhysObjectType::PLATFORM);
    tableEntity->AddComponent(tablePhysics);
    tablePhysics->initPosition();

    PhysicsComponent* couchPhysics = PhysicsManager::instance()->createGridObject(30, 67.5, 40, 15, PhysObjectType::PLATFORM);
    couchEntity->AddComponent(couchPhysics);
    couchPhysics->initPosition();

    PhysicsComponent* catstandPhysics = PhysicsManager::instance()->createGridObject(77.5, 67.5, 15, 15, PhysObjectType::PLATFORM);
    catstandEntity->AddComponent(catstandPhysics);
    catstandPhysics->initPosition();

    auto bookEntity = EntityManager::Instance().Create();
    Renderable* bookRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    bookRend->setModel(*book);
    bookRend->setColor(Color(1.0, 0.0, 1.0));
    bookEntity->AddComponent(bookRend);
    PhysicsComponent* bookPhysics = PhysicsManager::instance()->createGridObject(25, 5, 5, 5, PhysObjectType::OBSTACLE_UP);
    bookEntity->AddComponent(bookPhysics);
    bookPhysics->initPosition();

    auto boxEntity = EntityManager::Instance().Create();
    Renderable* boxRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    boxRend->setModel(*box);
    boxRend->setColor(Color(1.0, 0.0, 0.0));
    boxEntity->AddComponent(boxRend);
    PhysicsComponent* boxPhysics = PhysicsManager::instance()->createGridObject(50, 50, 7, 7, PhysObjectType::OBSTACLE_DOWN);
    boxEntity->AddComponent(boxPhysics);
    boxPhysics->initPosition();

    auto vaseEntity = EntityManager::Instance().Create();
    Renderable* vaseRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    vaseRend->setModel(*cylinder);
    vaseRend->setColor(Color(0.0, 1.0, 0.0));
    vaseEntity->AddComponent(vaseRend);
    PhysicsComponent* vasePhysics = PhysicsManager::instance()->createGridObject(30, 50, 5, 5, PhysObjectType::OBSTACLE_DOWN);
    vaseEntity->AddComponent(vasePhysics);
    vasePhysics->initPosition();

    auto lampEntity = EntityManager::Instance().Create();
    Renderable* lampRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    lampRend->setModel(*cylinder);
    lampRend->setColor(Color(1.0, 1.0, 0.0));
    lampEntity->AddComponent(lampRend);
    PhysicsComponent* lampPhysics = PhysicsManager::instance()->createGridObject(35, 20, 5, 5, PhysObjectType::OBSTACLE_UP);
    lampEntity->AddComponent(lampPhysics);
    lampPhysics->initPosition();

    auto ballEntity = EntityManager::Instance().Create();
    Renderable* ballRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    ballRend->setModel(*ball);
    ballRend->setColor(Color(1.0, 0.5, 0.5));
    ballEntity->AddComponent(ballRend);
    PhysicsComponent* ballPhysics = PhysicsManager::instance()->createGridObject(35, 30, 5, 5, PhysObjectType::OBSTACLE_UP);
    ballEntity->AddComponent(ballPhysics);
    ballPhysics->initPosition();

    root.AddChild(bookEntity);
    root.AddChild(boxEntity);
    root.AddChild(vaseEntity);
    root.AddChild(lampEntity);
    root.AddChild(ballEntity);

    PhysicsComponent* northWallPhysics = PhysicsManager::instance()->createObject(50, -2.5, 110, 5, 0, PhysObjectType::WALL);
    northWallEntity->AddComponent(northWallPhysics);
    northWallPhysics->initPosition();

    PhysicsComponent* southWallPhysics = PhysicsManager::instance()->createObject(50, 77.5, 110, 5, 0, PhysObjectType::WALL);
    southWallEntity->AddComponent(southWallPhysics);
    southWallPhysics->initPosition();

    PhysicsComponent* westWallPhysics = PhysicsManager::instance()->createObject(-2.5, 37.5, 5, 85, 0, PhysObjectType::WALL);
    westWallEntity->AddComponent(westWallPhysics);
    westWallPhysics->initPosition();

    PhysicsComponent* eastWallPhysics = PhysicsManager::instance()->createObject(102.5, 37.5, 5, 85, 0, PhysObjectType::WALL);
    eastWallEntity->AddComponent(eastWallPhysics);
    eastWallPhysics->initPosition();

    //Create other miscellaneous components for things
    //Mouse 1
    Mouse* mouse1Mouse = ComponentManager<UpdatableComponent>::Instance().Create<Mouse>();
    mouse1Mouse->speed = 50.0f;
    mouse1Entity->AddComponent(mouse1Mouse);

    PlayerComponent* mouse1Movement = ComponentManager<UpdatableComponent>::Instance().Create<PlayerComponent>();
    mouse1Movement->SetID(0); //Sets which controller handles this player (10 is the keyboard)
    mouse1Entity->AddComponent(mouse1Movement);

    HealthComponent* mouse1Health = ComponentManager<HealthComponent>::Instance().Create<HealthComponent>();
    mouse1Health->SetHealth(2);
    mouse1Entity->AddComponent(mouse1Health);

    SoundComponent* mouse1JumpSound = ComponentManager<SoundComponent>::Instance().Create<SoundComponent>(Jump);
    mouse1Entity->AddComponent(mouse1JumpSound);

    //Mouse 2
    Mouse* mouse2Mouse = ComponentManager<UpdatableComponent>::Instance().Create<Mouse>();
    mouse2Mouse->speed = 50.0f;
    mouse2Entity->AddComponent(mouse2Mouse);

    PlayerComponent* mouse2Movement = ComponentManager<UpdatableComponent>::Instance().Create<PlayerComponent>();
    mouse2Movement->SetID(1); //Sets which controller handles this player (10 is the keyboard)
    mouse2Entity->AddComponent(mouse2Movement);

    HealthComponent* mouse2Health = ComponentManager<HealthComponent>::Instance().Create<HealthComponent>();
    mouse2Health->SetHealth(2);
    mouse2Entity->AddComponent(mouse2Health);

    SoundComponent* mouse2JumpSound = ComponentManager<SoundComponent>::Instance().Create<SoundComponent>(Jump);
    mouse2Entity->AddComponent(mouse2JumpSound);

    //Mouse 3
    Mouse* mouse3Mouse = ComponentManager<UpdatableComponent>::Instance().Create<Mouse>();
    mouse3Mouse->speed = 50.0f;
    mouse3Entity->AddComponent(mouse3Mouse);

    PlayerComponent* mouse3Movement = ComponentManager<UpdatableComponent>::Instance().Create<PlayerComponent>();
    mouse3Movement->SetID(2); //Sets which controller handles this player (10 is the keyboard)
    mouse3Entity->AddComponent(mouse3Movement);

    HealthComponent* mouse3Health = ComponentManager<HealthComponent>::Instance().Create<HealthComponent>();
    mouse3Health->SetHealth(2);
    mouse3Entity->AddComponent(mouse3Health);

    SoundComponent* mouse3JumpSound = ComponentManager<SoundComponent>::Instance().Create<SoundComponent>(Jump);
    mouse3Entity->AddComponent(mouse3JumpSound);

    //Cat
    Cat* catCat = ComponentManager<UpdatableComponent>::Instance().Create<Cat>();
    catEntity->AddComponent(catCat);

    PlayerComponent* catMovement = ComponentManager<UpdatableComponent>::Instance().Create<PlayerComponent>();
    catMovement->SetID(10); //Sets which controller handles this player (10 is the keyboard)
    catEntity->AddComponent(catMovement);

    HealthComponent* catHealth = ComponentManager<HealthComponent>::Instance().Create<HealthComponent>();
    catEntity->AddComponent(catHealth);

    SoundComponent* catJumpSound = ComponentManager<SoundComponent>::Instance().Create<SoundComponent>(Jump);
    catEntity->AddComponent(catJumpSound);

    //Pickup Spawner
    PickupSpawner* pSpawnerSpawner = ComponentManager<UpdatableComponent>::Instance().Create<PickupSpawner>();
    pSpawnerEntity->AddComponent(pSpawnerSpawner);

    //Game Manager
    GameManager* gmGameManager = ComponentManager<UpdatableComponent>::Instance().Create<GameManager>();
    gmGameManager->AddMouse(mouse1Mouse);
    gmGameManager->AddMouse(mouse2Mouse);
    gmGameManager->AddMouse(mouse3Mouse);
    gmGameManager->SetCat(catCat);
    gmEntity->AddComponent(gmGameManager);

    // Lights
    Light* light1 = ComponentManager<Light>::Instance().Create<Light>();
    light1->setType(Light::LightType::Directional);
    light1->setColor(Color(1.2f, 1.25f, 0.8f));
    light1Entity->transform.setLocalRotation(glm::vec3(-1.1f, 0.8f, 0.0f));
    light1Entity->AddComponent(light1);

    Light* light2 = ComponentManager<Light>::Instance().Create<Light>();
    light2->setType(Light::LightType::Point);
    light2->setColor(Color(10.0f, 60.0f, 300.0f));
    light2->setAttenuation(1, 0.8, 0.32);
    light2Entity->transform.setLocalPosition(glm::vec3(13.0f, 0.0f, 0.25f));
    light2Entity->AddComponent(light2);

    //Don't forget the stupid teapots
    Entity* teapotEntity = PrefabLoader::LoadPrefab("res/prefabs/pot_army.json");
    teapotEntity->transform.setLocalPosition(glm::vec3(50, 0, 50));

	// Basic animations!
	Animation* squishSquashAnim = new Animation();
	squishSquashAnim->name = "idle";
	squishSquashAnim->duration = 4.0f;
	squishSquashAnim->AddScale(0.0f, glm::vec3(1.0f));
	squishSquashAnim->AddScale(2.0f, glm::vec3(1.0f, 1.2f, 1.0f));
	squishSquashAnim->AddScale(4.0f, glm::vec3(1.0f));
	squishSquashAnim->SetCurve(new SineConverter());

	TransformAnimator* mouse1Anim = ComponentManager<UpdatableComponent>::Instance().Create<TransformAnimator>();
	mouse1Anim->AddAnimation(squishSquashAnim);
	mouse1Entity->AddComponent(mouse1Anim);

	TransformAnimator* mouse2Anim = ComponentManager<UpdatableComponent>::Instance().Create<TransformAnimator>();
	mouse2Anim->AddAnimation(squishSquashAnim);
	mouse2Anim->SetProgress(0.5f);	// chnage progress to look a little different
	mouse2Entity->AddComponent(mouse2Anim);

	TransformAnimator* mouse3Anim = ComponentManager<UpdatableComponent>::Instance().Create<TransformAnimator>();
	mouse3Anim->AddAnimation(squishSquashAnim);
	mouse3Anim->SetProgress(0.8f);	// change progress to look a little different
	mouse3Entity->AddComponent(mouse3Anim);

    root.AddChild(mouse1Entity);
    root.AddChild(mouse2Entity);
    root.AddChild(mouse3Entity);
    root.AddChild(catEntity);
    root.AddChild(floorEntity);
    root.AddChild(counter1Entity);
    root.AddChild(counter2Entity);
    root.AddChild(islandEntity);
    root.AddChild(tableEntity);
    root.AddChild(couchEntity);
    root.AddChild(catstandEntity);
    root.AddChild(northWallEntity);
    root.AddChild(southWallEntity);
    root.AddChild(westWallEntity);
    root.AddChild(eastWallEntity);
    root.AddChild(pSpawnerEntity);
    root.AddChild(gmEntity);
    root.AddChild(cameraEntity);
    root.AddChild(light1Entity);
    root.AddChild(light2Entity);
}

void HostScene::CleanUp() {

}