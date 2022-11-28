#include "Pipeline.h"
#include "Shader.h"
#include "Result.h"
#include <cassert>

PipelineSet CreateObject3DPipeline()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	//パイプライン構造体
	PipelineSet pipelineSet;

	//シェーダー
	Shader shader_;

	shader_.vsBlob = shader_.Compile(L"Resources\\Shader\\OBJVS.hlsl", "vs_5_0", shader_.vsBlob.Get(), "main");
	shader_.psBlob = shader_.Compile(L"Resources\\Shader\\OBJPS.hlsl", "ps_5_0", shader_.psBlob.Get(), "main");

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

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;	//一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[4] = {};
	//定数バッファ0番 b0
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;					//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	//テクスチャサンプラーの生成 s0
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

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;				//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);	//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;		//サンプラーの先頭アドレス	配列ならルートパラメータと同じような書き方ができる
	rootSignatureDesc.NumStaticSamplers = 1;				//サンプラーの数			配列ならルートパラメータと同じような書き方ができる

	//ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader_.errorBlob);
	assert(SUCCEEDED(result));
	result = dx12->device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootsignature));
	assert(SUCCEEDED(result));

	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = pipelineSet.rootsignature.Get();

	//パイプラインステートの生成
	//ComPtr<ID3D12PipelineState> pipelineState;
	result = dx12->device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineSet.pipelinestate));
	assert(SUCCEEDED(result));

	return pipelineSet;
}

PipelineSet CreateSpritePipeline()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	PipelineSet spritePipelineSet;

	//シェーダー
	Shader shader2_;

	shader2_.vsBlob = shader2_.Compile(L"Resources\\Shader\\SpriteVS.hlsl", "vs_5_0", shader2_.vsBlob.Get(), "main");
	shader2_.psBlob = shader2_.Compile(L"Resources\\Shader\\SpritePS.hlsl", "ps_5_0", shader2_.psBlob.Get(), "main");

	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc2{};

	//シェーダーの設定
	pipelineDesc2.VS.pShaderBytecode = shader2_.vsBlob->GetBufferPointer();
	pipelineDesc2.VS.BytecodeLength = shader2_.vsBlob->GetBufferSize();
	pipelineDesc2.PS.pShaderBytecode = shader2_.psBlob->GetBufferPointer();
	pipelineDesc2.PS.BytecodeLength = shader2_.psBlob->GetBufferSize();

	//サンプルマスクの設定
	pipelineDesc2.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//標準設定

	//ラスタライザの設定
	pipelineDesc2.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	//背面カリングをしない
	pipelineDesc2.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//ポリゴン内塗りつぶし
	pipelineDesc2.RasterizerState.DepthClipEnable = true;			//深度クリッピングを有効に

	//ブレンドステート
	pipelineDesc2.BlendState.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA全てのチャンネルを描画

	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc2 = pipelineDesc2.BlendState.RenderTarget[0];
	blenddesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //RGBAすべてのチャンネルを描画

	blenddesc2.BlendEnable = true;					//ブレンドを有効にする

	//あるふぁ～～～～
	blenddesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc2.SrcBlendAlpha = D3D12_BLEND_ZERO;		//ソースの値を100%使う
	blenddesc2.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う


	int blendMode2 = ALPHA;

	if (blendMode2 == ADD)
	{
		//加算合成
		blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;			//加算
		blenddesc2.SrcBlend = D3D12_BLEND_ONE;			//ソースの値を100%使う
		blenddesc2.DestBlend = D3D12_BLEND_ONE;			//デストの値を100%使う
	}
	if (blendMode2 == SUB)
	{
		//減算合成
		blenddesc2.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//デストからソースを減算
		blenddesc2.SrcBlend = D3D12_BLEND_ONE;			//ソースの値を100%使う
		blenddesc2.DestBlend = D3D12_BLEND_ONE;			//デストの値を100%使う
	}
	if (blendMode2 == INV)
	{
		//色反転
		blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;			//加算
		blenddesc2.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-デストカラーの値
		blenddesc2.DestBlend = D3D12_BLEND_ZERO;			//使わない
	}
	if (blendMode2 == ALPHA)
	{
		//半透明合成
		blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;			//加算
		blenddesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;		//ソースのアルファ値
		blenddesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値
	}

	//頂点レイアウト
	static D3D12_INPUT_ELEMENT_DESC inputLayout2[] = {
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
	pipelineDesc2.InputLayout.pInputElementDescs = inputLayout2;
	pipelineDesc2.InputLayout.NumElements = _countof(inputLayout2);

	//図形の形状設定
	pipelineDesc2.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	pipelineDesc2.NumRenderTargets = 1;	//描画対象は1つ
	pipelineDesc2.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pipelineDesc2.SampleDesc.Count = 1;	//1ピクセルにつき1回サンプリング

	//深度ステートの設定
	pipelineDesc2.DepthStencilState.DepthEnable = false;	//深度テストを行わない
	pipelineDesc2.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;	//常に上書き
	
	//いらないかもしれんやつら
	pipelineDesc2.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可
	pipelineDesc2.DSVFormat = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange2{};
	descriptorRange2.NumDescriptors = 1;	//一度の描画に使うテクスチャが1枚なので1
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descriptorRange2.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams2[3] = {};
	//定数バッファ0番 b0
	rootParams2[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams2[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams2[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams2[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	rootParams2[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootParams2[1].DescriptorTable.pDescriptorRanges = &descriptorRange2;					//デスクリプタレンジ
	rootParams2[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams2[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

	//テクスチャサンプラーの生成 s0
	D3D12_STATIC_SAMPLER_DESC samplerDesc2{};
	samplerDesc2.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc2.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc2.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc2.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc2.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc2.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc2.MinLOD = 0.0f;
	samplerDesc2.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc2.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//ルートシグネチャ
	//ComPtr<ID3D12RootSignature> rootSignature;

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc2{};
	rootSignatureDesc2.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc2.pParameters = rootParams2;				//ルートパラメータの先頭アドレス
	rootSignatureDesc2.NumParameters = _countof(rootParams2);	//ルートパラメータ数
	rootSignatureDesc2.pStaticSamplers = &samplerDesc2;		//サンプラーの先頭アドレス	配列ならルートパラメータと同じような書き方ができる
	rootSignatureDesc2.NumStaticSamplers = 1;				//サンプラーの数			配列ならルートパラメータと同じような書き方ができる

	//ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob2;
	result = D3D12SerializeRootSignature(&rootSignatureDesc2, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob2, &shader2_.errorBlob);
	assert(SUCCEEDED(result));
	result = dx12->device->CreateRootSignature(0, rootSigBlob2->GetBufferPointer(), rootSigBlob2->GetBufferSize(),
		IID_PPV_ARGS(&spritePipelineSet.rootsignature));
	assert(SUCCEEDED(result));

	//パイプラインにルートシグネチャをセット
	pipelineDesc2.pRootSignature = spritePipelineSet.rootsignature.Get();

	//パイプラインステートの生成
	//ComPtr<ID3D12PipelineState> pipelineState;
	result = dx12->device->CreateGraphicsPipelineState(&pipelineDesc2, IID_PPV_ARGS(&spritePipelineSet.pipelinestate));
	assert(SUCCEEDED(result));

	return spritePipelineSet;
}

PipelineSet CreateGeometryPipeline()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	//パイプライン構造体
	PipelineSet pipelineSet;

	//シェーダー
	Shader shader_;

	shader_.vsBlob = shader_.Compile(L"Resources\\Shader\\ParticleVS.hlsl", "vs_5_0", shader_.vsBlob.Get(), "main");
	shader_.psBlob = shader_.Compile(L"Resources\\Shader\\ParticlePS.hlsl", "ps_5_0", shader_.psBlob.Get(), "main");
	shader_.gsBlob = shader_.Compile(L"Resources\\Shader\\ParticleGS.hlsl", "gs_5_0", shader_.gsBlob.Get(), "main");

	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = shader_.vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = shader_.vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = shader_.psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = shader_.psBlob->GetBufferSize();
	pipelineDesc.GS.pShaderBytecode = shader_.gsBlob->GetBufferPointer();
	pipelineDesc.GS.BytecodeLength = shader_.gsBlob->GetBufferSize();

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

	int blendMode = ADD;

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
			{ //スケールの情報
				"SCALE",0,DXGI_FORMAT_R32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
			{ //色の情報
				"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
	};

	//頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	//その他の設定
	pipelineDesc.NumRenderTargets = 1;	//描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1;	//1ピクセルにつき1回サンプリング

	pipelineDesc.DepthStencilState.DepthEnable = true;	//深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;//書き込み不可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	//小さければ許可
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;	//一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[2] = {};
	//定数バッファ0番 b0
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;					//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	

	//テクスチャサンプラーの生成 s0
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

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;				//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);	//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;		//サンプラーの先頭アドレス	配列ならルートパラメータと同じような書き方ができる
	rootSignatureDesc.NumStaticSamplers = 1;				//サンプラーの数			配列ならルートパラメータと同じような書き方ができる

	//ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader_.errorBlob);
	assert(SUCCEEDED(result));
	result = dx12->device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootsignature));
	assert(SUCCEEDED(result));

	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = pipelineSet.rootsignature.Get();

	//パイプラインステートの生成
	//ComPtr<ID3D12PipelineState> pipelineState;
	result = dx12->device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineSet.pipelinestate));
	assert(SUCCEEDED(result));

	return pipelineSet;
}
