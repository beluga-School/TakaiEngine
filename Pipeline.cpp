#include "Pipeline.h"
#include "Shader.h"
#include "Result.h"
#include <cassert>

PipelineSet CreateObject3DPipeline()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	//�p�C�v���C���\����
	PipelineSet pipelineSet;

	//�V�F�[�_�[
	Shader shader_;

	shader_.vsBlob = shader_.Compile(L"Resources\\Shader\\OBJVS.hlsl", "vs_5_0", shader_.vsBlob.Get(), "main");
	shader_.psBlob = shader_.Compile(L"Resources\\Shader\\OBJPS.hlsl", "ps_5_0", shader_.psBlob.Get(), "main");

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//�V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = shader_.vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = shader_.vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = shader_.psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = shader_.psBlob->GetBufferSize();

	//�T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//�W���ݒ�

	//���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	//�J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//�|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true;			//�[�x�N���b�s���O��L����

	//�u�����h�X�e�[�g
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA�S�Ẵ`�����l����`��

	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //RGBA���ׂẴ`�����l����`��

	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���

	//����ӂ��`�`�`�`
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ZERO;		//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��

	int blendMode = ALPHA;

	if (blendMode == ADD)
	{
		//���Z����
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
		blenddesc.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��
	}
	if (blendMode == SUB)
	{
		//���Z����
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//�f�X�g����\�[�X�����Z
		blenddesc.SrcBlend = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
		blenddesc.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��
	}
	if (blendMode == INV)
	{
		//�F���]
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
		blenddesc.DestBlend = D3D12_BLEND_ZERO;			//�g��Ȃ�
	}
	if (blendMode == ALPHA)
	{
		//����������
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;		//�\�[�X�̃A���t�@�l
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l
	}

	//���_���C�A�E�g
	static D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ //xyz���W
				"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
			{//�@���x�N�g��
				"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
			{ //uv���W
				"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
	};

	//���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1;	//�`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0�`255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1;	//1�s�N�Z���ɂ�1��T���v�����O

	pipelineDesc.DepthStencilState.DepthEnable = true;	//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	//��������΋���
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;	//��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[4] = {};
	//�萔�o�b�t�@0�� b0
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	//�e�N�X�`�����W�X�^0�� t0
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;					//�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	//�萔�o�b�t�@1�� b1
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams[2].Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//���ׂẴV�F�[�_���猩����
	//�萔�o�b�t�@2�� b2
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams[3].Descriptor.ShaderRegister = 2;					//�萔�o�b�t�@�ԍ�
	rootParams[3].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//���ׂẴV�F�[�_���猩����

	//�e�N�X�`���T���v���[�̐��� s0
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

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;				//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);	//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;		//�T���v���[�̐擪�A�h���X	�z��Ȃ烋�[�g�p�����[�^�Ɠ����悤�ȏ��������ł���
	rootSignatureDesc.NumStaticSamplers = 1;				//�T���v���[�̐�			�z��Ȃ烋�[�g�p�����[�^�Ɠ����悤�ȏ��������ł���

	//���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader_.errorBlob);
	assert(SUCCEEDED(result));
	result = dx12->device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootsignature));
	assert(SUCCEEDED(result));

	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = pipelineSet.rootsignature.Get();

	//�p�C�v���C���X�e�[�g�̐���
	//ComPtr<ID3D12PipelineState> pipelineState;
	result = dx12->device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineSet.pipelinestate));
	assert(SUCCEEDED(result));

	return pipelineSet;
}

PipelineSet CreateSpritePipeline()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	PipelineSet spritePipelineSet;

	//�V�F�[�_�[
	Shader shader2_;

	shader2_.vsBlob = shader2_.Compile(L"Resources\\Shader\\SpriteVS.hlsl", "vs_5_0", shader2_.vsBlob.Get(), "main");
	shader2_.psBlob = shader2_.Compile(L"Resources\\Shader\\SpritePS.hlsl", "ps_5_0", shader2_.psBlob.Get(), "main");

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc2{};

	//�V�F�[�_�[�̐ݒ�
	pipelineDesc2.VS.pShaderBytecode = shader2_.vsBlob->GetBufferPointer();
	pipelineDesc2.VS.BytecodeLength = shader2_.vsBlob->GetBufferSize();
	pipelineDesc2.PS.pShaderBytecode = shader2_.psBlob->GetBufferPointer();
	pipelineDesc2.PS.BytecodeLength = shader2_.psBlob->GetBufferSize();

	//�T���v���}�X�N�̐ݒ�
	pipelineDesc2.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//�W���ݒ�

	//���X�^���C�U�̐ݒ�
	pipelineDesc2.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	//�w�ʃJ�����O�����Ȃ�
	pipelineDesc2.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//�|���S�����h��Ԃ�
	pipelineDesc2.RasterizerState.DepthClipEnable = true;			//�[�x�N���b�s���O��L����

	//�u�����h�X�e�[�g
	pipelineDesc2.BlendState.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA�S�Ẵ`�����l����`��

	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc2 = pipelineDesc2.BlendState.RenderTarget[0];
	blenddesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //RGBA���ׂẴ`�����l����`��

	blenddesc2.BlendEnable = true;					//�u�����h��L���ɂ���

	//����ӂ��`�`�`�`
	blenddesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc2.SrcBlendAlpha = D3D12_BLEND_ZERO;		//�\�[�X�̒l��100%�g��
	blenddesc2.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��


	int blendMode2 = ALPHA;

	if (blendMode2 == ADD)
	{
		//���Z����
		blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc2.SrcBlend = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
		blenddesc2.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��
	}
	if (blendMode2 == SUB)
	{
		//���Z����
		blenddesc2.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//�f�X�g����\�[�X�����Z
		blenddesc2.SrcBlend = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
		blenddesc2.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��
	}
	if (blendMode2 == INV)
	{
		//�F���]
		blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc2.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
		blenddesc2.DestBlend = D3D12_BLEND_ZERO;			//�g��Ȃ�
	}
	if (blendMode2 == ALPHA)
	{
		//����������
		blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;		//�\�[�X�̃A���t�@�l
		blenddesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l
	}

	//���_���C�A�E�g
	static D3D12_INPUT_ELEMENT_DESC inputLayout2[] = {
			{ //xyz���W
				"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
			{ //uv���W
				"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
	};

	//���_���C�A�E�g�̐ݒ�
	pipelineDesc2.InputLayout.pInputElementDescs = inputLayout2;
	pipelineDesc2.InputLayout.NumElements = _countof(inputLayout2);

	//�}�`�̌`��ݒ�
	pipelineDesc2.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	pipelineDesc2.NumRenderTargets = 1;	//�`��Ώۂ�1��
	pipelineDesc2.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0�`255�w���RGBA
	pipelineDesc2.SampleDesc.Count = 1;	//1�s�N�Z���ɂ�1��T���v�����O

	//�[�x�X�e�[�g�̐ݒ�
	pipelineDesc2.DepthStencilState.DepthEnable = false;	//�[�x�e�X�g���s��Ȃ�
	pipelineDesc2.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;	//��ɏ㏑��
	
	//����Ȃ������������
	pipelineDesc2.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���
	pipelineDesc2.DSVFormat = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange2{};
	descriptorRange2.NumDescriptors = 1;	//��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 0;	//�e�N�X�`�����W�X�^0��
	descriptorRange2.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams2[3] = {};
	//�萔�o�b�t�@0�� b0
	rootParams2[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams2[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams2[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams2[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	//�e�N�X�`�����W�X�^0�� t0
	rootParams2[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootParams2[1].DescriptorTable.pDescriptorRanges = &descriptorRange2;					//�f�X�N���v�^�����W
	rootParams2[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParams2[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����

	//�e�N�X�`���T���v���[�̐��� s0
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

	//���[�g�V�O�l�`��
	//ComPtr<ID3D12RootSignature> rootSignature;

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc2{};
	rootSignatureDesc2.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc2.pParameters = rootParams2;				//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc2.NumParameters = _countof(rootParams2);	//���[�g�p�����[�^��
	rootSignatureDesc2.pStaticSamplers = &samplerDesc2;		//�T���v���[�̐擪�A�h���X	�z��Ȃ烋�[�g�p�����[�^�Ɠ����悤�ȏ��������ł���
	rootSignatureDesc2.NumStaticSamplers = 1;				//�T���v���[�̐�			�z��Ȃ烋�[�g�p�����[�^�Ɠ����悤�ȏ��������ł���

	//���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob2;
	result = D3D12SerializeRootSignature(&rootSignatureDesc2, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob2, &shader2_.errorBlob);
	assert(SUCCEEDED(result));
	result = dx12->device->CreateRootSignature(0, rootSigBlob2->GetBufferPointer(), rootSigBlob2->GetBufferSize(),
		IID_PPV_ARGS(&spritePipelineSet.rootsignature));
	assert(SUCCEEDED(result));

	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc2.pRootSignature = spritePipelineSet.rootsignature.Get();

	//�p�C�v���C���X�e�[�g�̐���
	//ComPtr<ID3D12PipelineState> pipelineState;
	result = dx12->device->CreateGraphicsPipelineState(&pipelineDesc2, IID_PPV_ARGS(&spritePipelineSet.pipelinestate));
	assert(SUCCEEDED(result));

	return spritePipelineSet;
}

PipelineSet CreateGeometryPipeline()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	//�p�C�v���C���\����
	PipelineSet pipelineSet;

	//�V�F�[�_�[
	Shader shader_;

	shader_.vsBlob = shader_.Compile(L"Resources\\Shader\\ParticleVS.hlsl", "vs_5_0", shader_.vsBlob.Get(), "main");
	shader_.psBlob = shader_.Compile(L"Resources\\Shader\\ParticlePS.hlsl", "ps_5_0", shader_.psBlob.Get(), "main");
	shader_.gsBlob = shader_.Compile(L"Resources\\Shader\\ParticleGS.hlsl", "gs_5_0", shader_.gsBlob.Get(), "main");

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//�V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = shader_.vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = shader_.vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = shader_.psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = shader_.psBlob->GetBufferSize();
	pipelineDesc.GS.pShaderBytecode = shader_.gsBlob->GetBufferPointer();
	pipelineDesc.GS.BytecodeLength = shader_.gsBlob->GetBufferSize();

	//�T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//�W���ݒ�

	//���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	//�J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//�|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true;			//�[�x�N���b�s���O��L����

	//�u�����h�X�e�[�g
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA�S�Ẵ`�����l����`��

	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //RGBA���ׂẴ`�����l����`��

	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���

	//����ӂ��`�`�`�`
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ZERO;		//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��

	int blendMode = ADD;

	if (blendMode == ADD)
	{
		//���Z����
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
		blenddesc.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��
	}
	if (blendMode == SUB)
	{
		//���Z����
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//�f�X�g����\�[�X�����Z
		blenddesc.SrcBlend = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
		blenddesc.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��
	}
	if (blendMode == INV)
	{
		//�F���]
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
		blenddesc.DestBlend = D3D12_BLEND_ZERO;			//�g��Ȃ�
	}
	if (blendMode == ALPHA)
	{
		//����������
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;		//�\�[�X�̃A���t�@�l
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l
	}

	//���_���C�A�E�g
	static D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ //xyz���W
				"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
			{ //�X�P�[���̏��
				"SCALE",0,DXGI_FORMAT_R32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
			{ //�F�̏��
				"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
	};

	//���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	//���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1;	//�`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0�`255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1;	//1�s�N�Z���ɂ�1��T���v�����O

	pipelineDesc.DepthStencilState.DepthEnable = true;	//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;//�������ݕs��
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	//��������΋���
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;	//��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[2] = {};
	//�萔�o�b�t�@0�� b0
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	//�e�N�X�`�����W�X�^0�� t0
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;					//�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	

	//�e�N�X�`���T���v���[�̐��� s0
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

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;				//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);	//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;		//�T���v���[�̐擪�A�h���X	�z��Ȃ烋�[�g�p�����[�^�Ɠ����悤�ȏ��������ł���
	rootSignatureDesc.NumStaticSamplers = 1;				//�T���v���[�̐�			�z��Ȃ烋�[�g�p�����[�^�Ɠ����悤�ȏ��������ł���

	//���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader_.errorBlob);
	assert(SUCCEEDED(result));
	result = dx12->device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootsignature));
	assert(SUCCEEDED(result));

	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = pipelineSet.rootsignature.Get();

	//�p�C�v���C���X�e�[�g�̐���
	//ComPtr<ID3D12PipelineState> pipelineState;
	result = dx12->device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineSet.pipelinestate));
	assert(SUCCEEDED(result));

	return pipelineSet;
}
