#include "GeometryObject.h"
#include "DirectXInit.h"

void GeometryObject::CreateModel()
{
	DirectX12 *dx12 = DirectX12::GetInstance();

	verticesPoint[vertexCount] = {
		{0.0f,0.0f,0.0f}
	};

	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	//UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//���_�f�[�^�S�̂̃T�C�Y
	resDesc.Width = sizeof(verticesPoint);
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@�̐���
	//ComPtr�ɂ�����_�������� �}�b�v�����Ɏg���Ă邩��H
	ID3D12Resource* vertBuff;
	result = dx12->device->CreateCommittedResource(
		&heapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (int i = 0; i < vertexCount; i++) {
		vertMap[i] = verticesPoint[i];	//	���W���R�s�[
	}
	//�q���������
	vertBuff->Unmap(0, nullptr);

	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeof(verticesPoint);
	//���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(verticesPoint[0]);
}

void GeometryObject::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	constBufferGeometry.constBufferData->mat = matView * matProjection;
}

void GeometryObject::Draw()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	//���_�o�b�t�@�̐ݒ�
	dx12->commandList->IASetVertexBuffers(0, 1, &vbView);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	//dx12->commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial.buffer->GetGPUVirtualAddress());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferM.buffer->GetGPUVirtualAddress());

	dx12->commandList->SetGraphicsRootConstantBufferView(0, constBufferGeometry.buffer->GetGPUVirtualAddress());

	//�`��R�}���h
	dx12->commandList->DrawInstanced(_countof(verticesPoint), 1, 0, 0);
}
