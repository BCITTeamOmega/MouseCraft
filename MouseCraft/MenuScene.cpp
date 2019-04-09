#include "MenuScene.h"

#include "Core/ComponentManager.h"
#include "Core/EntityManager.h"
#include "Core/OmegaEngine.h"
#include "UI/UIComponent.h"
#include "UI/ImageComponent.h"
#include "UI/TextComponent.h"
#include "HostScene.h"
#include "MenuController.h"

void MenuScene::InitScene() {
    std::string* menuGameStartTex = new std::string("res/textures/menu1.png");
    std::string* menuQuitTex = new std::string("res/textures/menu2.png");
	std::string* boxTex = new std::string("res/textures/blank.bmp");
	std::string* karenTex = new std::string("res/textures/araragi_karen.png");
	std::string* font = new std::string("res/fonts/ShareTechMono.png");

    // Set up menu entities
    _menu = EntityManager::Instance().Create();
    Entity* startGameEntity = EntityManager::Instance().Create();
    Entity* quitEntity = EntityManager::Instance().Create();
	_back = EntityManager::Instance().Create();
	_karen = EntityManager::Instance().Create();
    _menu->transform.setLocalPosition(glm::vec3(0.5, 0.6, 0));

    //ImageComponent* startGameUIRend = ComponentManager<UIComponent>::Instance().Create<ImageComponent>(*menuGameStartTex, 60.0f, 20.0f, 0.2f, 0.2f);
	TextComponent* startGameUIRend = ComponentManager<UIComponent>::Instance().Create<TextComponent>("Game Start", 10.0f, 0.0f, 0.2f, *font);
	startGameUIRend->zForce = 0.03f;
	startGameUIRend->SetSpacing(0.4f);
	startGameUIRend->hAnchor = HorizontalAnchor::ANCHOR_HCENTER;
    startGameEntity->AddComponent(startGameUIRend);

	TextComponent* quitUIRend = ComponentManager<UIComponent>::Instance().Create<TextComponent>("Quit", 10.0f, 0.0f, 0.3f, *font);
	quitUIRend->zForce = 0.03f;
	quitUIRend->SetSpacing(0.4f);
	quitUIRend->hAnchor = HorizontalAnchor::ANCHOR_HCENTER;
    quitEntity->AddComponent(quitUIRend);

	ImageComponent* backRend = ComponentManager<UIComponent>::Instance().Create<ImageComponent>(*boxTex, 100.0f, 20.0f, 0.0f, 0.2f);
	backRend->zForce = 0.05f;
	backRend->color = Color(0.0f, 0.0f, 0.0f, 0.9f);
	_back->AddComponent(backRend);

	ImageComponent* karenRend = ComponentManager<UIComponent>::Instance().Create<ImageComponent>(*karenTex, 32.5f, 90.0f, 0.0f, 0.0f);
	karenRend->zForce = 0.1f;
	karenRend->hAnchor = HorizontalAnchor::ANCHOR_HCENTER;
	karenRend->vAnchor = VerticalAnchor::ANCHOR_VCENTER;
	_karen->AddComponent(karenRend);

    //Create the menu component
    MenuController* mc = ComponentManager<MenuController>::Instance().Create<MenuController>();
    _menu->AddComponent(mc);
    mc->addMenuItem(startGameEntity);
    mc->addMenuItem(quitEntity);
    // NOTE: The menu controller's callback is set up at the end

    mc->onSelect([&](int scene) {
        std::cout << "Scene switch" << std::endl;
        switch (scene) {
        case 0:
            OmegaEngine::Instance().ChangeScene(new HostScene());
            break;
        }
    });

    root.AddChild(_menu);
	root.AddChild(_back);
	root.AddChild(_karen);
}

void MenuScene::Update(const float delta) {

}

void MenuScene::CleanUp() {
}