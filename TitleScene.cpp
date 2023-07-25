#include "TitleScene.h"
#include "Input.h"
#include "SceneChange.h"
#include "SceneManager.h"
#include "GameScene.h"

void TitleScene::LoadResource()
{

}

void TitleScene::Initialize()
{
	sceneID = "Title";
}

void TitleScene::Update()
{
	if (Input::Keyboard::TriggerKey(DIK_SPACE))
	{
		SceneChange::Get()->Start();
	}
	if (SceneChange::Get()->IsBlackOut())
	{
		SceneManager::Get()->ChangeScene<GameScene>();
	}
}

void TitleScene::Draw()
{

}

void TitleScene::End()
{

}
