#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "WinAPI.h"
#include "TimeManager.h"
#include "Input.h"
#include "DirectXInit.h"
#include "Camera.h"
#include "Texture.h"
#include "Sprite.h"
#include "Pipeline.h"
#include "ClearDrawScreen.h"
#include "SceneManager.h"
#include "Sound.h"
#include "ImguiManager.h"
#include "PostEffect.h"

#include "GameScene.h"
#include "ShaderTestScene.h"
#include "TitleScene.h"
#include "InstantDrawer.h"
#include "TestScene.h"

//命名規則は以下の通り
//メンバ変数->手前にm, mの次は大文字(例:mHoge)
//スタティック変数->手前にs, sの次は大文字(例:sHoge)
//定数->全部大文字、区分けしたいときは_を付ける(例:HOGE_HOGE_DANCE)
//その他(namespace内など)->普通のキャメルケース

//windowsアプリでのエントリーポイント(main関数)
int32_t WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int32_t nShowCmd)
{
	hInstance; 
	lpCmdLine;
	nShowCmd;
	hPrevInstance;
	///---DirectX初期化処理　ここから---///
	
#ifdef  _DEBUG
//デバッグレイヤーをオンに
	Microsoft::WRL::ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		//これより前にデバイスを生成するとデバイスが消されるらしい
		debugController->SetEnableGPUBasedValidation(false);
	}

#endif _DEBUG

	WinAPI* winApi = WinAPI::Get();

#ifdef  _DEBUG
	DirectX12* DX12 = DirectX12::Get();

	ID3D12InfoQueue* infoQueue;
	if (SUCCEEDED(DX12->mDevice->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	//ヤバイエラー時にとまと
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING,true);		//ワーニング時もとまと
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR,true);		//エラー時にとまと
		infoQueue->Release();
	}
#endif  _DEBUG

	Input::Keyboard::Get()->Initialize();
	Input::Mouse::Initialize();
	Input::Pad::Initialize();

	///---DirectX初期化処理　ここまで---///

	TimeManager::Init();

#pragma region 描画初期化処理

	//depth関連の処理
	CreateDepthView();

	//テクスチャの読み込み
	TextureManager* tManager = TextureManager::Get();
	tManager->Initialize();
	tManager->PreLoad();

	//imguiの初期化
	ImguiManager *imguiManager = ImguiManager::Get();
	imguiManager->Initialize();

	//モデルの読み込み
	ModelManager::Get()->PreLoad();

	//ライティングの初期化
	//LightGroup::Get()->Initialize();

	//ポストエフェクトの初期化
	std::unique_ptr<PostEffect> postEffect;
	postEffect = std::make_unique<PostEffect>();
	postEffect->Initialize();

	//音の初期化
	SoundManager* soundManager = SoundManager::Get();
	soundManager->Initialize();

	//シーン切り替え演出の初期化
	SceneChange::Get()->Initialize();

	//シーンの初期化
	SceneManager *scenemanager = SceneManager::Get();
	scenemanager->ChangeScene<TestScene>();

#pragma endregion 描画初期化処理

	//パイプラインの初期化
	PipelineManager::Get()->Initialize();

	SpriteCommon::sSpriteCommon.Initialize();

	InstantDrawer::PreCreate();

	//ゲームループ
	while (true){

#pragma region ウィンドウメッセージ
		if (PeekMessage(&winApi->mMsg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&winApi->mMsg);		//キー入力メッセージの処理
			DispatchMessage(&winApi->mMsg);		//プロシージャにメッセージを送る
		}

		if (winApi->mMsg.message == WM_QUIT) {
			break;
		}
#pragma endregion ウィンドウメッセージ

		///---DirectX毎フレーム処理 ここから---///
		ClearDrawScreen();

		imguiManager->PreUpdate();

		//更新処理
		Input::Keyboard::Get()->Update();
		Input::Mouse::Update();
		Input::Pad::Update();

		scenemanager->Update();
		postEffect->Update();

		///---DirectX毎フレーム処理 ここまで---///

		//--4.描画コマンドここから--//

#pragma region ポストエフェクト対応の描画方式

		postEffect->PreDrawScene();
		scenemanager->Draw();
		postEffect->PostDrawScene();

		PreDraw();

		postEffect->Draw();

		//--4.描画コマンドここまで--//

		//--5.画面入れかえ--//
		imguiManager->PreDraw();

		imguiManager->Draw();

		PostDraw();

#pragma endregion ポストエフェクト対応の描画方式

		//--5.画面入れかえおわり--//

		TimeManager::Update();

		if (Input::Keyboard::Get()->PushKey(DIK_ESCAPE))
		{
			break;
		}
	}

	imguiManager->Finalize();

	//入力デバイスの解放
	Input::Mouse::Finalize();
	Input::Keyboard::Get()->Finalize();

	//音声データは先にxAudio2を解放しなければならない
	//xAudio2の解放
	soundManager->End();

	//ウィンドウクラスを登録解除
	UnregisterClass(winApi->w.lpszClassName, winApi->w.hInstance);

	return 0;
}