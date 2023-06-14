#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <iostream>
#include <memory>

//自分でクラス化したやつ
#include "WinAPI.h"
#include "TimeManager.h"

#include "Result.h"
#include "Input.h"
#include "DirectXInit.h"
#include "Vertex.h"

#include "ViewProjection.h"
#include "Texture.h"

#include "Sprite.h"
#include "Pipeline.h"
#include "ClearDrawScreen.h"
#include "Billboard.h"

#include "DemoScene.h"
#include "GameScene.h"
#include <FBXLoadDemoScene.h>
#include "SceneFactory.h"

#include "SceneManager.h"

#include "Sound.h"
#include "DebugText.h"

#include "Particle.h"
#include "MathF.h"

#include "ImguiManager.h"
#include <string>

//構成がx64のみに確認したらなってなかったので削除
//ソリューションプラットフォームがx64のみ
//警告をエラーとして扱う様に設定
//警告をすべて削除
//リンカ警告をエラーとして扱うように設定
//日本語のファイル名はもともと入っていないぜ

//windowsアプリでのエントリーポイント(main関数)
int32_t WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int32_t nShowCmd)
{
	///---DirectX初期化処理　ここから---///
	
#ifdef  _DEBUG
//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		//これより前にデバイスを生成するとデバイスが消されるらしい
		debugController->SetEnableGPUBasedValidation(TRUE);
	}

#endif _DEBUG

	WinAPI* winApi = WinAPI::Get();
	DirectX12* DX12 = DirectX12::Get();

#ifdef  _DEBUG
	ID3D12InfoQueue* infoQueue;
	if (SUCCEEDED(DX12->device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	//ヤバイエラー時にとまと
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING,true);		//ワーニング時もとまと
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR,true);		//エラー時にとまと
		infoQueue->Release();
	}
#endif  _DEBUG

	Input::Keyboard* input_ = Input::Keyboard::Get();

	input_->Initialize();
	Input::Mouse::Initialize();

	///---DirectX初期化処理　ここまで---///

	TimeManager::Init();

#pragma region 描画初期化処理

	//depth関連の処理
	CreateDepthView();

	//テクスチャの読み込み
	TextureManager* tManager = TextureManager::Get();
	tManager->Initialize();
	tManager->PreLoad();

	//デバッグテキスト生成
	std::unique_ptr<Texture> debugFont = std::make_unique<Texture>();
	debugFont->Load(*L"Resources/debugfont.png");

	DebugText debugText;
	debugText.Initialize(*debugFont);

	//imguiの初期化
	ImguiManager *imguiManager = ImguiManager::GetInstance();
	imguiManager->Initialize();

	SpriteCommon::spriteCommon.Initialize();

	//モデルの読み込み
	ModelManager::Get()->PreLoad();

	LightGroup::Create();

	SceneManager *scenemanager = SceneManager::Get();
	
	//シーンファクトリーを生成し、マネージャにセット
	//SceneFactory *sceneFactory = new SceneFactory();
	//scenemanager->SetSceneFactory(sceneFactory);

	scenemanager->ChangeScene<DemoScene>();

#pragma endregion 描画初期化処理

	SoundManager *soundManager = SoundManager::GetInstance();
	soundManager->Initialize();

	//ゲームループ
	while (true){

#pragma region ウィンドウメッセージ
		if (PeekMessage(&winApi->msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&winApi->msg);		//キー入力メッセージの処理
			DispatchMessage(&winApi->msg);		//プロシージャにメッセージを送る
		}

		if (winApi->msg.message == WM_QUIT) {
			break;
		}
#pragma endregion ウィンドウメッセージ

#pragma region DirectX毎フレーム処理
		///---DirectX毎フレーム処理 ここから---///
		
		ClearDrawScreen();

		imguiManager->PreUpdate();

		//更新処理
		input_->Update();
		Input::Mouse::Update();
		
		//gameScene_.Update();


		scenemanager->Update();

		//pEmitter.Update();

		///---DirectX毎フレーム処理 ここまで---///
#pragma endregion DirectX毎フレーム処理

#pragma region グラフィックスコマンド
		//--4.描画コマンドここから--//
		//gameScene_.Draw();

		scenemanager->Draw();

		debugText.DrawAll();

		//--4.描画コマンドここまで--//

#pragma endregion グラフィックスコマンド

#pragma region 画面入れ替え

		imguiManager->PreDraw();

		imguiManager->Draw();

		PostDraw();

		debugText.PostDraw();

#pragma endregion 画面入れ替え

		TimeManager::Update();

		if (input_->PushKey(DIK_ESCAPE))
		{
			break;
		}
	}

	imguiManager->Finalize();

	//delete sceneFactory;

	//入力デバイスの解放
	Input::Mouse::Finalize();
	input_->Finalize();

	//音声データは先にxAudio2を解放しなければならない
	//xAudio2の解放
	soundManager->End();

	//ウィンドウクラスを登録解除
	UnregisterClass(winApi->w.lpszClassName, winApi->w.hInstance);

	return 0;
}