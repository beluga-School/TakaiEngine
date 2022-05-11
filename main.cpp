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

//自分でクラス化したやつ
#include "WinAPI.h"
#include "Result.h"
#include "Input.h"
#include "DirectXInit.h"
#include "Vertex.h"
#include "Shader.h"
#include "ConstBuffer.h"

WinAPI winApi_;
VertexData vertexdate_;

//struct ConstBufferDataMaterial {
//	XMFLOAT4 color;	//色(RGBA)
//};
//
//struct ConstBufferDataTransform {
//	XMMATRIX mat;	//3D変換行列
//};

//ConstBufferDataMaterial* constMapMaterial = nullptr;
//
//ConstBufferDataTransform* constMapTransform = nullptr;


//windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	Input* input_ = new Input();

	//WindowsAPI初期化処理
	winApi_.SetWindowClass();

	///---DirectX初期化処理　ここから---///
	
#ifdef  _DEBUG
	//デバッグレイヤーをオンに
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
#endif _DEBUG

	DirectXInit DX12;
	DX12.yobidasi(winApi_);

	//DirectInputの初期化
	input_->DirectInputInit(winApi_);

	//キーボードデバイスの生成
	input_->DirectInputCreate(winApi_);

	///---DirectX初期化処理　ここまで---///

#pragma region 描画初期化処理

	vertexdate_.CreateVertex(DX12);

	Shader shader_;

	shader_.vsBlob = shader_.Compile(L"BasicVS.hlsl", "vs_5_0", shader_.vsBlob,"main");
	shader_.psBlob = shader_.Compile(L"BasicPS.hlsl", "ps_5_0", shader_.psBlob,"main");

	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = shader_.vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = shader_.vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = shader_.psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = shader_.psBlob->GetBufferSize();

	//サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//標準設定

	//ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	//カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true;			//深度クリッピングを有効に

	//ブレンドステート
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA全てのチャンネルを描画

	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //RGBAすべてのチャンネルを描画

	blenddesc.BlendEnable = true;					//ブレンドを有効にする

	//あるふぁ〜〜〜〜
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ZERO;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う

	enum BLEND_MODE
	{
		ADD,
		SUB,
		INV,
		ALPHA,
	};
	int blendMode = ALPHA;

	if (blendMode == ADD)
	{
		//加算合成
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//加算
		blenddesc.SrcBlend = D3D12_BLEND_ONE;			//ソースの値を100%使う
		blenddesc.DestBlend = D3D12_BLEND_ONE;			//デストの値を100%使う
	}
	if (blendMode == SUB)
	{
		//減算合成
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//デストからソースを減算
		blenddesc.SrcBlend = D3D12_BLEND_ONE;			//ソースの値を100%使う
		blenddesc.DestBlend = D3D12_BLEND_ONE;			//デストの値を100%使う
	}
	if (blendMode == INV)
	{
		//色反転
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//加算
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-デストカラーの値
		blenddesc.DestBlend = D3D12_BLEND_ZERO;			//使わない
	}
	if (blendMode == ALPHA)
	{
		//半透明合成
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//加算
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;		//ソースのアルファ値
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値
	}

	//頂点レイアウト
	static D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ //xyz座標
				"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
			{ //uv座標
				"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
	};

	//頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	pipelineDesc.NumRenderTargets = 1;	//描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0〜255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1;	//1ピクセルにつき1回サンプリング

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/pizza.png",	//ここに読み込みたいファイルのパスを入れる
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	ScratchImage mipChain{};
	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	if (false)
	{
		//横方向ピクセル数
		const size_t textureWidth = 256;
		//縦方向ピクセル
		const size_t textureHeight = 256;
		//配列の要素数
		const size_t imageDataCount = textureWidth * textureHeight;
		//画像イメージデータ配列
		XMFLOAT4* imageData = new XMFLOAT4[imageDataCount]; //必ず後で解放する

		//全ピクセルの色を初期化
		for (size_t i = 0; i < imageDataCount; i++)
		{
			imageData[i].x = 1.0f;	//R
			imageData[i].y = 0.0f;	//G
			imageData[i].z = 0.0f;	//B
			imageData[i].w = 1.0f;	//A
		}
	}

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	ID3D12Resource* texBuff = nullptr;
	result = DX12.device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	for (SIZE_T i = 0; i < metadata.mipLevels; i++)
	{
		//ミップマップレベルを指定してくださいイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(	//ここで止まる
			(UINT)i,
			nullptr,	//全領域へコピー
			img->pixels,	//元データアドレス
			(UINT)img->rowPitch,	//1ラインサイズ
			(UINT)img->slicePitch	//全サイズ
		);
		assert(SUCCEEDED(result));
	}

	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定を元にSRV用デスクリプタヒープを生成
	ID3D12DescriptorHeap* srvHeap = nullptr;
	result = DX12.device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = vertexdate_.resDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = vertexdate_.resDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	DX12.device->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;	//一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;					//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	//テクスチャサンプラーの生成
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//ルートシグネチャ
	ID3D12RootSignature* rootSignature;

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;	//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);		//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	//ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader_.errorBlob);
	assert(SUCCEEDED(result));
	result = DX12.device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;

	//パイプラインステートの生成
	ID3D12PipelineState* pipelineState = nullptr;
	result = DX12.device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	ID3D12Resource* countBuffMaterial = nullptr;
	countBuffMaterial = CreateConstantBuff(cbHeapProp, cbResourceDesc, DX12, countBuffMaterial,constMapMaterial);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp2{};
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc2{};
	ID3D12Resource* constBuffTransform = nullptr;
	constBuffTransform = CreateConstantBuff(cbHeapProp2, cbResourceDesc2, DX12, constBuffTransform,constMapTransform);

	//値を書き込むと自動的に転送されるらしい
	constMapMaterial->color = XMFLOAT4(1, 0, 0, 0.5f);	//RGBAで半透明の赤の値

	constMapTransform->mat = XMMatrixIdentity();

#pragma endregion 描画初期化処理

	//ゲームループ内で使う変数の宣言
	FLOAT clearColor[] = { 0,0,0,0 };

	FLOAT redColor[] = { 0.5f,0.1f,0.25f,0.0f };	//赤っぽい色
	FLOAT blueColor[] = { 0.1f,0.25f,0.5f,0.0f };	//青っぽい色

	for (int i = 0; i < _countof(clearColor); i++)
	{
		clearColor[i] = blueColor[i];
	}

	XMFLOAT3 materialColor = { -0.005f,0.005f,0 };

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
		barrierDesc.Transition.pResource = DX12.backBuffers[bbIndex];	//バックバッファを指定
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	//表示状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;		//描画状態へ
		DX12.commandList->ResourceBarrier(1, &barrierDesc);

		//2.描画先の変更
		//レンダーターゲットビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = DX12.rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * DX12.device->GetDescriptorHandleIncrementSize(DX12.rtvHeapDesc.Type);
		DX12.commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

		//3.画面クリア
		
		DX12.commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

		input_->Update();

		if (input_->TriggerKey(DIK_1))
		{
			for (int i = 0; i < _countof(clearColor); i++)
			{
				if (clearColor[i] != redColor[i])
				{
					clearColor[i] = redColor[i];
				}
				else if (clearColor[i] == redColor[i])
				{
					clearColor[i] = blueColor[i];
				}
			}
		}

		constMapMaterial->color.x += materialColor.x;
		constMapMaterial->color.y += materialColor.y;

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
		DX12.commandList->SetPipelineState(pipelineState);
		DX12.commandList->SetGraphicsRootSignature(rootSignature);

		//プリミティブ形状の設定コマンド
		DX12.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//頂点バッファビューの設定コマンド
		DX12.commandList->IASetVertexBuffers(0, 1, &vertexdate_.vbView);

		//定数バッファビュー(CBV)の設定コマンド
		DX12.commandList->SetGraphicsRootConstantBufferView(0, countBuffMaterial->GetGPUVirtualAddress());

		//SRVヒープの設定コマンド
		DX12.commandList->SetDescriptorHeaps(1, &srvHeap);

		//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

		//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
		DX12.commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		//定数バッファビュー(CBV)の設定コマンド

		//インデックスバッファビューの設定コマンド
		DX12.commandList->IASetIndexBuffer(&vertexdate_.ibView);

		//描画コマンド
		DX12.commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);	//全ての頂点を使って描画

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
		ID3D12CommandList* commandLists[] = { DX12.commandList };
		DX12.commandQueue->ExecuteCommandLists(1, commandLists);

		//画面に表示するバッファをフリップ(裏表の入れ替え)
		result = DX12.swapChain->Present(1, 0);
		assert(SUCCEEDED(result));

		//コマンドの実行完了を待つ
		DX12.commandQueue->Signal(DX12.fence, ++DX12.fenceVal);
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
		result = DX12.commandList->Reset(DX12.commandAllocator, nullptr);
		assert(SUCCEEDED(result));

#pragma endregion 画面入れ替え

	}

	delete input_;

	//ウィンドウクラスを登録解除
	UnregisterClass(winApi_.w.lpszClassName, winApi_.w.hInstance);

	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

	return 0;
}