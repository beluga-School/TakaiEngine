#include "PostEffect.h"
#include "util.h"
#include "Result.h"
#include <memory>
#include "ClearDrawScreen.h"

const float PostEffect::sClearColor[4] = { 0.25f,0.5f,0.1f,1.0f };//�΂��ۂ��F�ŃN���A

PostEffect::PostEffect()
{

}

void PostEffect::Initialize()
{
	DirectX12* dx12 = DirectX12::Get();

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	VertexPosUV vertices[] = {
		//x      y      z        u    v
		{{ -0.5f, -0.5f,  0.0f},{0.0f,1.0f}},//����
		{{ -0.5f,  0.5f,  0.0f},{0.0f,0.0f}},//����
		{{  0.5f, -0.5f,  0.0f},{1.0f,1.0f}},//�E��
		{{  0.5f,  0.5f,  0.0f},{1.0f,0.0f}},//�E��
	};

	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//�����������������
	resDesc.Width = sizeof(vertices);	//���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@����
	sResult = dx12->mDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mVertBuff.GetAddressOf()));

	VertexPosUV* vertMap = nullptr;
	sResult = mVertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	mVertBuff->Unmap(0, nullptr);

	mVbView.BufferLocation = mVertBuff->GetGPUVirtualAddress();
	mVbView.SizeInBytes = sizeof(vertices);
	mVbView.StrideInBytes = sizeof(vertices[0]);

	//--�萔�o�b�t�@��ConstBuffer�̃R���X�g���N�^�ō쐬�����



	CreateTexture();
	
	CreateSRV();

	CreateRTV();

	CreateDepthBuff();

	CreateDSV();	
}

void PostEffect::Draw()
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	//�萔�o�b�t�@�̓]��
	sResult = mConstBuffer.mBuffer->Map(0, nullptr, (void**)&mConstBuffer.mConstBufferData);
	mConstBuffer.mConstBufferData->mat = XMMatrixIdentity();

	mConstBuffer.mBuffer->Unmap(0, nullptr);

	//�p�C�v���C�������������Ă���
	PipelineSet pSet = PipelineManager::GetPipeLine("ShiftBlur");
	//�p�C�v���C���X�e�[�g�̐ݒ�
	dx12->mCmdList->SetPipelineState(pSet.mPipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	dx12->mCmdList->SetGraphicsRootSignature(pSet.mRootsignature.Get());
	
	
	//�v���~�e�B�u�`���ݒ�
	dx12->mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//GetAddressOf���Ȃ��ƃo�O��܂� ���R�͖Y�ꂿ����� ComPtr�����炾�����H
	dx12->mCmdList->SetDescriptorHeaps(1, mDescHeapSRV.GetAddressOf());

	dx12->mCmdList->IASetVertexBuffers(0, 1, &mVbView);

	dx12->mCmdList->SetGraphicsRootDescriptorTable(1, mDescHeapSRV->GetGPUDescriptorHandleForHeapStart());

	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, mConstBuffer.mBuffer->GetGPUVirtualAddress());

	dx12->mCmdList->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::PreDrawScene()
{
	DirectX12* dx12 = DirectX12::Get();

	D3D12_RESOURCE_BARRIER barrierDesc{};

	//�e�N�X�`�����A��ʑS�̂�`�悷��Ƃ��Ɠ����悤�ɏ������ވׂ̏���
	barrierDesc.Transition.pResource = mTexBuff.Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;		//�\����Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;	//�`���Ԃ�
	dx12->mCmdList->ResourceBarrier(1, &barrierDesc);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		mDescHeapRTV->GetCPUDescriptorHandleForHeapStart();

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		mDescHeapDSV->GetCPUDescriptorHandleForHeapStart();

	dx12->mCmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	D3D12_VIEWPORT viewport{};
	viewport.Width = Util::WIN_WIDTH;
	viewport.Height = Util::WIN_HEIGHT;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	dx12->mCmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorRect{};
	scissorRect.left = 0;									
	scissorRect.right = scissorRect.left + Util::WIN_WIDTH;	
	scissorRect.top = 0;									
	scissorRect.bottom = scissorRect.top + Util::WIN_HEIGHT;
	dx12->mCmdList->RSSetScissorRects(1, &scissorRect);

	//�S��ʃN���A
	dx12->mCmdList->ClearRenderTargetView(rtvH, sClearColor, 0, nullptr);

	//�[�x�o�b�t�@�̃N���A
	dx12->mCmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene()
{
	DirectX12* dx12 = DirectX12::Get();
	Screen* screen = Screen::Get();

	//���\�[�X�o���A�̏�Ԃ�߂�
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = mTexBuff.Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	dx12->mCmdList->ResourceBarrier(1, &barrierDesc);

	//�����_�[�^�[�Q�b�g��߂�
	UINT bbIndex = dx12->mSwapChain->GetCurrentBackBufferIndex();

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx12->mRtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * dx12->mDevice->GetDescriptorHandleIncrementSize(dx12->mRtvHeapDesc.Type);

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = screen->dsvHeap->GetCPUDescriptorHandleForHeapStart();
	dx12->mCmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

}

void PostEffect::CreateTexture()
{
	DirectX12* dx12 = DirectX12::Get();

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureResourceDesc.Width = Util::WIN_WIDTH;
	textureResourceDesc.Height = (UINT)Util::WIN_HEIGHT;
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;
	textureResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_CLEAR_VALUE texClearValue{};
	for (int32_t i = 0; i < 4; i++)
	{
		texClearValue.Color[i] = sClearColor[i];
	}
	texClearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//�e�N�X�`���o�b�t�@�̐���
	sResult = dx12->mDevice->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&texClearValue,
		IID_PPV_ARGS(&mTexBuff));
	assert(SUCCEEDED(sResult));

	{//�e�N�X�`����ԂŃN���A

		//�c���̃s�N�Z�����̉�f��
		const UINT pixelCount = Util::WIN_WIDTH * Util::WIN_HEIGHT;

		const UINT rowPitch = sizeof(UINT) * Util::WIN_WIDTH;

		const UINT depthPitch = rowPitch * Util::WIN_HEIGHT;

		UINT* img = new UINT[pixelCount];
		for (int32_t i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;
		}

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		sResult = mTexBuff->WriteToSubresource(
			0, nullptr,
			img, rowPitch, depthPitch
		);
		assert(SUCCEEDED(sResult));
		delete[] img;
	}
}

void PostEffect::CreateSRV()
{
	DirectX12* dx12 = DirectX12::Get();

	///--�V����SRV�p�f�X�N���v�^�q�[�v���쐬
	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeaoDesc = {};

	//�f�X�N���v�^�q�[�v�̐ݒ�
	srvDescHeaoDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeaoDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_�[���猩����悤��
	srvDescHeaoDesc.NumDescriptors = 1; //�����_�[�e�N�X�`����p�̃q�[�v�Ȃ��߁A�T�C�Y��1�ł���

	//�ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	sResult = DirectX12::Get()->mDevice->CreateDescriptorHeap(&srvDescHeaoDesc, IID_PPV_ARGS(&mDescHeapSRV));
	assert(SUCCEEDED(sResult));

	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC mSrvDesc{};
	mSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	mSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	mSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	mSrvDesc.Texture2D.MipLevels = 1;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	DirectX12::Get()->mDevice->CreateShaderResourceView(mTexBuff.Get(), &mSrvDesc, mDescHeapSRV->GetCPUDescriptorHandleForHeapStart());

}

void PostEffect::CreateRTV()
{
	DirectX12* dx12 = DirectX12::Get();

	///--�����_�[�e�N�X�`�����쐬
	//��ʕ`��̂��߂̃o�b�N�o�b�t�@�Ɠ���

	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	sResult = dx12->mDevice->CreateDescriptorHeap(
		&rtvDescHeapDesc,
		IID_PPV_ARGS(&mDescHeapRTV));
	assert(SUCCEEDED(sResult));

	//�����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	//�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//�f�X�N���v�^�q�[�v��RTV�쐬
	dx12->mDevice->CreateRenderTargetView(
		mTexBuff.Get(),
		&renderTargetViewDesc,
		mDescHeapRTV->GetCPUDescriptorHandleForHeapStart()
	);
}

void PostEffect::CreateDepthBuff()
{
	DirectX12* dx12 = DirectX12::Get();

	//�[�x�o�b�t�@���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = Util::WIN_WIDTH;	//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = Util::WIN_HEIGHT;	//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	//�[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g

	//���\�[�X����
	sResult = dx12->mDevice->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&mDepthBuff));
	assert(SUCCEEDED(sResult));
}

void PostEffect::CreateDSV()
{
	DirectX12* dx12 = DirectX12::Get();

	//DSV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descHeapDesc.NumDescriptors = 1;

	//DSV�p�f�X�N���v�^�q�[�v���쐬
	sResult = dx12->mDevice->CreateDescriptorHeap(
		&descHeapDesc,
		IID_PPV_ARGS(&mDescHeapDSV)
	);

	assert(SUCCEEDED(sResult));

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dx12->mDevice->CreateDepthStencilView(
		mDepthBuff.Get(),
		&dsvDesc,
		mDescHeapDSV->GetCPUDescriptorHandleForHeapStart());
}
