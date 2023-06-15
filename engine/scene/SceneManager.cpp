#include "SceneManager.h"
#include <cassert>

void SceneManager::Update()
{
	//切り替え機構
	//次のシーンの予約があるなら
	if (mNextscene)
	{
		//旧シーンの終了
		if (mCurrentscene) {
			mCurrentscene->End();
		}

		//切り替え
		mCurrentscene.swap(mNextscene);
		mNextscene = nullptr;

		//シーンマネージャを現在シーンにセット
		mCurrentscene->SetManager(*this);
		//次シーンを初期化
		mCurrentscene->Initialize();
	}

	//実行中シーンの更新
	mCurrentscene->Update();
}

void SceneManager::Draw()
{
	//実行中シーンの描画
	mCurrentscene->Draw();
}

void SceneManager::End()
{
	
}

//void SceneManager::ChangeScene(const std::string& sceneName)
//{
//	assert(sceneFactory);
//	assert(nextscene == nullptr);
//
//	//次のシーンを生成
//	nextscene = sceneFactory->CreateScene(sceneName);
//}
