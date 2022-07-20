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
#include "Pipeline.h"

//やりたいことメモ
//・objの読み込みは実装したい

WinAPI winApi_;

struct SpriteS
{
	ID3D12Resource* vertBuff;
	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ConstBuffer<ConstBufferDataTransform> constBufferT;

	void Update();
	void Draw(DirectX12 DX12);
};

void SpriteS::Update()
{
}

void SpriteS::Draw(DirectX12 DX12)
{
	DX12.commandList->IASetVertexBuffers(0, 1, &vbView);
	DX12.commandList->SetGraphicsRootConstantBufferView(0, constBufferT.buffer->GetGPUVirtualAddress());
	DX12.commandList->DrawInstanced(4, 1, 0, 0);
}

void SpritePreDraw(DirectX12 DX12,const PipelineSet p, Texture& texture)
{
	DX12.commandList->SetPipelineState(p.pipelineState.Get());
	DX12.commandList->SetGraphicsRootSignature(p.rootSignature.Get());
	DX12.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//SRVヒープの設定コマンド
	DX12.commandList->SetDescriptorHeaps(1, &texture.srvHeap);
	//SRVヒープの先頭から順番にSRVをルートパラメータ1番に設定
	//ルートパラメータ1番はテクスチャバッファ
	DX12.commandList->SetGraphicsRootDescriptorTable(1, texture.GetHandle());

}

//SpriteS SpriteCreate(DirectX12 DX12)
//{
//	SpriteS sprite{};
//	std::vector<VertexSprite> vertices = {
//		{{   0.0f,100.0f,0.0f} ,{0.0f,1.0f}},	//左下
//		{{   0.0f, 0.0f,0.0f} ,{0.0f,0.0f}},	//左上
//		{{ 100.0f,100.0f,0.0f} ,{1.0f,1.0f}},	//右下
//		{{ 100.0f, 0.0f,0.0f} ,{1.0f,0.0f}},	//右上
//	};
//
//	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
//
//	//頂点バッファの設定
//	D3D12_HEAP_PROPERTIES heapProp{};		//ヒープ設定
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
//
//	//リソース設定
//	D3D12_RESOURCE_DESC resDesc{};
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB;	//頂点データ全体のサイズ
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	result = DX12.device->CreateCommittedResource(
//		&heapProp,	//ヒープ設定
//		D3D12_HEAP_FLAG_NONE,
//		&resDesc,	//リソース設定
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&sprite.vertBuff));
//	assert(SUCCEEDED(result));
//
//	//GPU状のバッファに対応した仮想メモリ(メインメモリ上)を取得
//	VertexSprite* vertMap = nullptr;
//	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	//全頂点に対して
//	for (int i = 0; i < vertices.size(); i++) {
//		vertMap[i] = vertices[i];	//	座標をコピー
//	}
//	//繋がりを解除
//	sprite.vertBuff->Unmap(0, nullptr);
//
//	//GPU仮想アドレス
//	sprite.vbView.BufferLocation = sprite.vertBuff->GetGPUVirtualAddress();
//	//頂点バッファのサイズ
//	sprite.vbView.SizeInBytes = sizeVB;
//	//頂点1つ分のデータサイズ
//	sprite.vbView.StrideInBytes = sizeof(vertices[0]);
//
//	result = DX12.device->CreateCommittedResource(
//		&heapProp,	//ヒープ設定
//		D3D12_HEAP_FLAG_NONE,
//		&resDesc,	//リソース設定
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&sprite.constBufferT.buffer));
//
//	//ここってデータマテリアルなの？
//	ConstBuffer<ConstBufferDataMaterial> constBufferM;
//
//	//ここら辺怪しい
//	result = sprite.constBufferT.buffer->Map(0, nullptr, (void**)&constBufferM);
//	constBufferM.constBufferData->color = XMFLOAT4(1, 1, 1, 1); //色指定 RGBA
//	sprite.constBufferT.constBufferData->mat = XMMatrixOrthographicOffCenterLH(
//		0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);
//	sprite.constBufferT.buffer->Unmap(0, nullptr);
//
//	return sprite;
//
//}

//windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	GameScene gameScene_;

	Input* input_ = new Input;

	//WindowsAPI初期化処理
	winApi_.SetWindowClass();

	///---DirectX初期化処理　ここから---///
	
#ifdef  _DEBUG
	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
#endif _DEBUG

	DX12.yobidasi(winApi_);

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
	Model sprite = Sprite();
	Model triangle = Triangle();
	Model line = Line();

	//頂点データ作成
	cube.CreateModel(DX12);
	sprite.CreateModel(DX12);
	triangle.CreateModel(DX12);
	line.CreateModel(DX12);

	//SpriteS spriteS;
	//spriteS = SpriteCreate(DX12);

	//WICテクスチャのロード
	const wchar_t* msg[3] = { L"Resources/zawa_sironuri.png", L"Resources/slime.png",L"Resources/pizza_sironuri.png" };

	Texture zawa;
	zawa.Load(msg[0], DX12);

	Texture slime;
	slime.Load(msg[1], DX12);

	Texture pizza;
	pizza.Load(msg[2], DX12);

	Texture white;
	white.CreateWhiteTexture(DX12);

	PipelineSet p = ObjectPipelineCreate3D(DX12);
	PipelineSet spriteP = ObjectPipelineCreate2D(DX12);

	//定数バッファの生成
	ConstBuffer<ConstBufferDataMaterial> constBufferM;

	const int kObjectCount = 2;
	std::unique_ptr<Obj3d[]> object3ds;
	object3ds = std::make_unique<Obj3d[]>(kObjectCount);

	for (int i = 0; i < kObjectCount; i++)
	{
		object3ds[i].Initialize();
		//親子構造　サンプル
		//先頭以外なら
		//if (i > 0)
		//{
		//	//ひとつ前を親オブジェクトとする
		//	object3ds[i].parent = &object3ds[i - 1];
		//	//子は親オブジェの9割の大きさ
		//	object3ds[i].scale = { 0.9f,0.9f,0.9f };
		//	//親オブジェに対してZ軸まわりに30度回転
		//	object3ds[i].rotation = { 0.0f,0.0f,XMConvertToRadians(30.0f) };

		//	//親オブジェに対してZ方向-8.0ずらす
		//	object3ds[i].position = { 0.0f,0.0f,-8.0f };
		//}
	}

	//どのモデルの形を使うかを設定
	object3ds[0].SetModel(&triangle);
	object3ds[1].SetModel(&cube);

	object3ds[1].position.x = 20;
	object3ds[1].position.y = 10;
	
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

	////単位行列で埋める
	//constMapTransform[0]->mat = XMMatrixIdentity();
	////指定の部分を書き換える
	//constMapTransform[0]->mat.r[0].m128_f32[0] = 2.0f / window_width;
	//constMapTransform[0]->mat.r[1].m128_f32[1] = -2.0f / window_height;

	//constMapTransform[0]->mat.r[3].m128_f32[0] = -1.0f;
	//constMapTransform[0]->mat.r[3].m128_f32[1] = 1.0f;
	//
	//constMapTransform[0]->mat = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(45.0f),				//上下画角45度
	//	(float)window_width / window_height,	//アスペクト比(画面横幅/画面縦幅)
	//	0.1f, 1000.0f							//前端、奥端
	//);

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

		//カメラ座標を動かす
		if (input_->PushKey(DIK_RIGHT) || 
			input_->PushKey(DIK_LEFT) || 
			input_->PushKey(DIK_UP) || 
			input_->PushKey(DIK_DOWN))
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
			viewProjection_.UpdatematView();
		}

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

		constBufferM.constBufferData->color = XMFLOAT4(color.x, color.y, color.z, color.w);

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
		DX12.commandList->SetPipelineState(p.pipelineState.Get());
		DX12.commandList->SetGraphicsRootSignature(p.rootSignature.Get());

		//プリミティブ形状の設定コマンド
		DX12.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//頂点バッファビューの設定コマンド
		//モデルのどれかのvertexbufferが入っていればok
		//DX12.commandList->IASetVertexBuffers(0, 1, &cube.vbView);

		//定数バッファビュー(CBV)の設定コマンド
		//DX12.commandList->SetGraphicsRootConstantBufferView(0, constBufferM.buffer->GetGPUVirtualAddress());

		for (int i = 0; i < kObjectCount; i++)
		{
			object3ds[i].constBufferM = constBufferM;
		}

		//描画処理
		gameScene_.Draw();

		object3ds[0].Draw(DX12.commandList.Get(), white);
		object3ds[1].Draw(DX12.commandList.Get(), pizza);

		for (int i = 0; i < kLineCountX; i++)
		{
			//LineX[i].constBufferM = constBufferM;
			LineX[i].Draw(DX12.commandList.Get(), white);
		}
		for (int i = 0; i < kLineCountY; i++)
		{
			//LineY[i].constBufferM = constBufferM;
			LineY[i].Draw(DX12.commandList.Get(), white);
		}

		//SpritePreDraw(DX12, p,white);
		//spriteS.Draw(DX12);

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

	}

	gameScene_.End();

	//ウィンドウクラスを登録解除
	UnregisterClass(winApi_.w.lpszClassName, winApi_.w.hInstance);

	return 0;
}
