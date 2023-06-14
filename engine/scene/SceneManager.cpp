#include "SceneManager.h"
#include <cassert>

void SceneManager::Update()
{
	//切り替え機構
	//次のシーンの予約があるなら
	if (nextscene)
	{
		//旧シーンの終了
		if (currentscene) {
			currentscene->End();
			delete currentscene;
		}

		//切り替え
		currentscene = nextscene;
		nextscene = nullptr;

		//シーンマネージャを現在シーンにセット
		currentscene->SetManager(*this);
		//次シーンを初期化
		currentscene->Initialize();
	}

	//実行中シーンの更新
	currentscene->Update();
}

void SceneManager::Draw()
{
	//実行中シーンの描画
	currentscene->Draw();
}

void SceneManager::End()
{
	delete currentscene;
}

//void SceneManager::ChangeScene(const std::string& sceneName)
//{
//	assert(sceneFactory);
//	assert(nextscene == nullptr);
//
//	//次のシーンを生成
//	nextscene = sceneFactory->CreateScene(sceneName);
//}
