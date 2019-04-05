#include "MenuScene.h"

#include "Core/ComponentManager.h"
#include "Core/EntityManager.h"
#include "Core/OmegaEngine.h"
#include "Graphics/UIRenderable.h"
#include "HostScene.h"
#include "MenuController.h"

void MenuScene::InitScene() {
    std::string* menuGameStartTex = new std::string("res/textures/menu1.png");
    std::string* menuQuitTex = new std::string("res/textures/menu2.png");

    // Set up menu entities
    _menu = EntityManager::Instance().Create();
    Entity* startGameEntity = EntityManager::Instance().Create();
    Entity* quitEntity = EntityManager::Instance().Create();
    _menu->transform.setLocalPosition(glm::vec3(0.5, 0.6, 0));

    UIRenderable* startGameUIRend = ComponentManager<UIRenderable>::Instance().Create<UIRenderable>();
    startGameUIRend->setSize(0.6f, 0.2f);
    startGameUIRend->setTexture(menuGameStartTex);
    startGameUIRend->setColor(Color(1.0, 1.0, 1.0));
    startGameEntity->AddComponent(startGameUIRend);

    UIRenderable* quitUIRend = ComponentManager<UIRenderable>::Instance().Create<UIRenderable>();
    quitUIRend->setSize(0.6f, 0.2f);
    quitUIRend->setTexture(menuQuitTex);
    quitUIRend->setColor(Color(1.0, 1.0, 1.0));
    quitEntity->AddComponent(quitUIRend);

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
            _menu->Destroy();
            break;
        }
    });

    root.AddChild(_menu);
}

void MenuScene::Update(const float delta) {

}

void MenuScene::CleanUp() {
}