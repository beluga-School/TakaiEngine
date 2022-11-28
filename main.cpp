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

#include "Particle.h"
#include "MathF.h"

#include "SceneManager.h"

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

	//WICテクスチャのロード
	TextureManager::GetInstance()->Initialize();
	TextureManager::GetInstance()->PreLoad();

	SpriteCommon::spriteCommon.Initialize();

	ModelManager::GetInstance()->PreLoad();

	PipelineSet object3dPipelineSet = CreateObject3DPipeline();

	//ジオメトリオブジェクト用パイプライン生成
	PipelineSet geometryObjectPipelineSet = CreateGeometryPipeline();

	//スプライト
	Sprite pizzaSprite;
	SpriteCreate(&pizzaSprite,&TextureManager::GetInstance()->pizza, {0.5f,0.5f});
	SpriteInit(pizzaSprite, SpriteCommon::spriteCommon,{ 100,100}, 0);
	SpriteSetSize(pizzaSprite, { 100,100 });

	Sprite slimeSprite;
	SpriteCreate(&slimeSprite, &TextureManager::GetInstance()->pizza, { 0.5f,0.5f });
	SpriteInit(slimeSprite, SpriteCommon::spriteCommon,{ 200,200 }, 45,{1,0,1,1});

	//デバッグテキスト生成

	std::unique_ptr<Texture> debugFont = std::make_unique<Texture>();
	debugFont->Load(L"Resources/debugfont.png");

	DebugText debugText;
	debugText.Initialize(debugFont.get());

	//ジオメトリオブジェクト生成
	ParticleManager *particleManager = ParticleManager::Getinstance();
	particleManager->Initialize();

	Obj3d skydome;
	skydome.Initialize();

	//どのモデルの形を使うかを設定
	skydome.SetModel(&ModelManager::GetInstance()->skyDomeM);

	skydome.SetTexture(&TextureManager::GetInstance()->slime);

	//ビュー変換行列(透視投影)を計算
	Camera *camera = Camera::camera;
	camera->Initialize();

	Obj3d cubeObj;
	cubeObj.Initialize();
	cubeObj.SetModel(&ModelManager::GetInstance()->cubeM);
	cubeObj.SetTexture(&TextureManager::GetInstance()->slime);

	cubeObj.position = { 0,0,10 };

	Billboard billboard = Billboard(camera,false);
	billboard.Initialize();
	billboard.SetModel(&ModelManager::GetInstance()->boardM);
	billboard.SetTexture(&TextureManager::GetInstance()->pizza);

	billboard.position = { 10,10,10 };
	billboard.scale = { 3,3,3 };

	camera->target = {
		0,0,0
	};

	camera->SetEye({ 0,20,-10 });

	//camera->SetEye({ 10,10,10 });

#pragma endregion 描画初期化処理

	SoundManager soundManager;
	soundManager.Initialize();

	SoundData curser = soundManager.SoundLoadWave("Resources\\sound\\curser.wav");

	gameScene_.Initialize();

	//ゲームループ内で使う変数の宣言

	float angle = 0.0f;	//カメラの回転角
	float angleY = 0.0f;

	ParticleEmitter pEmitter;
	pEmitter.Initialize();
	pEmitter.SetInfo({ -10,-10,20 }, 10, 5,{0,0.5f,1,1}, 1, true);

	ParticleEmitter pEmitter2;
	pEmitter2.Initialize();
	pEmitter2.SetInfo({ 10,10,10 }, 3, 1, { 1,1,1,1 }, 1);	
	
	ParticleEmitter pEmitter3;
	pEmitter3.Initialize();
	pEmitter3.SetInfo({ 20,-20,10 }, 10, 1, { 0,0.1f,0.1f,1 }, 10);

	SceneManager sceneManager;

	Scene Tscene("Title");
	Scene Nscene("newGame");
	Scene Cscene("gameClear");
	Scene Oscene("gameOver");

	sceneManager.LoadScene(Tscene);
	sceneManager.LoadScene(Nscene);
	sceneManager.LoadScene(Cscene);
	sceneManager.LoadScene(Oscene);

	sceneManager.ChangeScene("Title");

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

		pEmitter.Update();
		pEmitter2.Update();
		pEmitter3.Update();

		//カメラ座標を動かす
		if (input_->PushKey(DIK_RIGHT)) {
			camera->eye.x += 20 * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_LEFT)) {
			camera->eye.x -= 20 * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_UP)) {
			camera->eye.y += 20 * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_DOWN)) {
			camera->eye.y -= 20 * TimeManager::deltaTime;
		}

		if (input_->PushKey(DIK_D)) {

			camera->eye.x += 20 * TimeManager::deltaTime;
			camera->target.x += 20 * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_A)) {

			camera->eye.x -= 20 * TimeManager::deltaTime;
			camera->target.x -= 20 * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_W)) {

			camera->eye.z += 20 * TimeManager::deltaTime;
			camera->target.z += 20 * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_S)) {

			camera->eye.z -= 20 * TimeManager::deltaTime;
			camera->target.z -= 20 * TimeManager::deltaTime;
		}

		camera->UpdatematView();

		if (input_->TriggerKey(DIK_R))
		{
			camera->Initialize();
			camera->eye.y = 20.0f;
			camera->UpdatematView();
		}

		camera->UpdatematView();
	
		if (input_->TriggerKey(DIK_1))
		{
			sceneManager.ChangeScene("Title");
			//TimeManager::fixFPS = 30;
			//TimeManager::fps = 0;
		}
		if (input_->TriggerKey(DIK_2))
		{
			sceneManager.ChangeScene("newGame");
			//TimeManager::fixFPS = 60;
			//TimeManager::fps = 0;
		}
		if (input_->TriggerKey(DIK_3))
		{
			sceneManager.ChangeScene("gameClear");
			//TimeManager::fixFPS = 120;
			//TimeManager::fps = 0;
		}
		if (input_->TriggerKey(DIK_4))
		{
			sceneManager.ChangeScene("gameOver");
			//TimeManager::fixFPS = 120;
			//TimeManager::fps = 0;
		}

		//オブジェクトの更新
		particleManager->Update(camera->matView, camera->matProjection);

		skydome.Update(camera->matView, camera->matProjection);

		cubeObj.Update(camera->matView, camera->matProjection);
		billboard.Update(camera->matProjection);

		SpriteUpdate(pizzaSprite, SpriteCommon::spriteCommon);
		SpriteUpdate(slimeSprite, SpriteCommon::spriteCommon);

		debugText.Print(SpriteCommon::spriteCommon,
			"deltaTime " + std::to_string(TimeManager::deltaTime), 50, 500);

		debugText.Print(SpriteCommon::spriteCommon,
			"fps " + std::to_string(TimeManager::fps), 50, 550);

		debugText.Print(SpriteCommon::spriteCommon,
			"FixFPS " + std::to_string(TimeManager::fixFPS), 50, 600);
		
		debugText.Print(SpriteCommon::spriteCommon,
			"particles.size " + std::to_string(particleManager->particles.size()), 50, 350);

		debugText.Print(SpriteCommon::spriteCommon,
			sceneManager.nowScene->str, 50, 400);

		///---DirectX毎フレーム処理 ここまで---///
#pragma endregion DirectX毎フレーム処理

#pragma region グラフィックスコマンド
		//--4.描画コマンドここから--//
		BasicObjectPreDraw( object3dPipelineSet);

		//描画処理
		gameScene_.Draw();

		billboard.Draw();
		cubeObj.Draw();
		
		skydome.DrawMaterial();


		GeometryObjectPreDraw(geometryObjectPipelineSet);
		particleManager->Draw(&TextureManager::GetInstance()->particle);


		//スプライトの前描画(共通コマンド)
		SpriteCommonBeginDraw(SpriteCommon::spriteCommon);

		//スプライト単体描画
		//SpriteDraw(pizzaSprite);
		//SpriteDraw(slimeSprite);

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