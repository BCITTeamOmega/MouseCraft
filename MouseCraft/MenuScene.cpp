#include "MenuScene.h"

#include "Core/ComponentManager.h"
#include "Core/EntityManager.h"
#include "Core/OmegaEngine.h"
#include "UI/UIComponent.h"
#include "UI/ImageComponent.h"
#include "UI/TextComponent.h"
#include "HostScene.h"
#include "MenuController.h"
#include "Sound/TrackParams.h"

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
    Entity* joinEntity = EntityManager::Instance().Create();
    _back = EntityManager::Instance().Create();
    _karen = EntityManager::Instance().Create();
    _menu->transform.setLocalPosition(glm::vec3(0.5, 0.6, 0));

    Entity *titleEntity = EntityManager::Instance().Create();
    TextComponent* titleRend = ComponentManager<UIComponent>::Instance().Create<TextComponent>("MOUSECRAFT", 15.0f, 0.0f, 0.1f, *font);
    titleRend->zForce = 0.03f;
    titleRend->SetSpacing(0.303f);
    titleRend->hAnchor = HorizontalAnchor::ANCHOR_HCENTER;
    titleRend->vAnchor = VerticalAnchor::ANCHOR_VCENTER;
    titleEntity->AddComponent(titleRend);

    Entity *titleBack = EntityManager::Instance().Create();
    ImageComponent* titleBackRend = ComponentManager<UIComponent>::Instance().Create<ImageComponent>(*boxTex, 100.0f, 15.0f, 0.0, 0.1f);
    titleBackRend->zForce = 0.05f;
    titleBackRend->color = Color(0.0f, 0.0f, 0.0f, 0.9f);
    titleBackRend->hAnchor = HorizontalAnchor::ANCHOR_HCENTER;
    titleBackRend->vAnchor = VerticalAnchor::ANCHOR_VCENTER;
    titleBack->AddComponent(titleBackRend);

    //ImageComponent* startGameUIRend = ComponentManager<UIComponent>::Instance().Create<ImageComponent>(*menuGameStartTex, 60.0f, 20.0f, 0.2f, 0.2f);
    TextComponent* startGameUIRend = ComponentManager<UIComponent>::Instance().Create<TextComponent>("Game Start", 7.5f, 0.0f, -0.175f, *font);
    startGameUIRend->zForce = 0.03f;
    startGameUIRend->SetSpacing(0.303f);
    startGameUIRend->hAnchor = HorizontalAnchor::ANCHOR_HCENTER;
    startGameUIRend->vAnchor = VerticalAnchor::ANCHOR_VCENTER;
    startGameEntity->AddComponent(startGameUIRend);

    TextComponent* joinGameRend = ComponentManager<UIComponent>::Instance().Create<TextComponent>("Join Game", 7.5f, 0.0f, -0.25f, *font);
    joinGameRend->zForce = 0.03f;
    joinGameRend->SetSpacing(0.303f);
    joinGameRend->hAnchor = HorizontalAnchor::ANCHOR_HCENTER;
    joinGameRend->vAnchor = VerticalAnchor::ANCHOR_VCENTER;
    joinEntity->AddComponent(joinGameRend);

    TextComponent* quitUIRend = ComponentManager<UIComponent>::Instance().Create<TextComponent>("Quit", 7.5f, 0.0f, -0.325f, *font);
    quitUIRend->zForce = 0.03f;
    quitUIRend->SetSpacing(0.303f);
    quitUIRend->hAnchor = HorizontalAnchor::ANCHOR_HCENTER;
    quitUIRend->vAnchor = VerticalAnchor::ANCHOR_VCENTER;
    quitEntity->AddComponent(quitUIRend);

    ImageComponent* backRend = ComponentManager<UIComponent>::Instance().Create<ImageComponent>(*boxTex, 100.0f, 22.5f, 0.0, -0.25f);
    backRend->zForce = 0.05f;
    backRend->color = Color(0.0f, 0.0f, 0.0f, 0.9f);
    backRend->hAnchor = HorizontalAnchor::ANCHOR_HCENTER;
    backRend->vAnchor = VerticalAnchor::ANCHOR_VCENTER;
    _back->AddComponent(backRend);

    ImageComponent* karenRend = ComponentManager<UIComponent>::Instance().Create<ImageComponent>(*karenTex, 30.628f, 90.0f, 0.0f, 0.0f);
    karenRend->zForce = 0.1f;
    karenRend->hAnchor = HorizontalAnchor::ANCHOR_HCENTER;
    karenRend->vAnchor = VerticalAnchor::ANCHOR_VCENTER;
    _karen->AddComponent(karenRend);

    //Create the menu component
    MenuController* mc = ComponentManager<MenuController>::Instance().Create<MenuController>();
    _menu->AddComponent(mc);
    mc->addMenuItem(startGameEntity);
    mc->addMenuItem(joinEntity);
    mc->addMenuItem(quitEntity);
    // NOTE: The menu controller's callback is set up at the end

    mc->onSelect([&](int menuItem) {
        switch (menuItem) {
        case 0: // Start Game
            OmegaEngine::Instance().ChangeScene<HostScene>();
            selectSong(MainBGM);
            break;
        case 1: // Join Game
            break;
        case 2: // Quit
            OmegaEngine::Instance().Stop();
            break;
        }
    });

    root.AddChild(titleEntity);
    root.AddChild(titleBack);
    root.AddChild(_menu);
    root.AddChild(_back);
    root.AddChild(_karen);
}

void MenuScene::Update(const float delta) {

}

void MenuScene::CleanUp() {
}