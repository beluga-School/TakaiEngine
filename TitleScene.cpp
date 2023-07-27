#include "TitleScene.h"
#include "Input.h"
#include "SceneChange.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "ClearDrawScreen.h"
#include "Util.h"

void TitleScene::LoadResource()
{
	TextureManager::Load("Resources\\title_logo.png", "title_logo");

	titleUI.SetTexture(*TextureManager::GetTexture("title_logo"));
	titleUI.SetPos({ Util::WIN_WIDTH / 2, Util::WIN_HEIGHT / 2 });
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

	titleUI.Update();
}

void TitleScene::Draw()
{
	SpriteCommonBeginDraw();
	titleUI.Draw();
}

void TitleScene::End()
{

}
