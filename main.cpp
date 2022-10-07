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
#include "Shader.h"
#include "Obj.h"
#include "ViewProjection.h"
#include "Vector3.h"
#include "Model.h"
#include "Texture.h"
#include "GameScene.h"
#include "Sprite.h"
#include "Pipeline.h"

#include "Sound.h"
#include "DebugText.h"

WinAPI winApi_;

//windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	GameScene gameScene_;

	Input* input_ = new Input;

	//WindowsAPI初期化処理
	winApi_.SetWindowClass();

	///---DirectX初期化処理　ここから---///
	
#ifdef  _DEBUG
	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}

#endif _DEBUG

	DX12.yobidasi(winApi_);

#ifdef  _DEBUG
	ID3D12InfoQueue* infoQueue;
	if (SUCCEEDED(DX12.device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->GetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION);//ヤバイエラー時にとまと
		infoQueue->GetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR);	 //エラー時に止まる
		infoQueue->Release();
	}
#endif  _DEBUG

	//DirectInputの初期化
	input_->DirectInputInit(winApi_);

	//キーボードデバイスの生成
	input_->DirectInputCreate(winApi_);

	///---DirectX初期化処理　ここまで---///

#pragma region 描画初期化処理

	//--depthこっから

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = window_width;	//レンダーターゲットに合わせる
	depthResourceDesc.Height = window_height;	//レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	//深度値1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット

	//リソース生成
	ComPtr<ID3D12Resource> depthBuff;
	result = DX12.device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));

	//深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;	//深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//デプスステンシルビュー
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	result = DX12.device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DX12.device->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//--depthここまで

	Model cube = Cube();
	Model plane = Plate();
	Model triangle = Triangle();
	Model line = Line();

	Model triangleM = Model();

	//頂点データ作成
	cube.CreateDefaultModel(DX12);
	plane.CreateDefaultModel(DX12);
	triangle.CreateDefaultModel(DX12);
	line.CreateDefaultModel(DX12);

	triangleM.CreateModel(L"Resources\\triangle.obj", DX12);

	//WICテクスチャのロード
	const wchar_t* msg[4] = 
	{ L"Resources/zawa_sironuri.png", 
		L"Resources/slime.png",
		L"Resources/pizza_sironuri.png",
		L"Resources/tyusiten.png"
	};

	std::shared_ptr<Texture> zawa = std::make_shared<Texture>();
	zawa->Load(msg[0], DX12);

	std::shared_ptr<Texture> slime = std::make_shared<Texture>();
	slime->Load(msg[1], DX12);

	std::shared_ptr<Texture> pizza = std::make_shared<Texture>();
	pizza->Load(msg[2], DX12);

	std::shared_ptr<Texture> tyusiten = std::make_shared<Texture>();
	tyusiten->Load(msg[3], DX12);

	std::shared_ptr<Texture> white = std::make_shared<Texture>();
	white->CreateWhiteTexture(DX12);

	PipelineSet object3dPipelineSet = CreateObject3DPipeline(DX12);

	SpriteCommon spriteCommon;
	spriteCommon = SpriteCommonCreate(DX12);

	//スプライト
	Sprite pizzaSprite;
	pizzaSprite = SpriteCreate(DX12, pizza.get(), {0.5f,0.5f});
	SpriteInit(pizzaSprite, spriteCommon,{ 100,100}, 0);
	SpriteSetSize(pizzaSprite, { 100,100 });

	Sprite slimeSprite;
	slimeSprite = SpriteCreate(DX12,slime.get(), { 0.5f,0.5f });
	SpriteInit(slimeSprite, spriteCommon,{ 200,200 }, 45,{1,0,1,1});

	//デバッグテキスト生成

	std::shared_ptr<Texture> debugFont = std::make_shared<Texture>();
	debugFont->Load(L"Resources/debugfont.png", DX12);

	DebugText debugText;
	debugText.Initialize(DX12, debugFont.get());

	//3Dオブジェクト用の定数バッファ生成
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	//ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
	//リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	ConstBuffer<ConstBufferDataMaterial> constBufferM;
	ConstBuffer<ConstBufferDataMaterial> constBufferM2;

	const int kObjectCount = 2;
	std::unique_ptr<Obj3d[]> object3ds;
	object3ds = std::make_unique<Obj3d[]>(kObjectCount);

	for (int i = 0; i < kObjectCount; i++)
	{
		object3ds[i].Initialize();
	}

	//どのモデルの形を使うかを設定
	object3ds[0].SetModel(&triangleM);
	object3ds[1].SetModel(&plane);

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

	//値を書き込むと自動的に転送されるらしい
	constBufferM.constBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);

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
	FLOAT clearColor[] = { 0,0,0,0 };

	FLOAT redColor[] = { 0.5f,0.1f,0.25f,0.0f };	//赤っぽい色
	FLOAT blueColor[] = { 0.1f,0.25f,0.5f,0.0f };	//青っぽい色

	for (int i = 0; i < _countof(clearColor); i++)
	{
		clearColor[i] = blueColor[i];
	}

	//XMFLOAT3 materialColor = { -0.005f,0.005f,0 };

	float angle = 0.0f;	//カメラの回転角
	float angleY = 0.0f;

	float cameraY = 100;

	float count = 0;

	XMFLOAT4 color = { 0,1,0.5f,1.0f };
	XMFLOAT4 colorSpd = { 0.01f,0.01f,0.01f,0.01f };

	//ゲームループ
	while (true){

#pragma region ウィンドウメッセージ
		if (PeekMessage(&winApi_.msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&winApi_.msg);		//キー入力メッセージの処理
			DispatchMessage(&winApi_.msg);		//プロシージャにメッセージを送る
		}

		if (winApi_.msg.message == WM_QUIT) {
			break;
		}
#pragma endregion ウィンドウメッセージ

#pragma region DirectX毎フレーム処理
		///---DirectX毎フレーム処理 ここから---///
		
		UINT bbIndex = DX12.swapChain->GetCurrentBackBufferIndex();

		//1.リソースバリアで書き込み化に変更
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = DX12.backBuffers[bbIndex].Get();	//バックバッファを指定
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	//表示状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;		//描画状態へ
		DX12.commandList->ResourceBarrier(1, &barrierDesc);

		//2.描画先の変更
		//レンダーターゲットビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = DX12.rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * DX12.device->GetDescriptorHandleIncrementSize(DX12.rtvHeapDesc.Type);
		//DX12.commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		DX12.commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

		//3.画面クリア
		DX12.commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		DX12.commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


		//更新処理

		gameScene_.Update();

		input_->Update();

		if (input_->TriggerKey(DIK_SPACE))
		{

			soundManager.SoundPlayWave(curser);
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

		//viewProjection_.target.x = object3ds[0].position.x + offset.x;
		//viewProjection_.target.y = object3ds[0].position.y + offset.y;
		//viewProjection_.target.z = object3ds[0].position.z + offset.z;

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
				object3ds[i].rotation.z += 0.1f;
			}
			if (input_->PushKey(DIK_E))
			{
				object3ds[i].rotation.z -= 0.1f;
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

		//色を変える処理
		if (false)
		{
			if (color.x < 0 || color.x > 1)
			{
				colorSpd.x *= -1;
			}
			if (color.y < 0 || color.y > 1)
			{
				colorSpd.y *= -1;
			}
			if (color.z < 0 || color.z > 1)
			{
				colorSpd.z *= -1;
			}
			if (color.z < 0 || color.z > 1)
			{
				colorSpd.w *= -1;
			}
			color.x += colorSpd.x;
			color.y += colorSpd.y;
			color.z += colorSpd.z;
			color.w += colorSpd.w;

			constBufferM.constBufferData->color = XMFLOAT4(color.x, color.y, color.z, 1.0f);
			constBufferM2.constBufferData->color = XMFLOAT4(1.0f, 1.0f, 1.0f, color.w);
		}

		SpriteUpdate(pizzaSprite, spriteCommon);
		SpriteUpdate(slimeSprite, spriteCommon);

		debugText.Print(spriteCommon, "Hello,DirectX", 200,100,1.0f);
		debugText.Print(spriteCommon, "Nihon Kogakuin", 200,200,2.0f);

		///---DirectX毎フレーム処理 ここまで---///
#pragma endregion DirectX毎フレーム処理

#pragma region グラフィックスコマンド
		//--4.描画コマンドここから--//
		
		D3D12_VIEWPORT viewport{};
		viewport.Width = window_width;
		viewport.Height = window_height;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		//ビューポート設定コマンドを、コマンドリストに積む
		DX12.commandList->RSSetViewports(1, &viewport);

		D3D12_RECT scissorRect{};
		scissorRect.left = 0;									//切り抜き座標左
		scissorRect.right = scissorRect.left + window_width;	//切り抜き座標右
		scissorRect.top = 0;									//切り抜き座標上
		scissorRect.bottom = scissorRect.top + window_height;	//切り抜き座標下
		//シザー矩形設定コマンドを、コマンドリストに積む
		DX12.commandList->RSSetScissorRects(1, &scissorRect);

		//パイプラインステートとルートシグネチャの設定コマンド
		//スプライトじゃない方
		DX12.commandList->SetPipelineState(object3dPipelineSet.pipelinestate.Get());
		DX12.commandList->SetGraphicsRootSignature(object3dPipelineSet.rootsignature.Get());

		//プリミティブ形状の設定コマンド
		DX12.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		object3ds[0].constBufferM = constBufferM;
		object3ds[1].constBufferM = constBufferM2;
		//object3ds[2].constBufferM = constBufferM2;

		//描画処理
		gameScene_.Draw();

		object3ds[0].Draw(DX12.commandList.Get(), pizza.get());
		
		if (input_->PushKey(DIK_SPACE))
		{
			object3ds[1].Draw(DX12.commandList.Get(), tyusiten.get());
		}

		for (int i = 0; i < kLineCountX; i++)
		{
			LineX[i].Draw(DX12.commandList.Get(), white.get());
		}
		for (int i = 0; i < kLineCountY; i++)
		{
			LineY[i].Draw(DX12.commandList.Get(), white.get());
		}

		//スプライトの前描画(共通コマンド)
		SpriteCommonBeginDraw(DX12, spriteCommon);

		//スプライト単体描画
		SpriteDraw(pizzaSprite, DX12);
		SpriteDraw(slimeSprite, DX12);

		debugText.DrawAll(DX12, spriteCommon);

		//--4.描画コマンドここまで--//

#pragma endregion グラフィックスコマンド

#pragma region 画面入れ替え

		//5.リソースバリアを戻す
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//描画状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;	//表示状態へ
		DX12.commandList->ResourceBarrier(1, &barrierDesc);

		//命令のクローズ
		result = DX12.commandList->Close();
		assert(SUCCEEDED(result));

		//コマンドリストの実行
		ID3D12CommandList* commandLists[] = { DX12.commandList.Get() };
		DX12.commandQueue->ExecuteCommandLists(1, commandLists);

		//画面に表示するバッファをフリップ(裏表の入れ替え)
		result = DX12.swapChain->Present(1, 0);
		assert(SUCCEEDED(result));

		//コマンドの実行完了を待つ
		DX12.commandQueue->Signal(DX12.fence.Get(), ++DX12.fenceVal);
		if (DX12.fence->GetCompletedValue() != DX12.fenceVal) {
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			DX12.fence->SetEventOnCompletion(DX12.fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		//キューをクリア
		result = DX12.commandAllocator->Reset();
		assert(SUCCEEDED(result));

		//再びコマンドリストを貯める準備
		result = DX12.commandList->Reset(DX12.commandAllocator.Get(), nullptr);
		assert(SUCCEEDED(result));

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
	UnregisterClass(winApi_.w.lpszClassName, winApi_.w.hInstance);

	return 0;
}