#include "Sprite.h"
#include "Result.h"

Sprite SpriteCreate(DirectX12 dx12)
{
	result = S_FALSE;

	//�V�K�X�v���C�g�𐶐�
	Sprite sprite{};

	//���_�f�[�^
	VertexPosUV vertices[] = {
		//x      y      z        u    v
		{{  0.0f,100.0f,  0.0f},{0.0f,1.0f}},//����
		{{  0.0f,  0.0f,  0.0f},{0.0f,0.0f}},//����
		{{100.0f,100.0f,  0.0f},{1.0f,1.0f}},//�E��
		{{100.0f,  0.0f,  0.0f},{1.0f,0.0f}},//�E��
	};

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

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
	result = dx12.device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&sprite.vertBuff));

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUV* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);

	sprite.vbView.BufferLocation = sprite.vertBuff->GetGPUVirtualAddress();
	sprite.vbView.SizeInBytes = sizeof(vertices);
	sprite.vbView.StrideInBytes = sizeof(vertices[0]);

	//�萔�o�b�t�@�̐ݒ�
	//���s���e�s��
	sprite.constBufferT.constBufferData->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);

	//�F�w��
	sprite.constBufferM.constBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);

	return sprite;
}

void SpriteCommonBeginDraw(DirectX12 dx12, const PipelineSet& pipelineSet, Texture &tex)
{
	//�p�C�v���C���X�e�[�g�̐ݒ�
	dx12.commandList->SetPipelineState(pipelineSet.pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	dx12.commandList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	dx12.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//dx12.commandList->SetDescriptorHeaps(1, &tex.srvHeap);
}

void SpriteDraw(const Sprite& sprite, DirectX12 dx12, Texture& tex)
{
	dx12.commandList->IASetVertexBuffers(0, 1, &sprite.vbView);

	dx12.commandList->SetDescriptorHeaps(1, &tex.srvHeap);
	dx12.commandList->SetGraphicsRootDescriptorTable(1, tex.GetHandle());

	dx12.commandList->SetGraphicsRootConstantBufferView(0, sprite.constBufferM.buffer->GetGPUVirtualAddress());
	dx12.commandList->SetGraphicsRootConstantBufferView(2, sprite.constBufferT.buffer->GetGPUVirtualAddress());

	dx12.commandList->DrawInstanced(4, 1, 0, 0);
}
