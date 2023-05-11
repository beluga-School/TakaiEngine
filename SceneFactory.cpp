#include "SceneFactory.h"
#include "DemoScene.h"
#include "GameScene.h"

IScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    //���̃V�[���𐶐�
    IScene* newScene = nullptr;
    
    if (sceneName == "DEMO")
    {
        newScene = new DemoScene();
    }
    else if (sceneName == "GAMEPLAY")
    {
        Game* game = new Game();
        newScene = game;
    }

    return newScene;
}
