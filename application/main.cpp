#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>
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

#include "GameScene.h"
#include "DemoScene.h"

#include "Sound.h"
#include "DebugText.h"

#include "Particle.h"
#include "MathF.h"

#include "ImguiManager.h"
#include <string>

//windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

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

	WinAPI* winApi = WinAPI::GetInstance();
	DirectX12* DX12 = DirectX12::GetInstance();

#ifdef  _DEBUG
	ID3D12InfoQueue* infoQueue;
	if (SUCCEEDED(DX12->device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);//ヤバイエラー時にとまと
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR,true);	 //エラー時に止まる
		infoQueue->Release();
	}
#endif  _DEBUG

	Input* input_ = Input::GetInstance();

	input_->Initialize();

	///---DirectX初期化処理　ここまで---///

	TimeManager::Init();

#pragma region 描画初期化処理

	//depth関連の処理
	CreateDepthView();

	//テクスチャの読み込み
	TextureManager* tManager = TextureManager::GetInstance();
	tManager->Initialize();
	tManager->PreLoad();

	//デバッグテキスト生成
	std::unique_ptr<Texture> debugFont = std::make_unique<Texture>();
	debugFont->Load(L"Resources/debugfont.png");

	DebugText debugText;
	debugText.Initialize(debugFont.get());

	//imguiの初期化
	ImguiManager *imguiManager = ImguiManager::GetInstance();
	imguiManager->Initialize();

	SpriteCommon::spriteCommon.Initialize();

	//モデルの読み込み
	ModelManager::GetInstance()->PreLoad();

	Game gameScene_;
	gameScene_.Initialize();

	DemoScene demo;
	demo.Initialize();

#pragma endregion 描画初期化処理

	SoundManager *soundManager = SoundManager::GetInstance();
	soundManager->Initialize();

	SoundData curser = soundManager->SoundLoadWave("Resources\\sound\\curser.wav");
	SoundData bgm = soundManager->SoundLoadWave("Resources\\sound\\bgm.wav");

	//ゲームループ内で使う変数の宣言
	//ParticleEmitter pEmitter;
	//pEmitter.Initialize();
	//pEmitter.SetInfo({ -10,-10,20 }, 10, 5,{0,0.5f,1,1}, 1, true);

	//soundManager->SoundPlayWave(bgm,true);

	GUI gui("GameSceneGUI");

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
	
		gui.Begin({ 800,10 }, { 400,200 });
		//gameScene_.Update();
		gui.End();

		demo.Update();

		//pEmitter.Update();

		///---DirectX毎フレーム処理 ここまで---///
#pragma endregion DirectX毎フレーム処理

#pragma region グラフィックスコマンド
		//--4.描画コマンドここから--//
		//gameScene_.Draw();
		demo.Draw();

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

	gameScene_.End();

	//音声データは先にxAudio2を解放しなければならない
	//xAudio2の解放
	soundManager->End();

	//音声データの解放
	soundManager->SoundUnload(&curser);
	soundManager->SoundUnload(&bgm);

	//ウィンドウクラスを登録解除
	UnregisterClass(winApi->w.lpszClassName, winApi->w.hInstance);

	return 0;
}