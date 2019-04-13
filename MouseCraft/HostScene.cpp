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
#include "ObstacleFactory.h"
#include "Network/NetworkSystem.h"
#include "HealthDisplay.h"
#include "Graphics/OutlineComponent.h"
#include "UI/ImageComponent.h"
#include "UI/TextComponent.h"
#include "TransformAnimator.h"
#define CAT_HEALTH 8

void HostScene::InitScene() {
    //Make the entities
    Entity* mouse1Entity = EntityManager::Instance().Create();
    Entity* mouse2Entity = EntityManager::Instance().Create();
    Entity* mouse3Entity = EntityManager::Instance().Create();
    Entity* catEntity = EntityManager::Instance().Create();
	catEntity->transform.setLocalScale(glm::vec3(1.8, 1.0, 1.2));
    Entity* catAttackEntity = EntityManager::Instance().Create();
    catAttackEntity->transform.setLocalPosition(glm::vec3(0, 2, -5));
    catAttackEntity->transform.setLocalRotation(glm::vec3(0, -M_PI/2, 0));
    catAttackEntity->transform.scale(4.4);
    catAttackEntity->SetEnabled(false);
    catEntity->AddChild(catAttackEntity);
    Entity* floorEntity = EntityManager::Instance().Create();
    floorEntity->transform.setLocalPosition(glm::vec3(50, 0, 37.5));
	Entity* counter1Entity = PrefabLoader::LoadPrefab("res/prefabs/environment/counter1.json");
	Entity* counter2Entity = PrefabLoader::LoadPrefab("res/prefabs/environment/counter2.json");
	Entity* islandEntity = PrefabLoader::LoadPrefab("res/prefabs/environment/island.json");
	Entity* tableEntity = PrefabLoader::LoadPrefab("res/prefabs/environment/table.json");
	Entity* couchEntity = PrefabLoader::LoadPrefab("res/prefabs/environment/couch.json");
    Entity* catstandEntity = PrefabLoader::LoadPrefab("res/prefabs/environment/catstand.json");
	Entity* bobRossEntity = EntityManager::Instance().Create();
	bobRossEntity->transform.setLocalPosition(glm::vec3(0, 6, 65));
    Entity* northWallEntity = EntityManager::Instance().Create();
	Entity* southWallEntity = EntityManager::Instance().Create();
    Entity* westWallEntity = EntityManager::Instance().Create();
    Entity* eastWallEntity = EntityManager::Instance().Create();
	Entity* cameraEntity = EntityManager::Instance().Create();
    cameraEntity->transform.setLocalPosition(glm::vec3(50, 50, 40));
    cameraEntity->transform.setLocalRotation(glm::vec3(-1.5f, 0, 0));
    Entity* pSpawnerEntity = EntityManager::Instance().Create();
    Entity* gmEntity = EntityManager::Instance().Create();
    Entity* light1Entity = EntityManager::Instance().Create();
    Entity* light2Entity = EntityManager::Instance().Create();
	Entity* healthUIEntity = EntityManager::Instance().Create();
	Entity* healthBarUIEntity = EntityManager::Instance().Create();
	Entity* recipeUIEntity = EntityManager::Instance().Create();
	Entity* recipeUIImgEntity = EntityManager::Instance().Create();

    //Make the models
    //Player Models
    Model* mouseModel = ModelLoader::loadModel("res/models/rat_tri.obj");
    Model* catModel = ModelLoader::loadModel("res/models/cat_tri.obj");
    Model* CatAttackModel = ModelLoader::loadModel("res/models/crescent.obj");
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
	Model* bobRossModel = ModelGen::makeCube(0.1, 3, 5);
    //Obstacle Models
    Model* ball = ModelLoader::loadModel("res/models/test/teapot.obj"); // ball temp
    Model* cylinder = ModelLoader::loadModel("res/models/test/Cylinder.obj"); // vase / lamp temp
    Model* box = ModelGen::makeCube(4, 4, 4);
    Model* book = ModelGen::makeCube(2, 2, 1);
	//Font
	std::string* font = new std::string("res/fonts/ShareTechMono.png");
	
    //Set the textures
    std::string* woodTex = new std::string("res/textures/wood.png");
	std::string* boxTex = new std::string("res/textures/blank.bmp");
	std::string* recipeTex = new std::string("res/textures/recipe_UI.png");
	std::string* bobRossTex = new std::string("res/textures/bob_ross.png");
    floorModel->setTexture(woodTex);
    horizWallModel->setTexture(woodTex);
    vertWallModel->setTexture(woodTex);
	bobRossModel->setTexture(bobRossTex);

    //Create the camera
    Camera* cam = ComponentManager<Camera>::Instance().Create<Camera>();
    cam->setFOV(1.51f);
    cam->setCloseClip(0.01f);
    cam->setFarClip(100.0f);
    cameraEntity->AddComponent(cam);

    //Create the renderables, set their model and colour, and add them to their entity
    Renderable* mouse1Rend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    mouse1Rend->setModel(*mouseModel);
    mouse1Rend->setColor(Color(0.46, 0.12, 0.08));
	mouse1Rend->setShininess(0.4);
	mouse1Rend->setRoughness(10.0);
    mouse1Entity->AddComponent(mouse1Rend);

    Renderable* mouse2Rend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    mouse2Rend->setModel(*mouseModel);
    mouse2Rend->setColor(Color(0.16, 0.18, 0.45));
	mouse2Rend->setShininess(0.4);
	mouse2Rend->setRoughness(10.0);
	mouse2Entity->AddComponent(mouse2Rend);

    Renderable* mouse3Rend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    mouse3Rend->setModel(*mouseModel);
    mouse3Rend->setColor(Color(0.19, 0.42, 0.17));
	mouse3Rend->setShininess(0.4);
	mouse3Rend->setRoughness(10.0);
    mouse3Entity->AddComponent(mouse3Rend);

    Renderable* catRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    catRend->setModel(*catModel);
	//catRend->setModel(*box);
    catRend->setColor(Color(0.34, 0.08, 0));
	catRend->setShininess(0.4);
	catRend->setRoughness(10.0);
    catEntity->AddComponent(catRend);

    Renderable* catAttackRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    catAttackRend->setModel(*CatAttackModel);
    catAttackRend->setColor(Color(1.0, 1.0, 1.0));
    catAttackEntity->AddComponent(catAttackRend);

    Renderable* floorRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
    floorRend->setModel(*floorModel);
    floorRend->setColor(Color(1.0, 1.0, 1.0));
    floorEntity->AddComponent(floorRend);

	/*
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
	*/

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

	Renderable* bobRossRend = ComponentManager<Renderable>::Instance().Create<Renderable>();
	bobRossRend->setModel(*bobRossModel);
	bobRossRend->setColor(Color(1.0, 1.0, 1.0));
	bobRossEntity->AddComponent(bobRossRend);

    //Create PhysicsManager and tell it how big the world is
    PhysicsManager::instance()->setupGrid(100, 75, 5);

    //Create the physics components
    PhysicsComponent* mouse1Physics = PhysicsManager::instance()->createObject(2.5, 45, 3, 3, 0, PhysObjectType::MOUSE_DOWN);
    mouse1Entity->AddComponent(mouse1Physics);
    mouse1Physics->initPosition();

    PhysicsComponent* mouse2Physics = PhysicsManager::instance()->createObject(7.5, 50, 3, 3, 0, PhysObjectType::MOUSE_DOWN);
    mouse2Entity->AddComponent(mouse2Physics);
    mouse2Physics->initPosition();

    PhysicsComponent* mouse3Physics = PhysicsManager::instance()->createObject(2.5, 55, 3, 3, 0, PhysObjectType::MOUSE_DOWN);
    mouse3Entity->AddComponent(mouse3Physics);
    mouse3Physics->initPosition();

    PhysicsComponent* catPhysics = PhysicsManager::instance()->createObject(77.5, 67.5, 4.5, 4.5, 0, PhysObjectType::CAT_UP);
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

	auto* bookEntity = ObstacleFactory::Instance().Create(OBSTACLES::BOOK, glm::vec3(7.5, 0, 32.5), true);
	auto* boxEntity = ObstacleFactory::Instance().Create(OBSTACLES::BOX, glm::vec3(50, 0, 50), false);
	auto* vaseEntity = ObstacleFactory::Instance().Create(OBSTACLES::VASE, glm::vec3(27.5, 0, 47.5), false);
	auto* lampEntity = ObstacleFactory::Instance().Create(OBSTACLES::LAMP, glm::vec3(37.5, 0, 22.5), true);
	auto* ballEntity = ObstacleFactory::Instance().Create(OBSTACLES::YARNBALL, glm::vec3(32.5, 0, 32.5), true);
	auto* lampEntity2 = ObstacleFactory::Instance().Create(OBSTACLES::LAMP, glm::vec3(72.5, 0, 47.5), false);

	Light* lampLight = ComponentManager<Light>::Instance().Create<Light>();
	lampLight->setType(Light::LightType::Point);
	lampLight->setColor(Color(2.5f, 2.1f, 0.6f));
	lampLight->setAttenuation(1, 0.08, 0.01);
	lampEntity->AddComponent(lampLight);

	Light* lampLight2 = ComponentManager<Light>::Instance().Create<Light>();
	lampLight2->setType(Light::LightType::Point);
	lampLight2->setColor(Color(2.5f, 2.1f, 0.6f));
	lampLight2->setAttenuation(1, 0.08, 0.01);
	lampEntity2->AddComponent(lampLight2);

    root.AddChild(bookEntity);
    root.AddChild(boxEntity);
    root.AddChild(vaseEntity);
    root.AddChild(lampEntity);
	root.AddChild(lampEntity2);
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
    mouse1Health->SetHealth(1);
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
    mouse2Health->SetHealth(1);
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
    mouse3Health->SetHealth(1);
    mouse3Entity->AddComponent(mouse3Health);

    SoundComponent* mouse3JumpSound = ComponentManager<SoundComponent>::Instance().Create<SoundComponent>(Jump);
    mouse3Entity->AddComponent(mouse3JumpSound);

    //Cat
    Cat* catCat = ComponentManager<UpdatableComponent>::Instance().Create<Cat>();
    catCat->Hitbox = catAttackEntity;
    catEntity->AddComponent(catCat);

    PlayerComponent* catMovement = ComponentManager<UpdatableComponent>::Instance().Create<PlayerComponent>();
    catMovement->SetID(10); //Sets which controller handles this player (10 is the keyboard)
    catEntity->AddComponent(catMovement);

    HealthComponent* catHealth = ComponentManager<HealthComponent>::Instance().Create<HealthComponent>();
	catHealth->SetHealth(CAT_HEALTH);
	catEntity->AddComponent(catHealth);

    SoundComponent* catJumpSound = ComponentManager<SoundComponent>::Instance().Create<SoundComponent>(Jump);
	catEntity->AddComponent(catJumpSound);

	// Network Components
	NetworkComponent *mouseNetwork = NetworkSystem::Instance()->CreateComponent(1);
	mouse1Entity->AddComponent(mouseNetwork);

	NetworkComponent *mouseNetwork2 = NetworkSystem::Instance()->CreateComponent(2);
	mouse2Entity->AddComponent(mouseNetwork2);

	NetworkComponent *mouseNetwork3 = NetworkSystem::Instance()->CreateComponent(3);
	mouse3Entity->AddComponent(mouseNetwork3);

	NetworkComponent *catNetwork = NetworkSystem::Instance()->CreateComponent(4);
	catEntity->AddComponent(catNetwork);

	NetworkComponent *catAtkNet = NetworkSystem::Instance()->CreateComponent(5);
	catAttackEntity->AddComponent(catAtkNet);

    //Pickup Spawner
    PickupSpawner* pSpawnerSpawner = ComponentManager<UpdatableComponent>::Instance().Create<PickupSpawner>();
	pSpawnerSpawner->spawnDelay = 2.0f;
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
    light1->setColor(Color(1.5f, 1.45f, 0.9f));
    light1Entity->transform.setLocalRotation(glm::vec3(-1.1f, 0.8f, 0.0f));
	light1Entity->AddComponent(light1);

	/*
    Light* light2 = ComponentManager<Light>::Instance().Create<Light>();
    light2->setType(Light::LightType::Point);
    light2->setColor(Color(0.5f, 1.0f, 2.5f));
    light2->setAttenuation(1, 0.0, 0.01);
    light2Entity->transform.setLocalPosition(glm::vec3(65.0f, 12.0f, 50.0f));
    light2Entity->AddComponent(light2);
	*/

	// Health Bar UI
	ImageComponent* healthImg = ComponentManager<UIComponent>::Instance().Create<ImageComponent>(*boxTex, 98.0f, 90.0f, 0.01f, 0.0f);
	healthImg->color = Color(1.0f, 0.1f, 0.1f, 1.0f);
	healthImg->zForce = 0.1;
	healthImg->vAnchor = VerticalAnchor::ANCHOR_VCENTER;
	healthBarUIEntity->AddComponent(healthImg);

	ImageComponent* healthBackImg = ComponentManager<UIComponent>::Instance().Create<ImageComponent>(*boxTex, 80.0f, 5.0f, 0.1f, 0.05f);
	healthBackImg->color = Color(0.0f, 0.0f, 0.0f, 0.8f);
	healthBackImg->zForce = 0.2;
	healthUIEntity->AddComponent(healthBackImg);
	healthUIEntity->AddChild(healthBarUIEntity);

	HealthDisplay* healthDisplayController = ComponentManager<HealthDisplay>::Instance().Create<HealthDisplay>(CAT_HEALTH);
	healthDisplayController->setHealthUI(healthImg);
	healthDisplayController->setWatchingHealthComponent(catHealth);
	healthUIEntity->AddComponent(healthDisplayController);

	//Contraption Recipe UI
	ImageComponent* recipeBackImg = ComponentManager<UIComponent>::Instance().Create<ImageComponent>(*boxTex, 18.0f, 34.0f, 0.81f, 0.33f);
	recipeBackImg->color = Color(0.6f, 0.6f, 0.6f, 0.4f);
	recipeBackImg->zForce = 0.2;
	recipeUIEntity->AddComponent(recipeBackImg);

	ImageComponent* recipeUIImg = ComponentManager<UIComponent>::Instance().Create<ImageComponent>(*recipeTex, 100.0f, 96.0f, 0.0f, 0.01f);
	//recipeUIImg->color = Color(0.0f, 0.0f, 0.0f, 1.0f);
	recipeUIImg->zForce = 0.19;
	recipeUIImgEntity->AddComponent(recipeUIImg);
	recipeUIEntity->AddChild(recipeUIImgEntity);

	//healthImg->vAnchor = VerticalAnchor::ANCHOR_VCENTER;
	
	//Add outlines to the player entities
	OutlineComponent* catOutline = ComponentManager<OutlineComponent>::Instance().Create<OutlineComponent>();
	catOutline->setWidth(0.17f);
	catOutline->setColor(Color(0.0f, 0.0f, 0.0f));
	catEntity->AddComponent(catOutline);

	OutlineComponent* mouse1Outline = ComponentManager<OutlineComponent>::Instance().Create<OutlineComponent>();
	mouse1Outline->setWidth(0.17f);
	mouse1Outline->setColor(Color(0.0f, 0.0f, 0.0f));
	mouse1Entity->AddComponent(mouse1Outline);

	OutlineComponent* mouse2Outline = ComponentManager<OutlineComponent>::Instance().Create<OutlineComponent>();
	mouse2Outline->setWidth(0.17f);
	mouse2Outline->setColor(Color(0.0f, 0.0f, 0.0f));
	mouse2Entity->AddComponent(mouse2Outline);

	OutlineComponent* mouse3Outline = ComponentManager<OutlineComponent>::Instance().Create<OutlineComponent>();
	mouse3Outline->setWidth(0.17f);
	mouse3Outline->setColor(Color(0.0f, 0.0f, 0.0f));
	mouse3Entity->AddComponent(mouse3Outline);

    //Don't forget the stupid teapots
	//Entity* teapotEntity = PrefabLoader::LoadPrefab("res/prefabs/pot_army.json");
	//Entity* jumpingTeapot = PrefabLoader::LoadPrefab("res/prefabs/jumping_teapot.json");
	//jumpingTeapot->transform.setLocalPosition(glm::vec3(60, 0, 50));
	//Entity* jumpingTeapot2 = PrefabLoader::LoadPrefab("res/prefabs/jumping_teapot.json");
	//jumpingTeapot2->transform.setLocalPosition(glm::vec3(60, 0, 40));

	// Basic animations!
	Animation* squishSquashAnim = new Animation();
	squishSquashAnim->name = "idle";
	squishSquashAnim->duration = 4.0f;
	squishSquashAnim->AddScale(0.0f, glm::vec3(1.0f));
	squishSquashAnim->AddScale(2.0f, glm::vec3(1.02f, 1.1f, 1.02f));
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

	TransformAnimator* catAnim = ComponentManager<UpdatableComponent>::Instance().Create<TransformAnimator>();
	catAnim->AddAnimation(squishSquashAnim);
	catAnim->SetSpeed(0.8f);
	catEntity->AddComponent(mouse3Anim);

	auto doorEntity = PrefabLoader::LoadPrefab("res/prefabs/environment/door.json");
	root.AddChild(doorEntity);

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
	root.AddChild(healthUIEntity);
	root.AddChild(recipeUIEntity);
	root.AddChild(bobRossEntity);

    /*
    Leaving mice enabled on release build for local play

    // Disable mice on start, to enable when players connect
	mouse1Entity->SetEnabled(false);
	mouse2Entity->SetEnabled(false);
	mouse3Entity->SetEnabled(false);
    */
}

void HostScene::CleanUp() {

}
