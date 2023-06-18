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

#include "PostEffectt.h"

//①文字列変換とサウンドデータを除き、使用していない
//②自身のコード内ではint、shortはすべてint32_tなどに置き換えた charは一部でまだ使用している
//③確認できる限りはconst& もしくは値渡しに置き換えた
//④文字列変換とサウンドデータを除き、使用していない
//⑤命名規則は以下の通り
//メンバ変数->手前にm, mの次は大文字(例:mHoge)
//スタティック変数->手前にs, sの次は大文字(例:sHoge)
//定数->全部大文字、区分けしたいときは_を付ける(例:HOGE_HOGE_DANCE)
//その他(namespace内など)->普通のキャメルケース

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

	//デバッグテキスト生成
	std::unique_ptr<Texture> debugFont = std::make_unique<Texture>();
	debugFont->Load(*L"Resources/debugfont.png");

	DebugText debugText;
	debugText.Initialize(*debugFont);

	//imguiの初期化
	ImguiManager *imguiManager = ImguiManager::GetInstance();
	imguiManager->Initialize();

	SpriteCommon::sSpriteCommon.Initialize();

	//モデルの読み込み
	ModelManager::Get()->PreLoad();

	//ライティングの初期化
	LightGroup::Create();

	//ポストエフェクトの初期化
	std::unique_ptr<PostEffect> postEffect;
	postEffect = std::make_unique<PostEffect>();
	postEffect->Initialize();

	//シーンの初期化
	SceneManager *scenemanager = SceneManager::Get();
	scenemanager->ChangeScene<DemoScene>();

#pragma endregion 描画初期化処理

	//音の初期化
	SoundManager *soundManager = SoundManager::GetInstance();
	soundManager->Initialize();

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

#pragma region DirectX毎フレーム処理
		///---DirectX毎フレーム処理 ここから---///
		
		ClearDrawScreen();

		imguiManager->PreUpdate();

		//更新処理
		Input::Keyboard::Get()->Update();
		Input::Mouse::Update();
		Input::Pad::Update();
		
		//gameScene_.Update();


		scenemanager->Update();

		//pEmitter.Update();

		///---DirectX毎フレーム処理 ここまで---///
#pragma endregion DirectX毎フレーム処理

#pragma region グラフィックスコマンド
		//--4.描画コマンドここから--//

		scenemanager->Draw();
		postEffect->Draw();

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