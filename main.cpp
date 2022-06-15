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
#include "TextureLoad.h"
#include "Obj.h"
#include "ViewProjection.h"
#include "Vector3.h"

//やりたいことメモ
//・objの読み込みは実装したい

WinAPI winApi_;
VertexData vertexdate_;
//Obj zawa_[2];

ConstBufferDataMaterial* constMapMaterial = nullptr;
ID3D12Resource* countBuffMaterial = nullptr;

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
	ID3D12Resource* depthBuff = nullptr;
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
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	result = DX12.device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DX12.device->CreateDepthStencilView(
		depthBuff,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//頂点データ作成
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
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	//カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true;			//深度クリッピングを有効に

	//ブレンドステート
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA全てのチャンネルを描画

	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //RGBAすべてのチャンネルを描画

	blenddesc.BlendEnable = true;					//ブレンドを有効にする

	//あるふぁ～～～～
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
			{//法線ベクトル
				"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
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
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1;	//1ピクセルにつき1回サンプリング

	pipelineDesc.DepthStencilState.DepthEnable = true;	//深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	//小さければ許可
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット

	//TextureLoad textureload;
	//textureload.WIC(L"Resources/pizza.png");
	
	//WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	
	result = LoadFromWICFile(
		L"Resources/zawa_sironuri.png",	//ここに読み込みたいファイルのパスを入れる
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
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
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
	result = DX12.device->CreateCommittedResource(
		&cbHeapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&countBuffMaterial));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = countBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	//マッピング
	assert(SUCCEEDED(result));

	const size_t kObjectCount = 50;
	Obj3d object3ds[kObjectCount];

	for (int i = 0; i < _countof(object3ds); i++)
	{
		object3ds[i].Initialize(DX12.device);

		//親子構造　サンプル
		//先頭以外なら
		if (i > 0)
		{
			//ひとつ前を親オブジェクトとする
			object3ds[i].parent = &object3ds[i - 1];
			//子は親オブジェの9割の大きさ
			object3ds[i].scale = { 0.9f,0.9f,0.9f };
			//親オブジェに対してZ軸まわりに30度回転
			object3ds[i].rotation = { 0.0f,0.0f,XMConvertToRadians(30.0f) };

			//親オブジェに対してZ方向-8.0ずらす
			object3ds[i].position = { 0.0f,0.0f,-8.0f };
		}
	}

	//Obj3d biocheckBox;
	//biocheckBox.Initialize(DX12.device);
	//biocheckBox.position = { object3ds[0].position.x - 10,
	//	object3ds[0].position.y - 10,object3ds[0].position.z};
	//biocheckBox.scale = { 0.9f,0.9f,0.9f };

	//値を書き込むと自動的に転送されるらしい
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1.0f);

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

	float angle = 0.0f;	//カメラの回転角
	float angleY = 0.0f;

	float cameraY = 100;

	enum CameraMode
	{
		BIOHAZARD,
		TPS,
		IMAMADE,
	};
	int cameramode = TPS;
	
	float matWorldZ = 0;

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
		//DX12.commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		DX12.commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

		//3.画面クリア
		DX12.commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		DX12.commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


		//更新処理
		input_->Update();

		switch (cameramode)
		{
		case BIOHAZARD:
		{
			if (input_->TriggerKey(DIK_Q)) { cameramode = TPS; }
			viewProjection_.UpdatematView();
			for (size_t i = 0; i < _countof(object3ds); i++)
			{
				object3ds[i].Update(viewProjection_.matView, matProjection);
			}
			break;
		}
		case TPS:
		{
			//カメラ移動
			if (input_->TriggerKey(DIK_Q)) { cameramode = BIOHAZARD; }

			//if (input_->PushKey(DIK_W))
			//{
			//	matWorldZ = object3ds->matWorld.r[2];	//Z軸の抽出はできる
			//}

			if (input_->PushKey(DIK_RIGHT) || 
				input_->PushKey(DIK_LEFT) || 
				input_->PushKey(DIK_UP) || 
				input_->PushKey(DIK_DOWN))
			{
				if (input_->PushKey(DIK_RIGHT)) { 
					viewProjection_.eye.x += 2;
					viewProjection_.target.x += 2;
				}
				if (input_->PushKey(DIK_LEFT)) { 
					viewProjection_.eye.x -= 2; 
					viewProjection_.target.x -= 2;
				}
				if (input_->PushKey(DIK_UP)) {
					viewProjection_.eye.y += 2;
					viewProjection_.target.y += 2;
				}
				if (input_->PushKey(DIK_DOWN)) {
					viewProjection_.eye.y -= 2;
					viewProjection_.target.y -= 2;
				}
				viewProjection_.UpdatematView();
			}
	
			for (size_t i = 0; i < _countof(object3ds); i++)
			{
				object3ds[i].Update(viewProjection_.matView, matProjection);
			}
			break;
		}
		default:
		{
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

			//ビュー行列の計算
			if (input_->PushKey(DIK_D) || input_->PushKey(DIK_A))
			{
				if (input_->PushKey(DIK_D)) { angle += XMConvertToRadians(1.0f); }
				if (input_->PushKey(DIK_A)) { angle -= XMConvertToRadians(1.0f); }

				//angleラジアンだけY軸まわりに回転。半径は-100
				viewProjection_.eye.x = -cameraY * sinf(angle);
				viewProjection_.eye.z = -cameraY * cosf(angle);

				//射影行列の計算(更新しない場合は再計算しなくていいのでこの位置でOK)
				viewProjection_.UpdatematView();
			}

			if (input_->PushKey(DIK_W) || input_->PushKey(DIK_S))
			{
				if (input_->PushKey(DIK_W)) { angleY += XMConvertToRadians(1.0f); }
				if (input_->PushKey(DIK_S)) { angleY -= XMConvertToRadians(1.0f); }

				viewProjection_.eye.y = -cameraY * sinf(angleY);
				viewProjection_.eye.z = -cameraY * cosf(angleY);

				viewProjection_.UpdatematView();
			}

			if (input_->PushKey(DIK_UP) ||
				input_->PushKey(DIK_DOWN) ||
				input_->PushKey(DIK_RIGHT) ||
				input_->PushKey(DIK_LEFT)) {
				//座標を移動する処理
				if (input_->PushKey(DIK_UP)) { object3ds[0].position.z += 1.0f; }
				if (input_->PushKey(DIK_DOWN)) { object3ds[0].position.z -= 1.0f; }
				if (input_->PushKey(DIK_RIGHT)) { object3ds[0].position.x += 1.0f; }
				if (input_->PushKey(DIK_LEFT)) { object3ds[0].position.x -= 1.0f; }
			}

			//object3ds[1].rotation.x += 0.5f;

			for (size_t i = 0; i < _countof(object3ds); i++)
			{
				object3ds[i].Update(viewProjection_.matView, matProjection);
			}
			break;
		}
		}

		//constMapMaterial->color.x += materialColor.x;
		//constMapMaterial->color.y += materialColor.y;

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

		//描画処理
		for (int i = 0; i < _countof(object3ds); i++)
		{
			object3ds[i].Draw(DX12.commandList, vertexdate_.vbView, vertexdate_.ibView, _countof(indices));
		}

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

	return 0;
}