#include "SceneManager.h"
#include "SceneChange.h"
#include <cassert>
#include "InstantDrawer.h"

void SceneManager::Update()
{
	//溜め込んだ描画実行を破棄
	InstantDrawer::DrawInit();

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
		
		//リソース読み込み
		mCurrentscene->LoadResource();

		//次シーンを初期化
		mCurrentscene->Initialize();

		//暗転を解除
		if (SceneChange::Get()->IsBlackOut())
		{
			SceneChange::Get()->Open();
		}
	}

	//実行中シーンの更新
	mCurrentscene->Update();

	SceneChange::Get()->Update();
}

void SceneManager::Draw()
{
	//実行中シーンの描画
	mCurrentscene->Draw();

	//簡易描画クラスの毎フレーム更新
	InstantDrawer::AllUpdate();
	//簡易描画クラスの描画
	InstantDrawer::AllDraw2D();
	//ビルボードが手前に来るように
	InstantDrawer::AllDraw3D();

	//切り替え演出の描画
	SpriteCommonBeginDraw();
	SceneChange::Get()->Draw();
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
