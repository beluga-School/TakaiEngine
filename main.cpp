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
#include "Result.h"
#include "Input.h"
#include "DirectXInit.h"
#include "Vertex.h"

#include "ViewProjection.h"
#include "Vector3.h"
#include "Texture.h"

#include "Sprite.h"
#include "Pipeline.h"
#include "ClearDrawScreen.h"

#include "GameScene.h"

#include "Sound.h"
#include "DebugText.h"

//windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	WinAPI* winApi = WinAPI::GetInstance();
	DirectX12 *DX12 = DirectX12::GetInstance();

	///---DirectX初期化処理　ここから---///
	
#ifdef  _DEBUG
//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}

#endif _DEBUG

#ifdef  _DEBUG
	ID3D12InfoQueue* infoQueue;
	if (SUCCEEDED(DX12->device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->GetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION);//ヤバイエラー時にとまと
		infoQueue->GetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR);	 //エラー時に止まる
		infoQueue->Release();
	}
#endif  _DEBUG

	Input* input_ = Input::GetInstance();

	//DirectInputの初期化
	input_->DirectInputInit();

	//キーボードデバイスの生成
	input_->DirectInputCreate();

	///---DirectX初期化処理　ここまで---///

#pragma region 描画初期化処理

	//--depthこっから

	CreateDepthView(*DX12);

	//--depthここまで

	GameScene gameScene_;

	Model cube = Cube();
	Model plane = Plate();
	Model triangle = Triangle();
	Model line = Line();

	Model triangleM = Model();
	Model skydome = Model();

	//頂点データ作成
	cube.CreateDefaultModel(*DX12);
	plane.CreateDefaultModel(*DX12);
	triangle.CreateDefaultModel(*DX12);
	line.CreateDefaultModel(*DX12);

	triangleM.CreateModel("triangle_mat", *DX12);
	skydome.CreateModel("skydome", *DX12);

	//WICテクスチャのロード
	const wchar_t* msg[4] = 
	{ L"Resources/zawa_sironuri.png", 
		L"Resources/slime.png",
		L"Resources/pizza_sironuri.png",
		L"Resources/tyusiten.png"
	};

	std::unique_ptr<Texture> zawa = std::make_unique<Texture>();
	zawa->Load(msg[0], *DX12);

	std::unique_ptr<Texture> slime = std::make_unique<Texture>();
	slime->Load(msg[1], *DX12);

	std::unique_ptr<Texture> pizza = std::make_unique<Texture>();
	pizza->Load(msg[2], *DX12);

	std::unique_ptr<Texture> tyusiten = std::make_unique<Texture>();
	tyusiten->Load(msg[3], *DX12);

	std::unique_ptr<Texture> sinomiya = std::make_unique<Texture>();
	sinomiya->Load(L"Resources/triangle_mat/tex1.png", *DX12);

	std::unique_ptr<Texture> white = std::make_unique<Texture>();
	white->CreateWhiteTexture(*DX12);

	PipelineSet object3dPipelineSet = CreateObject3DPipeline(*DX12);

	SpriteCommon spriteCommon;
	spriteCommon = SpriteCommonCreate(*DX12);

	//スプライト
	Sprite pizzaSprite;
	pizzaSprite = SpriteCreate(*DX12, pizza.get(), {0.5f,0.5f});
	SpriteInit(pizzaSprite, spriteCommon,{ 100,100}, 0);
	SpriteSetSize(pizzaSprite, { 100,100 });

	Sprite slimeSprite;
	slimeSprite = SpriteCreate(*DX12,slime.get(), { 0.5f,0.5f });
	SpriteInit(slimeSprite, spriteCommon,{ 200,200 }, 45,{1,0,1,1});

	//デバッグテキスト生成

	std::unique_ptr<Texture> debugFont = std::make_unique<Texture>();
	debugFont->Load(L"Resources/debugfont.png", *DX12);

	DebugText debugText;
	debugText.Initialize(*DX12, debugFont.get());

	const int kObjectCount = 3;
	std::unique_ptr<Obj3d[]> object3ds;
	object3ds = std::make_unique<Obj3d[]>(kObjectCount);

	for (int i = 0; i < kObjectCount; i++)
	{
		object3ds[i].Initialize();
	}

	//どのモデルの形を使うかを設定
	object3ds[0].SetModel(&triangleM);
	object3ds[1].SetModel(&plane);
	object3ds[2].SetModel(&skydome);

	object3ds[0].scale.x = 10.0f;
	object3ds[0].scale.y = 10.0f;

	object3ds[1].position.x = 0;
	object3ds[1].position.y = -10;
	object3ds[1].scale.x = 10.0f;
	object3ds[1].scale.y = 10.0f;
	object3ds[1].rotation.x += XMConvertToRadians(90.0f);
	
	//線のモデルを使う設定
	const int kLineCountX = 15;
	const int kLineCountY = 15;
	std::unique_ptr<Obj3d[]> LineX;
	LineX = std::make_unique<Obj3d[]>(kLineCountX);

	for (int i = 0; i < kLineCountX; i++)
	{
		LineX[i].Initialize();
		LineX[i].SetModel(&line);
		LineX[i].position.x += 10 * i - (10 * kLineCountX / 2);
	}

	std::unique_ptr<Obj3d[]> LineY;
	LineY = std::make_unique<Obj3d[]>(kLineCountY);

	for (int i = 0; i < kLineCountY; i++)
	{
		LineY[i].Initialize();
		LineY[i].SetModel(&line);
		//LineY[i].position.x += 10 * i;
		LineY[i].position.z += 10 * i - (10 * kLineCountY / 2);

		LineY[i].rotation.y += XMConvertToRadians(90.0f);
	}

	//射影変換行列(投資投影)
	XMMATRIX matProjection =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),
			(float)window_width / window_height,
			0.1f, 1000.0f
		);

	//ビュー変換行列(透視投影)を計算
	ViewProjection viewProjection_;
	viewProjection_.Initialize();

	viewProjection_.eye.y = 50.0f;
	viewProjection_.UpdatematView();

#pragma endregion 描画初期化処理

	SoundManager soundManager;
	soundManager.Initialize();

	SoundData curser = soundManager.SoundLoadWave("Resources\\sound\\curser.wav");

	gameScene_.Initialize();

	//ゲームループ内で使う変数の宣言

	float angle = 0.0f;	//カメラの回転角
	float angleY = 0.0f;

	float cameraY = 100;

	float count = 0;

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
		
		ClearDrawScreen(*DX12);

		//更新処理

		gameScene_.Update();

		input_->Update();

		if (input_->TriggerKey(DIK_SPACE))
		{
			//soundManager.SoundPlayWave(curser);
		}

		//カメラ座標を動かす
		if (input_->PushKey(DIK_RIGHT) || 
			input_->PushKey(DIK_LEFT) || 
			input_->PushKey(DIK_UP) || 
			input_->PushKey(DIK_DOWN)||
			input_->PushKey(DIK_SPACE))
		{
			if (input_->PushKey(DIK_RIGHT)) { 
				viewProjection_.eye.x += 2;
				//viewProjection_.target.x += 2;
			}
			if (input_->PushKey(DIK_LEFT)) { 
				viewProjection_.eye.x -= 2; 
				//viewProjection_.target.x -= 2;
			}
			if (input_->PushKey(DIK_UP)) {
				viewProjection_.eye.z += 2;
				//viewProjection_.target.z += 2;
			}
			if (input_->PushKey(DIK_DOWN)) {
				viewProjection_.eye.z -= 2;
				//viewProjection_.target.z -= 2;
			}
			if (input_->PushKey(DIK_SPACE))
			{
				if (input_->PushKey(DIK_RIGHT)) {
				
					viewProjection_.target.x += 2;
				}
				if (input_->PushKey(DIK_LEFT)) {
					
					viewProjection_.target.x -= 2;
				}
				if (input_->PushKey(DIK_UP)) {
					
					viewProjection_.target.z += 2;
				}
				if (input_->PushKey(DIK_DOWN)) {
					
					viewProjection_.target.z -= 2;
				}
			}
			viewProjection_.UpdatematView();
		}

		if (input_->TriggerKey(DIK_R))
		{
			for (int i = 0; i < kObjectCount; i++)
			{
				object3ds[i].Initialize();
			}
			viewProjection_.Initialize();
			viewProjection_.eye.y = 50.0f;
			viewProjection_.UpdatematView();
		}

		//相対距離を求める
		XMFLOAT3 offset = { 0,0,0 };
		offset.x = viewProjection_.eye.x - object3ds[0].position.x;
		offset.y = viewProjection_.eye.y - object3ds[0].position.y;
		offset.z = viewProjection_.eye.z - object3ds[0].position.z;

		viewProjection_.eye.x = object3ds[0].position.x + offset.x;
		viewProjection_.eye.y = object3ds[0].position.y + offset.y;
		viewProjection_.eye.z = object3ds[0].position.z + offset.z;

		viewProjection_.UpdatematView();
		//オブジェクトの更新
		for (size_t i = 0; i < kObjectCount; i++)
		{
			if (input_->PushKey(DIK_D))
			{
				object3ds[i].position.x += 1;
			}
			if (input_->PushKey(DIK_A))
			{
				object3ds[i].position.x -= 1;
			}
			if (input_->PushKey(DIK_W))
			{
				object3ds[i].position.z += 1;
			}
			if (input_->PushKey(DIK_S))
			{
				object3ds[i].position.z -= 1;
			}
			if (input_->PushKey(DIK_Q))
			{
				object3ds[i].rotation.y += 0.1f;
			}
			if (input_->PushKey(DIK_E))
			{
				object3ds[i].rotation.y -= 0.1f;
			}
			object3ds[i].Update(viewProjection_.matView, matProjection);
		}

		for (int i = 0; i < kLineCountX; i++)
		{
			LineX[i].Update(viewProjection_.matView, matProjection);
		}
		for (int i = 0; i < kLineCountY; i++)
		{
			LineY[i].Update(viewProjection_.matView, matProjection);
		}

		SpriteUpdate(pizzaSprite, spriteCommon);
		SpriteUpdate(slimeSprite, spriteCommon);

		debugText.Print(spriteCommon, "Hello,DirectX", 200,100,1.0f);
		debugText.Print(spriteCommon, "Nihon Kogakuin", 200,200,2.0f);

		///---DirectX毎フレーム処理 ここまで---///
#pragma endregion DirectX毎フレーム処理

#pragma region グラフィックスコマンド
		//--4.描画コマンドここから--//
		PreDraw(*DX12, object3dPipelineSet);

		//描画処理
		gameScene_.Draw();

		object3ds[0].MaterialDraw(DX12->commandList.Get());
		//object3ds[0].MaterialDraw(DX12.commandList.Get());
		object3ds[2].MaterialDraw(DX12->commandList.Get());
		
		if (input_->TriggerKey(DIK_SPACE))
		{
			object3ds[1].Draw(DX12->commandList.Get(), slime.get());
		}

		for (int i = 0; i < kLineCountX; i++)
		{
			LineX[i].Draw(DX12->commandList.Get(), white.get());
		}
		for (int i = 0; i < kLineCountY; i++)
		{
			LineY[i].Draw(DX12->commandList.Get(), white.get());
		}

		//スプライトの前描画(共通コマンド)
		SpriteCommonBeginDraw(*DX12, spriteCommon);

		//スプライト単体描画
		SpriteDraw(pizzaSprite, *DX12);
		SpriteDraw(slimeSprite, *DX12);

		debugText.DrawAll(*DX12, spriteCommon);

		//--4.描画コマンドここまで--//

#pragma endregion グラフィックスコマンド

#pragma region 画面入れ替え

		PostDraw(*DX12);

#pragma endregion 画面入れ替え

		if (input_->PushKey(DIK_ESCAPE))
		{
			break;
		}
	}

	gameScene_.End();

	//音声データは先にxAudio2を解放しなければならない
	//xAudio2の解放
	soundManager.End();

	//音声データの解放
	soundManager.SoundUnload(&curser);

	//ウィンドウクラスを登録解除
	UnregisterClass(winApi->w.lpszClassName, winApi->w.hInstance);

	return 0;
}