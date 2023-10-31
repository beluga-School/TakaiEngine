#include "Pipeline.h"
#include "Shader.h"
#include "Result.h"
#include <cassert>
#include "StringUtil.h"

std::map<std::string, PipelineSet> PipelineManager::sPipelines;

PipelineSet CreateLambertPipeline()
{
	DirectX12* dx12 = DirectX12::Get();

	//パイプライン構造体
	PipelineSet pipelineSet;

	//シェーダー
	Shader shader_;

	shader_.mVsBlob = shader_.Compile(L"Resources\\Shader\\LambertVS.hlsl", "vs_5_0", shader_.mVsBlob.Get(), "main");
	shader_.mPsBlob = shader_.Compile(L"Resources\\Shader\\LambertPS.hlsl", "ps_5_0", shader_.mPsBlob.Get(), "main");

	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = shader_.mVsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = shader_.mVsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = shader_.mPsBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = shader_.mPsBlob->GetBufferSize();

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
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う

	int32_t blendMode = ALPHA;

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
	static D3D12_INPUT_ELEMENT_DESC sInputLayout[] = {
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
	pipelineDesc.InputLayout.pInputElementDescs = sInputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(sInputLayout);

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
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	sResult = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader_.mErrorBlob);
	assert(SUCCEEDED(sResult));
	sResult = dx12->mDevice->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.mRootsignature));
	assert(SUCCEEDED(sResult));

	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = pipelineSet.mRootsignature.Get();

	//パイプラインステートの生成
	//ComPtr<ID3D12PipelineState> pipelineState;
	sResult = dx12->mDevice->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineSet.mPipelinestate));
	assert(SUCCEEDED(sResult));

	return pipelineSet;
}

PipelineSet::PipelineSet()
{
	//デフォルトのシェーダー
	vs.shaderName = "Resources\\Shader\\PhongVS.hlsl";
	ps.shaderName = "Resources\\Shader\\PhongPS.hlsl";
	gs.shaderName = "";

	//サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//標準設定

	//ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	//背面カリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true;			//深度クリッピングを有効に

	//ブレンドステート
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA全てのチャンネルを描画

	//ブレンド設定
	blenddesc = pipelineDesc.BlendState.RenderTarget[0];

	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //RGBAすべてのチャンネルを描画
	blenddesc.BlendEnable = true;					//ブレンドを有効にする

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う

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
	descriptorRange.NumDescriptors = 1;	//一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータの設定
	rootParams.resize(paramSize);
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
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams[0];				//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();	//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;		//サンプラーの先頭アドレス	配列ならルートパラメータと同じような書き方ができる
	rootSignatureDesc.NumStaticSamplers = 1;				//サンプラーの数			配列ならルートパラメータと同じような書き方ができる
}

void PipelineSet::Create()
{
	DirectX12* dx12 = DirectX12::Get();

	//シェーダー
	Shader shader_;

	if (vs.shaderName != "")
	{
		shader_.mVsBlob = shader_.Compile(ConvertStringToWChar(vs.shaderName).c_str(), "vs_5_0", shader_.mVsBlob.Get(), "main");
	
		pipelineDesc.VS.pShaderBytecode = shader_.mVsBlob->GetBufferPointer();
		pipelineDesc.VS.BytecodeLength = shader_.mVsBlob->GetBufferSize();
	}
	if (ps.shaderName != "")
	{
		shader_.mPsBlob = shader_.Compile(ConvertStringToWChar(ps.shaderName).c_str(), "ps_5_0", shader_.mPsBlob.Get(), "main");
	
		pipelineDesc.PS.pShaderBytecode = shader_.mPsBlob->GetBufferPointer();
		pipelineDesc.PS.BytecodeLength = shader_.mPsBlob->GetBufferSize();
	}
	if (gs.shaderName != "")
	{
		shader_.mGsBlob = shader_.Compile(ConvertStringToWChar(gs.shaderName).c_str(), "gs_5_0", shader_.mGsBlob.Get(), "main");
	
		pipelineDesc.GS.pShaderBytecode = shader_.mGsBlob->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = shader_.mGsBlob->GetBufferSize();
	}

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

	//ブレンドステート
	pipelineDesc.BlendState.RenderTarget[0] = blenddesc;
	pipelineDesc.BlendState.RenderTarget[1] = blenddesc;


	//頂点レイアウトの設定
	if (inputLayout.size() > 0)
	{
		pipelineDesc.InputLayout.pInputElementDescs = &inputLayout[0];
		pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();
	}
	else
	{
		pipelineDesc.InputLayout.pInputElementDescs = nullptr;
		pipelineDesc.InputLayout.NumElements = 0;
	}

	if (rootParams.size() > 0)
	{
		rootSignatureDesc.pParameters = &rootParams[0];				//ルートパラメータの先頭アドレス
		rootSignatureDesc.NumParameters = (UINT)rootParams.size();	//ルートパラメータ数
	}
	else
	{
		rootSignatureDesc.pParameters = nullptr;
		rootSignatureDesc.NumParameters = 0;
	}

	//ルートシグネチャのシリアライズ
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	sResult = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader_.mErrorBlob);
	assert(SUCCEEDED(sResult));

	//ルートシグネチャ生成
	sResult = dx12->mDevice->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&mRootsignature));
	assert(SUCCEEDED(sResult));

	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = mRootsignature.Get();

	//パイプラインステートの生成
	sResult = dx12->mDevice->CreateGraphicsPipelineState(&pipelineDesc,
		IID_PPV_ARGS(&mPipelinestate));
	assert(SUCCEEDED(sResult));
}

PipelineSet PipelineManager::GetPipeLine(const std::string& name)
{
	return sPipelines[name];
}

void PipelineManager::Initialize()
{
	Object3DPipeLine();

	SpritePipeLine();

	NonePostEffectPipeLine();

	GaussianBlurPipeLine();

	ShiftBlurPipeLine();

	MultiRenderPipeLine();

	CG4PipeLine();

	PhongPipeLine();

	SingleColorPipeLine();

	ToonPipeLine();

	ToonNoDepthWritePipeLine();

	OutLinePipeLine();

	SkydomePipeLine();

	GroundToonPipeLine();

	GroundToonNoDepthWritePipeLine();

	WireFramePipeLine();

	PerlinNoisePipeLine();

	TextureBlendPipeLine();

	DisolvePipeLine();

	PerlinNoisePostEffectPipeLine();

	LinePipeLine();

	DitherTransparentPipeLine();
}

void PipelineManager::Object3DPipeLine()
{
	PipelineSet pipeLineSet;

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pipeLineSet.inputLayout =
	{
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

	//ルートパラメータの設定
	pipeLineSet.paramSize = 5;
	pipeLineSet.rootParams.resize(pipeLineSet.paramSize);
	//定数バッファ0番 b0
	pipeLineSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pipeLineSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pipeLineSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pipeLineSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pipeLineSet.descriptorRange;					//デスクリプタレンジ
	pipeLineSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pipeLineSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pipeLineSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pipeLineSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pipeLineSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pipeLineSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pipeLineSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pipeLineSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	//パイプライン生成
	pipeLineSet.Create();

	std::string pipeLineName = "Object3D";
	sPipelines[pipeLineName] = pipeLineSet;
}

void PipelineManager::SpritePipeLine()
{
	PipelineSet pSet;

	//シェーダー設定
	pSet.vs.shaderName = "Resources\\Shader\\SpriteVS.hlsl";
	pSet.ps.shaderName = "Resources\\Shader\\SpritePS.hlsl";

	//インプットレイアウト
	pSet.inputLayout =
	{
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

	//サンプルマスクの設定
	pSet.pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//標準設定

	//ラスタライザの設定
	pSet.pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	//背面カリングをしない
	pSet.pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//ポリゴン内塗りつぶし
	pSet.pipelineDesc.RasterizerState.DepthClipEnable = true;			//深度クリッピングを有効に

	//ブレンドステート
	pSet.pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA全てのチャンネルを描画

	pSet.blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //RGBAすべてのチャンネルを描画

	pSet.blenddesc.BlendEnable = true;					//ブレンドを有効にする

	//あるふぁ～～～～
	pSet.blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	pSet.blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	pSet.blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う


	//頂点レイアウト
	pSet.inputLayout = {
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

	//図形の形状設定
	pSet.pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	pSet.pipelineDesc.NumRenderTargets = 1;	//描画対象は1つ
	pSet.pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pSet.pipelineDesc.SampleDesc.Count = 1;	//1ピクセルにつき1回サンプリング

	//深度ステートの設定
	pSet.pipelineDesc.DepthStencilState.DepthEnable = false;	//深度テストを行わない
	pSet.pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;	//常に上書き

	//いらないかもしれんやつら
	pSet.pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可
	pSet.pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット

	pSet.descriptorRange.NumDescriptors = 1;	//一度の描画に使うテクスチャが1枚なので1
	pSet.descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pSet.descriptorRange.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	pSet.descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータの設定
	pSet.paramSize = 2;
	pSet.rootParams.resize(pSet.paramSize);
	//定数バッファ0番 b0
	pSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pSet.descriptorRange;					//デスクリプタレンジ
	pSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

	//テクスチャサンプラーの生成 s0

	pSet.samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pSet.samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pSet.samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pSet.samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	pSet.samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	pSet.samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	pSet.samplerDesc.MinLOD = 0.0f;
	pSet.samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	pSet.samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	//ルートシグネチャの設定

	pSet.rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	pSet.rootSignatureDesc.NumStaticSamplers = 1;

	pSet.Create();

	std::string pipeLineName = "Sprite";
	sPipelines[pipeLineName] = pSet;
}

void PipelineManager::NonePostEffectPipeLine()
{
	PipelineSet pSet;

	//シェーダー設定
	pSet.vs.shaderName = "Resources\\Shader\\NonePostEffectVS.hlsl";
	pSet.ps.shaderName = "Resources\\Shader\\NonePostEffectPS.hlsl";

	//インプットレイアウト
	pSet.inputLayout =
	{
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

	pSet.pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	pSet.pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	pSet.blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	pSet.blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	pSet.blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う

	pSet.blendMode = ALPHA;

	pSet.pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pSet.pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//UVずらし系のポストエフェクトを作った際に、逆側の色を拾ってこないための設定
	pSet.samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	pSet.samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	D3D12_DESCRIPTOR_RANGE descRangeSRV0;
	descRangeSRV0.NumDescriptors = 1;
	descRangeSRV0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRangeSRV0.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descRangeSRV0.RegisterSpace = 0;
	descRangeSRV0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descRangeSRV1;
	descRangeSRV1.NumDescriptors = 1;
	descRangeSRV1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRangeSRV1.BaseShaderRegister = 1;	//テクスチャレジスタ1番
	descRangeSRV1.RegisterSpace = 0;
	descRangeSRV1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	//ルートパラメータの設定
	pSet.paramSize = 3;
	pSet.rootParams.resize(pSet.paramSize);

	pSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[1].DescriptorTable.pDescriptorRanges = &descRangeSRV0;					//デスクリプタレンジ
	pSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ1番 t1
	pSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[2].DescriptorTable.pDescriptorRanges = &descRangeSRV1;					//デスクリプタレンジ
	pSet.rootParams[2].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

	pSet.Create();

	std::string pipeLineName = "None";
	sPipelines[pipeLineName] = pSet;
}

void PipelineManager::MultiRenderPipeLine()
{
	PipelineSet pipeLineSet;

	//シェーダー設定
	pipeLineSet.vs.shaderName = "Resources\\Shader\\MultiRenderVS.hlsl";
	pipeLineSet.ps.shaderName = "Resources\\Shader\\MultiRenderPS.hlsl";

	pipeLineSet.pipelineDesc.NumRenderTargets = 2;	//描画対象は1つ
	pipeLineSet.pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pipeLineSet.pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pipeLineSet.inputLayout =
	{
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

	//ルートパラメータの設定
	pipeLineSet.paramSize = 5;
	pipeLineSet.rootParams.resize(pipeLineSet.paramSize);
	//定数バッファ0番 b0
	pipeLineSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pipeLineSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pipeLineSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pipeLineSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pipeLineSet.descriptorRange;					//デスクリプタレンジ
	pipeLineSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pipeLineSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pipeLineSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pipeLineSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pipeLineSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pipeLineSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pipeLineSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pipeLineSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	//パイプライン生成
	pipeLineSet.Create();

	std::string pipeLineName = "MultiRender";
	sPipelines[pipeLineName] = pipeLineSet;
}

void PipelineManager::GaussianBlurPipeLine()
{
	PipelineSet pSet;

	//シェーダー設定
	pSet.vs.shaderName = "Resources\\Shader\\GaussianBlurVS.hlsl";
	pSet.ps.shaderName = "Resources\\Shader\\GaussianBlurPS.hlsl";

	//インプットレイアウト
	pSet.inputLayout =
	{
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

	pSet.pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	pSet.pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	pSet.blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	pSet.blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	pSet.blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う

	pSet.blendMode = ALPHA;

	pSet.pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pSet.pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//UVずらし系のポストエフェクトを作った際に、逆側の色を拾ってこないための設定
	pSet.samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	pSet.samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	D3D12_DESCRIPTOR_RANGE descRangeSRV1;
	descRangeSRV1.NumDescriptors = 1;
	descRangeSRV1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRangeSRV1.BaseShaderRegister = 1;	//テクスチャレジスタ1番

	//ルートパラメータの設定
	pSet.paramSize = 3;
	pSet.rootParams.resize(pSet.paramSize);

	pSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pSet.descriptorRange;					//デスクリプタレンジ
	pSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ1番 t1
	pSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[2].DescriptorTable.pDescriptorRanges = &descRangeSRV1;					//デスクリプタレンジ
	pSet.rootParams[2].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

	pSet.Create();

	std::string pipeLineName = "GaussianBlur";
	sPipelines[pipeLineName] = pSet;
}

void PipelineManager::ShiftBlurPipeLine()
{
	PipelineSet pSet;

	//シェーダー設定
	pSet.vs.shaderName = "Resources\\Shader\\ShiftBlurVS.hlsl";
	pSet.ps.shaderName = "Resources\\Shader\\ShiftBlurPS.hlsl";

	//インプットレイアウト
	pSet.inputLayout =
	{
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

	pSet.pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	pSet.pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	pSet.blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	pSet.blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	pSet.blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う

	pSet.blendMode = ALPHA;

	pSet.pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pSet.pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//UVずらし系のポストエフェクトを作った際に、逆側の色を拾ってこないための設定
	pSet.samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	pSet.samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	pSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pSet.descriptorRange;					//デスクリプタレンジ
	pSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

	pSet.Create();

	std::string pipeLineName = "ShiftBlur";
	sPipelines[pipeLineName] = pSet;
}

void PipelineManager::CG4PipeLine()
{
	PipelineSet pSet;

	//シェーダー設定
	pSet.vs.shaderName = "Resources\\Shader\\CG4VS.hlsl";
	pSet.ps.shaderName = "Resources\\Shader\\CG4PS.hlsl";

	//インプットレイアウト
	pSet.inputLayout =
	{
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

	pSet.pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	pSet.pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	pSet.blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	pSet.blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	pSet.blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う

	pSet.blendMode = ALPHA;

	pSet.pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pSet.pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//UVずらし系のポストエフェクトを作った際に、逆側の色を拾ってこないための設定
	pSet.samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	pSet.samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	D3D12_DESCRIPTOR_RANGE descRangeSRV0;
	descRangeSRV0.NumDescriptors = 1;
	descRangeSRV0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRangeSRV0.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descRangeSRV0.RegisterSpace = 0;
	descRangeSRV0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descRangeSRV1;
	descRangeSRV1.NumDescriptors = 1;
	descRangeSRV1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRangeSRV1.BaseShaderRegister = 1;	//テクスチャレジスタ1番
	descRangeSRV1.RegisterSpace = 0;
	descRangeSRV1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	//ルートパラメータの設定
	pSet.paramSize = 3;
	pSet.rootParams.resize(pSet.paramSize);

	pSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[1].DescriptorTable.pDescriptorRanges = &descRangeSRV0;					//デスクリプタレンジ
	pSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ1番 t1
	pSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[2].DescriptorTable.pDescriptorRanges = &descRangeSRV1;					//デスクリプタレンジ
	pSet.rootParams[2].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

	pSet.Create();

	std::string pipeLineName = "CG4";
	sPipelines[pipeLineName] = pSet;
}

void PipelineManager::PhongPipeLine()
{
	PipelineSet pSet;

	pSet.pipelineDesc.NumRenderTargets = 2;	//描画対象は1つ
	pSet.pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pSet.pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pSet.inputLayout =
	{
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

	//ルートパラメータの設定
	pSet.paramSize = 5;
	pSet.rootParams.resize(pSet.paramSize);
	//定数バッファ0番 b0
	pSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pSet.descriptorRange;					//デスクリプタレンジ
	pSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	pSet.Create();

	std::string pipeLineName = "Phong";
	sPipelines[pipeLineName] = pSet;
}

void PipelineManager::SingleColorPipeLine()
{
	PipelineSet pSet;

	//シェーダー設定
	pSet.vs.shaderName = "Resources\\Shader\\SingleColorVS.hlsl";
	pSet.ps.shaderName = "Resources\\Shader\\SingleColorPS.hlsl";

	pSet.pipelineDesc.NumRenderTargets = 2;	//描画対象は1つ
	pSet.pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pSet.pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pSet.inputLayout =
	{
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

	//ルートパラメータの設定
	pSet.paramSize = 5;
	pSet.rootParams.resize(pSet.paramSize);
	//定数バッファ0番 b0
	pSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pSet.descriptorRange;					//デスクリプタレンジ
	pSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	pSet.Create();

	std::string pipeLineName = "SingleColor";
	sPipelines[pipeLineName] = pSet;
}

void PipelineManager::ToonPipeLine()
{
	PipelineSet pSet;

	//シェーダー設定
	pSet.vs.shaderName = "Resources\\Shader\\ToonVS.hlsl";
	pSet.ps.shaderName = "Resources\\Shader\\ToonPS.hlsl";

	pSet.pipelineDesc.NumRenderTargets = 2;	//描画対象は1つ
	pSet.pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pSet.pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA

	pSet.pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pSet.inputLayout =
	{
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

	//ルートパラメータの設定
	pSet.paramSize = 6;
	pSet.rootParams.resize(pSet.paramSize);
	//定数バッファ0番 b0
	pSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pSet.descriptorRange;					//デスクリプタレンジ
	pSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ4番 b4
	pSet.rootParams[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[5].Descriptor.ShaderRegister = 4;					//定数バッファ番号
	pSet.rootParams[5].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	pSet.Create();

	std::string pipeLineName = "Toon";
	sPipelines[pipeLineName] = pSet;
}

void PipelineManager::ToonNoDepthWritePipeLine()
{
	PipelineSet pSet;

	//シェーダー設定
	pSet.vs.shaderName = "Resources\\Shader\\ToonVS.hlsl";
	pSet.ps.shaderName = "Resources\\Shader\\ToonPS.hlsl";

	pSet.pipelineDesc.NumRenderTargets = 2;	//描画対象は1つ
	pSet.pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pSet.pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA

	pSet.pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	pSet.pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	//書き込み禁止

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pSet.inputLayout =
	{
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

	//ルートパラメータの設定
	pSet.paramSize = 5;
	pSet.rootParams.resize(pSet.paramSize);
	//定数バッファ0番 b0
	pSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pSet.descriptorRange;					//デスクリプタレンジ
	pSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	pSet.Create();

	std::string pipeLineName = "ToonNDW";
	sPipelines[pipeLineName] = pSet;
}

void PipelineManager::OutLinePipeLine()
{
	PipelineSet pSet;

	//シェーダー設定
	pSet.vs.shaderName = "Resources\\Shader\\OutLineVS.hlsl";
	pSet.ps.shaderName = "Resources\\Shader\\OutLinePS.hlsl";

	pSet.pipelineDesc.NumRenderTargets = 2;	//描画対象は1つ
	pSet.pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pSet.pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA

	pSet.pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;

	pSet.pipelineDesc.DepthStencilState.DepthEnable = true;	//深度テストを行う
	pSet.pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	//書き込み禁止

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pSet.inputLayout =
	{
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

	//ルートパラメータの設定
	pSet.paramSize = 2;
	pSet.rootParams.resize(pSet.paramSize);
	//定数バッファ0番 b0
	pSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[1].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pSet.rootParams[1].Descriptor.RegisterSpace = 0;					//デフォルト値
	pSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	pSet.Create();

	std::string pipeLineName = "OutLine";
	sPipelines[pipeLineName] = pSet;
}

void PipelineManager::SkydomePipeLine()
{
	PipelineSet pipeLineSet;

	//シェーダー設定
	pipeLineSet.vs.shaderName = "Resources\\Shader\\SkydomeVS.hlsl";
	pipeLineSet.ps.shaderName = "Resources\\Shader\\SkydomePS.hlsl";

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pipeLineSet.inputLayout =
	{
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

	//ルートパラメータの設定
	pipeLineSet.paramSize = 5;
	pipeLineSet.rootParams.resize(pipeLineSet.paramSize);
	//定数バッファ0番 b0
	pipeLineSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pipeLineSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pipeLineSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pipeLineSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pipeLineSet.descriptorRange;					//デスクリプタレンジ
	pipeLineSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pipeLineSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pipeLineSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pipeLineSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pipeLineSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pipeLineSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pipeLineSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pipeLineSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	//パイプライン生成
	pipeLineSet.Create();

	std::string pipeLineName = "Skydome";
	sPipelines[pipeLineName] = pipeLineSet;
}

void PipelineManager::GroundToonPipeLine()
{
	PipelineSet pSet;

	//シェーダー設定
	pSet.vs.shaderName = "Resources\\Shader\\Ground\\GroundVS.hlsl";
	pSet.ps.shaderName = "Resources\\Shader\\Ground\\GroundPS.hlsl";

	pSet.pipelineDesc.NumRenderTargets = 2;	//描画対象は1つ
	pSet.pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pSet.pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA

	pSet.pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	
	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pSet.inputLayout =
	{
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

	//ルートパラメータの設定
	pSet.paramSize = 6;
	pSet.rootParams.resize(pSet.paramSize);
	//定数バッファ0番 b0
	pSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pSet.descriptorRange;					//デスクリプタレンジ
	pSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ4番 b4
	pSet.rootParams[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[5].Descriptor.ShaderRegister = 4;					//定数バッファ番号
	pSet.rootParams[5].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える


	pSet.Create();

	std::string pipeLineName = "GroundToon";
	sPipelines[pipeLineName] = pSet;
}

void PipelineManager::GroundToonNoDepthWritePipeLine()
{
	PipelineSet pSet;

	//シェーダー設定
	pSet.vs.shaderName = "Resources\\Shader\\Ground\\GroundVS.hlsl";
	pSet.ps.shaderName = "Resources\\Shader\\Ground\\GroundPS.hlsl";

	pSet.pipelineDesc.NumRenderTargets = 2;	//描画対象は1つ
	pSet.pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pSet.pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA

	pSet.pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

	pSet.pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	//書き込み禁止

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pSet.inputLayout =
	{
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

	//ルートパラメータの設定
	pSet.paramSize = 5;
	pSet.rootParams.resize(pSet.paramSize);
	//定数バッファ0番 b0
	pSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pSet.descriptorRange;					//デスクリプタレンジ
	pSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	pSet.Create();

	std::string pipeLineName = "GroundToonNDW";
	sPipelines[pipeLineName] = pSet;
}

void PipelineManager::WireFramePipeLine()
{
	PipelineSet pipeLineSet;

	//シェーダー設定
	pipeLineSet.vs.shaderName = "Resources\\Shader\\SkydomeVS.hlsl";
	pipeLineSet.ps.shaderName = "Resources\\Shader\\SkydomePS.hlsl";

	pipeLineSet.pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	pipeLineSet.pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pipeLineSet.inputLayout =
	{
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

	//ルートパラメータの設定
	pipeLineSet.paramSize = 5;
	pipeLineSet.rootParams.resize(pipeLineSet.paramSize);
	//定数バッファ0番 b0
	pipeLineSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pipeLineSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pipeLineSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pipeLineSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pipeLineSet.descriptorRange;					//デスクリプタレンジ
	pipeLineSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pipeLineSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pipeLineSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pipeLineSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pipeLineSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pipeLineSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pipeLineSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pipeLineSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	//パイプライン生成
	pipeLineSet.Create();

	std::string pipeLineName = "WireFrame";
	sPipelines[pipeLineName] = pipeLineSet;
}

void PipelineManager::PerlinNoisePipeLine()
{
	PipelineSet pipeLineSet;

	//シェーダー設定
	pipeLineSet.vs.shaderName = "Resources\\Shader\\PerlinNoise\\PerlinNoiseVS.hlsl";
	pipeLineSet.ps.shaderName = "Resources\\Shader\\PerlinNoise\\PerlinNoisePS.hlsl";

	pipeLineSet.pipelineDesc.NumRenderTargets = 2;	//描画対象は1つ
	pipeLineSet.pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pipeLineSet.pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pipeLineSet.inputLayout =
	{
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

	//ルートパラメータの設定
	pipeLineSet.paramSize = 5;
	pipeLineSet.rootParams.resize(pipeLineSet.paramSize);
	//定数バッファ0番 b0
	pipeLineSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pipeLineSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pipeLineSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pipeLineSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pipeLineSet.descriptorRange;					//デスクリプタレンジ
	pipeLineSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pipeLineSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pipeLineSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pipeLineSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pipeLineSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pipeLineSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pipeLineSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pipeLineSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える


	//パイプライン生成
	pipeLineSet.Create();

	std::string pipeLineName = "PerlinNoise";
	sPipelines[pipeLineName] = pipeLineSet;
}

void PipelineManager::TextureBlendPipeLine()
{
	PipelineSet pipeLineSet;

	//シェーダー設定
	pipeLineSet.vs.shaderName = "Resources\\Shader\\TextureBlend\\TextureBlendVS.hlsl";
	pipeLineSet.ps.shaderName = "Resources\\Shader\\TextureBlend\\TextureBlendPS.hlsl";
	
	pipeLineSet.pipelineDesc.NumRenderTargets = 2;	//描画対象は1つ
	pipeLineSet.pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pipeLineSet.pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pipeLineSet.inputLayout =
	{
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

	//ルートパラメータの設定
	//テクスチャレジスタが3つ必要
	//
	pipeLineSet.paramSize = 7;
	pipeLineSet.rootParams.resize(pipeLineSet.paramSize);
	//定数バッファ0番 b0
	pipeLineSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pipeLineSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pipeLineSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pipeLineSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pipeLineSet.descriptorRange;					//デスクリプタレンジ
	pipeLineSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pipeLineSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pipeLineSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pipeLineSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pipeLineSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pipeLineSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pipeLineSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pipeLineSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//テクスチャレジスタ1番 t1
	//レンジを使いまわして書き換える
	D3D12_DESCRIPTOR_RANGE dRange2 = pipeLineSet.descriptorRange;
	dRange2.BaseShaderRegister = 1;
	
	pipeLineSet.rootParams[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pipeLineSet.rootParams[5].DescriptorTable.pDescriptorRanges = &dRange2;					//デスクリプタレンジ
	pipeLineSet.rootParams[5].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pipeLineSet.rootParams[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ2番 t2
	//レンジを使いまわして書き換える
	D3D12_DESCRIPTOR_RANGE dRange3 = pipeLineSet.descriptorRange;
	dRange3.BaseShaderRegister = 2;

	pipeLineSet.rootParams[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pipeLineSet.rootParams[6].DescriptorTable.pDescriptorRanges = &dRange3;					//デスクリプタレンジ
	pipeLineSet.rootParams[6].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pipeLineSet.rootParams[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

	//パイプライン生成
	pipeLineSet.Create();

	std::string pipeLineName = "TextureBlend";
	sPipelines[pipeLineName] = pipeLineSet;
}

void PipelineManager::DisolvePipeLine()
{
	PipelineSet pipeLineSet;

	//シェーダー設定
	pipeLineSet.vs.shaderName = "Resources\\Shader\\Disolve\\DisolveVS.hlsl";
	pipeLineSet.ps.shaderName = "Resources\\Shader\\Disolve\\DisolvePS.hlsl";

	pipeLineSet.pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	pipeLineSet.pipelineDesc.NumRenderTargets = 2;	//描画対象は1つ
	pipeLineSet.pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pipeLineSet.pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pipeLineSet.inputLayout =
	{
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

	//ルートパラメータの設定
	//テクスチャレジスタが3つ必要
	//
	pipeLineSet.paramSize = 7;
	pipeLineSet.rootParams.resize(pipeLineSet.paramSize);
	//定数バッファ0番 b0
	pipeLineSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pipeLineSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pipeLineSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pipeLineSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pipeLineSet.descriptorRange;					//デスクリプタレンジ
	pipeLineSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pipeLineSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pipeLineSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pipeLineSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pipeLineSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pipeLineSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pipeLineSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pipeLineSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ4番 b4
	pipeLineSet.rootParams[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[5].Descriptor.ShaderRegister = 4;					//定数バッファ番号
	pipeLineSet.rootParams[5].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	//テクスチャレジスタ1番 t1
	//レンジを使いまわして書き換える
	D3D12_DESCRIPTOR_RANGE dRange2 = pipeLineSet.descriptorRange;
	dRange2.BaseShaderRegister = 1;

	pipeLineSet.rootParams[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pipeLineSet.rootParams[6].DescriptorTable.pDescriptorRanges = &dRange2;					//デスクリプタレンジ
	pipeLineSet.rootParams[6].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pipeLineSet.rootParams[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

	//パイプライン生成
	pipeLineSet.Create();

	std::string pipeLineName = "Disolve";
	sPipelines[pipeLineName] = pipeLineSet;
}

void PipelineManager::PerlinNoisePostEffectPipeLine()
{
	PipelineSet pSet;

	//シェーダー設定
	pSet.vs.shaderName = "Resources\\Shader\\PerlinNoisePE\\PerlinNoisePEVS.hlsl";
	pSet.ps.shaderName = "Resources\\Shader\\PerlinNoisePE\\PerlinNoisePEPS.hlsl";

	//インプットレイアウト
	pSet.inputLayout =
	{
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

	pSet.pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	pSet.pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	pSet.blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	pSet.blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	pSet.blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う

	pSet.blendMode = ALPHA;

	pSet.pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pSet.pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//UVずらし系のポストエフェクトを作った際に、逆側の色を拾ってこないための設定
	pSet.samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	pSet.samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	D3D12_DESCRIPTOR_RANGE descRangeSRV0;
	descRangeSRV0.NumDescriptors = 1;
	descRangeSRV0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRangeSRV0.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descRangeSRV0.RegisterSpace = 0;
	descRangeSRV0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descRangeSRV1;
	descRangeSRV1.NumDescriptors = 1;
	descRangeSRV1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRangeSRV1.BaseShaderRegister = 1;	//テクスチャレジスタ1番
	descRangeSRV1.RegisterSpace = 0;
	descRangeSRV1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータの設定
	pSet.paramSize = 3;
	pSet.rootParams.resize(pSet.paramSize);

	//定数バッファ0番
	pSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[1].DescriptorTable.pDescriptorRanges = &descRangeSRV0;					//デスクリプタレンジ
	pSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ1番 t1
	pSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pSet.rootParams[2].DescriptorTable.pDescriptorRanges = &descRangeSRV1;					//デスクリプタレンジ
	pSet.rootParams[2].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

	pSet.Create();

	std::string pipeLineName = "PerlinNoisePE";
	sPipelines[pipeLineName] = pSet;
}

void PipelineManager::LinePipeLine()
{
	PipelineSet pipeLineSet;

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pipeLineSet.inputLayout =
	{
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

	//ルートパラメータの設定
	pipeLineSet.paramSize = 5;
	pipeLineSet.rootParams.resize(pipeLineSet.paramSize);
	//定数バッファ0番 b0
	pipeLineSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pipeLineSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pipeLineSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pipeLineSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pipeLineSet.descriptorRange;					//デスクリプタレンジ
	pipeLineSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pipeLineSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pipeLineSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pipeLineSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pipeLineSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pipeLineSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pipeLineSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pipeLineSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	pipeLineSet.pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

	//パイプライン生成
	pipeLineSet.Create();

	std::string pipeLineName = "Line";
	sPipelines[pipeLineName] = pipeLineSet;
}

void PipelineManager::DitherTransparentPipeLine()
{
	PipelineSet pipeLineSet;

	//シェーダー設定
	pipeLineSet.vs.shaderName = "Resources\\Shader\\DTVS.hlsl";
	pipeLineSet.ps.shaderName = "Resources\\Shader\\DTPS.hlsl";

	//pipeLineSet.pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	pipeLineSet.pipelineDesc.NumRenderTargets = 2;	//描画対象は1つ
	pipeLineSet.pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA
	pipeLineSet.pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0～255指定のRGBA

	//3dオブジェクト用のパイプライン生成
	//頂点レイアウト
	pipeLineSet.inputLayout =
	{
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

	//ルートパラメータの設定
	//テクスチャレジスタが3つ必要
	//
	pipeLineSet.paramSize = 7;
	pipeLineSet.rootParams.resize(pipeLineSet.paramSize);
	//定数バッファ0番 b0
	pipeLineSet.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	pipeLineSet.rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番 t0
	pipeLineSet.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pipeLineSet.rootParams[1].DescriptorTable.pDescriptorRanges = &pipeLineSet.descriptorRange;					//デスクリプタレンジ
	pipeLineSet.rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pipeLineSet.rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番 b1
	pipeLineSet.rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	pipeLineSet.rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ2番 b2
	pipeLineSet.rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[3].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	pipeLineSet.rootParams[3].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ3番 b3
	pipeLineSet.rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[4].Descriptor.ShaderRegister = 3;					//定数バッファ番号
	pipeLineSet.rootParams[4].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える
	//定数バッファ4番 b4
	pipeLineSet.rootParams[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	pipeLineSet.rootParams[5].Descriptor.ShaderRegister = 4;					//定数バッファ番号
	pipeLineSet.rootParams[5].Descriptor.RegisterSpace = 0;						//デフォルト値
	pipeLineSet.rootParams[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//すべてのシェーダから見える

	//テクスチャレジスタ1番 t1
	//レンジを使いまわして書き換える
	D3D12_DESCRIPTOR_RANGE dRange2 = pipeLineSet.descriptorRange;
	dRange2.BaseShaderRegister = 1;

	pipeLineSet.rootParams[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	pipeLineSet.rootParams[6].DescriptorTable.pDescriptorRanges = &dRange2;					//デスクリプタレンジ
	pipeLineSet.rootParams[6].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	pipeLineSet.rootParams[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

	//パイプライン生成
	pipeLineSet.Create();

	std::string pipeLineName = "DitherTransparent";
	sPipelines[pipeLineName] = pipeLineSet;
}
