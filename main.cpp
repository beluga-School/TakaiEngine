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
#include "Vector3.h"
#include "Texture.h"

#include "Sprite.h"
#include "Pipeline.h"
#include "ClearDrawScreen.h"
#include "Billboard.h"

#include "GameScene.h"

#include "Sound.h"
#include "DebugText.h"

#include "GeometryObject.h"


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

	TimeManager::Init();

#pragma region 描画初期化処理

	//--depthこっから

	CreateDepthView();

	//--depthここまで

	GameScene gameScene_;

	Model cube = Cube();
	Model plane = Plate();
	Model triangle = Triangle();
	Model line = Line();

	Model triangleM = Model();
	Model skydome = Model();

	//頂点データ作成
	cube.CreateDefaultModel();
	plane.CreateDefaultModel();
	triangle.CreateDefaultModel();
	line.CreateDefaultModel();

	//gitignoreの設定ミスでobjが送れてなかったよ
	triangleM.CreateModel("triangle_mat");
	skydome.CreateModel("skydome");

	//WICテクスチャのロード
	const wchar_t* msg[4] = 
	{ L"Resources/zawa_sironuri.png", 
		L"Resources/slime.png",
		L"Resources/pizza_sironuri.png",
		L"Resources/tyusiten.png"
	};

	std::unique_ptr<Texture> zawa = std::make_unique<Texture>();
	zawa->Load(msg[0]);

	std::unique_ptr<Texture> slime = std::make_unique<Texture>();
	slime->Load(msg[1]);

	std::unique_ptr<Texture> pizza = std::make_unique<Texture>();
	pizza->Load(msg[2]);

	std::unique_ptr<Texture> tyusiten = std::make_unique<Texture>();
	tyusiten->Load(msg[3]);

	std::unique_ptr<Texture> sinomiya = std::make_unique<Texture>();
	sinomiya->Load(L"Resources/triangle_mat/tex1.png");

	std::unique_ptr<Texture> white = std::make_unique<Texture>();
	white->CreateWhiteTexture();

	PipelineSet object3dPipelineSet = CreateObject3DPipeline();

	SpriteCommon spriteCommon;
	//spriteCommon = SpriteCommon::GetInstance();
	spriteCommon = SpriteCommonCreate();

	//スプライト
	Sprite pizzaSprite;
	pizzaSprite = SpriteCreate(pizza.get(), {0.5f,0.5f});
	SpriteInit(pizzaSprite, spriteCommon,{ 100,100}, 0);
	SpriteSetSize(pizzaSprite, { 100,100 });

	Sprite slimeSprite;
	slimeSprite = SpriteCreate(slime.get(), { 0.5f,0.5f });
	SpriteInit(slimeSprite, spriteCommon,{ 200,200 }, 45,{1,0,1,1});

	//デバッグテキスト生成

	std::unique_ptr<Texture> debugFont = std::make_unique<Texture>();
	debugFont->Load(L"Resources/debugfont.png");

	DebugText debugText;
	debugText.Initialize(debugFont.get());

	//ジオメトリオブジェクト用パイプライン生成
	PipelineSet geometryObjectPipelineSet = CreateGeometryPipeline();

	//ジオメトリオブジェクト生成
	GeometryObject GObject;
	GObject.CreateModel();

	const int kObjectCount = 3;
	std::unique_ptr<Obj3d[]> object3ds;
	object3ds = std::make_unique<Obj3d[]>(kObjectCount);

	for (int i = 0; i < kObjectCount; i++)
	{
		object3ds[i].Initialize();
	}

	//どのモデルの形を使うかを設定
	object3ds[0].SetModel(&cube);
	object3ds[1].SetModel(&plane);
	object3ds[2].SetModel(&skydome);

	object3ds[0].scale.x = 10.0f;
	object3ds[0].scale.y = 10.0f;

	object3ds[1].position.x = 0;
	object3ds[1].position.y = -10;
	object3ds[1].scale.x = 10.0f;
	object3ds[1].scale.y = 10.0f;
	object3ds[1].rotation.x += XMConvertToRadians(90.0f);
	
	////線のモデルを使う設定
	//const int kLineCountX = 15;
	//const int kLineCountY = 15;
	//std::unique_ptr<Obj3d[]> LineX;
	//LineX = std::make_unique<Obj3d[]>(kLineCountX);

	//for (int i = 0; i < kLineCountX; i++)
	//{
	//	LineX[i].Initialize();
	//	LineX[i].SetModel(&line);
	//	LineX[i].position.x += 10 * i - (10 * kLineCountX / 2);
	//}

	//std::unique_ptr<Obj3d[]> LineY;
	//LineY = std::make_unique<Obj3d[]>(kLineCountY);

	//for (int i = 0; i < kLineCountY; i++)
	//{
	//	LineY[i].Initialize();
	//	LineY[i].SetModel(&line);
	//	//LineY[i].position.x += 10 * i;
	//	LineY[i].position.z += 10 * i - (10 * kLineCountY / 2);

	//	LineY[i].rotation.y += XMConvertToRadians(90.0f);
	//}

	//射影変換行列(投資投影)
	XMMATRIX matProjection =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),
			(float)window_width / window_height,
			0.1f, 1000.0f
		);

	//ビュー変換行列(透視投影)を計算
	View view_;
	view_.Initialize();

	view_.eye.y = 50.0f;
	view_.UpdatematView();

	Billboard billboard = Billboard(&view_,false);
	billboard.Initialize();
	billboard.SetModel(&triangleM);

	billboard.position = { 10,10,0 };
	billboard.scale = { 100,100,100 };

#pragma endregion 描画初期化処理

	SoundManager soundManager;
	soundManager.Initialize();

	SoundData curser = soundManager.SoundLoadWave("Resources\\sound\\curser.wav");

	gameScene_.Initialize();

	//ゲームループ内で使う変数の宣言

	float angle = 0.0f;	//カメラの回転角
	float angleY = 0.0f;

	float cameraY = 100;

	//float count = 0;

	int count = 0;

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
				view_.eye.x += 2;
			}
			if (input_->PushKey(DIK_LEFT)) { 
				view_.eye.x -= 2; 
			}
			if (input_->PushKey(DIK_UP)) {
				view_.eye.y += 2;
			}
			if (input_->PushKey(DIK_DOWN)) {
				view_.eye.y -= 2;
			}

			if (input_->PushKey(DIK_SPACE))
			{
				if (input_->PushKey(DIK_RIGHT)) {
				
					view_.target.x += 2;
				}
				if (input_->PushKey(DIK_LEFT)) {
					
					view_.target.x -= 2;
				}
				if (input_->PushKey(DIK_UP)) {
					
					view_.target.y += 2;
				}
				if (input_->PushKey(DIK_DOWN)) {
					
					view_.target.y -= 2;
				}
			}
			view_.UpdatematView();
		}

		if (input_->TriggerKey(DIK_R))
		{
			for (int i = 0; i < kObjectCount; i++)
			{
				object3ds[i].Initialize();
			}
			view_.Initialize();
			view_.eye.y = 50.0f;
			view_.UpdatematView();
		}

		/*if (input_->PushKey(DIK_D))
		{
			billboard.position.x += 10.0f * TimeManager::deltaTime;
		}

		if (input_->PushKey(DIK_A))
		{
			billboard.position.x -= 10.0f * TimeManager::deltaTime;
		}*/

		//相対距離を求める
		XMFLOAT3 offset = { 0,0,0 };
		offset.x = view_.eye.x - object3ds[0].position.x;
		offset.y = view_.eye.y - object3ds[0].position.y;
		offset.z = view_.eye.z - object3ds[0].position.z;

		view_.eye.x = object3ds[0].position.x + offset.x;
		view_.eye.y = object3ds[0].position.y + offset.y;
		view_.eye.z = object3ds[0].position.z + offset.z;

		view_.UpdatematView();
		//オブジェクトの更新
		if (input_->PushKey(DIK_D))
		{
			billboard.position.x += 60.0f * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_A))
		{
			billboard.position.x -= 60.0f * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_W))
		{
			billboard.position.y += 60.0f * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_S))
		{
			billboard.position.y -= 60.0f * TimeManager::deltaTime;
		}
		for (size_t i = 0; i < kObjectCount; i++)
		{
			object3ds[i].Update(view_.matView, matProjection);
		}

		if (input_->PushKey(DIK_Q))
		{
			billboard.yBillboardMode = true;
		}
		else
		{
			billboard.yBillboardMode = false;
		}

		if (input_->PushKey(DIK_1))
		{
			TimeManager::fixFPS = 30;
		}
		if (input_->PushKey(DIK_2))
		{
			TimeManager::fixFPS = 60;
		}
		if (input_->PushKey(DIK_3))
		{
			TimeManager::fixFPS = 120;
		}

		GObject.Update(view_.matView,matProjection);

		billboard.Update(matProjection);

		SpriteUpdate(pizzaSprite, spriteCommon);
		SpriteUpdate(slimeSprite, spriteCommon);

	
		count++;
		
		debugText.Print(spriteCommon,
			"deltaTime " + std::to_string(TimeManager::deltaTime), 50, 100, 1.0f);

		debugText.Print(spriteCommon,
			"fps " + std::to_string(TimeManager::fps), 50, 200, 1.0f);

		///---DirectX毎フレーム処理 ここまで---///
#pragma endregion DirectX毎フレーム処理

#pragma region グラフィックスコマンド
		//--4.描画コマンドここから--//
		BasicObjectPreDraw( object3dPipelineSet);

		//描画処理
		gameScene_.Draw();

		//object3ds[0].MaterialDraw(DX12->commandList.Get());
		//object3ds[0].Draw(slime.get());
		object3ds[2].MaterialDraw();

		//billboard.Draw(slime.get());

		GeometryObjectPreDraw(geometryObjectPipelineSet);
		GObject.Draw(slime.get());


		//スプライトの前描画(共通コマンド)
		SpriteCommonBeginDraw(spriteCommon);

		//スプライト単体描画
		SpriteDraw(pizzaSprite);
		SpriteDraw(slimeSprite);

		debugText.DrawAll();

		//--4.描画コマンドここまで--//

#pragma endregion グラフィックスコマンド

#pragma region 画面入れ替え

		PostDraw();

		debugText.PostDraw();

#pragma endregion 画面入れ替え

		TimeManager::Update();

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