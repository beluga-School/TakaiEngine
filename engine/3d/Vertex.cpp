#include "Vertex.h"
#include "Result.h"

void VertexData::CreateVertex(const std::vector<Vertex> &vertices,const std::vector<uint16_t> &indices)
{
	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	mResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	mResDesc.Width = sizeVB;	//���_�f�[�^�S�̂̃T�C�Y
	mResDesc.Height = 1;
	mResDesc.DepthOrArraySize = 1;
	mResDesc.MipLevels = 1;
	mResDesc.SampleDesc.Count = 1;
	mResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@�̐���
	//ComPtr�ɂ�����_�������� �}�b�v�����Ɏg���Ă邩��H
	result = DirectX12::Get()->mDevice->CreateCommittedResource(
		&heapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&mResDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mVertBuff.GetAddressOf()));
	assert(SUCCEEDED(result));

	//GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	result = mVertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (int32_t i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i];	//	���W���R�s�[
	}
	//�q���������
	mVertBuff->Unmap(0, nullptr);

	//GPU���z�A�h���X
	mVbView.BufferLocation = mVertBuff->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	mVbView.SizeInBytes = sizeVB;
	//���_1���̃f�[�^�T�C�Y
	mVbView.StrideInBytes = sizeof(vertices[0]);

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	//ComPtr�ɂ�����_�������� �}�b�v�����Ɏg���Ă邩��H
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

	//���\�[�X�ݒ�
	mResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	mResDesc.Width = sizeIB;	//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	mResDesc.Height = 1;
	mResDesc.DepthOrArraySize = 1;
	mResDesc.MipLevels = 1;
	mResDesc.SampleDesc.Count = 1;
	mResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�C���f�b�N�X�o�b�t�@�̐���
	result = DirectX12::Get()->mDevice->CreateCommittedResource(
		&heapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&mResDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mIndexBuff.GetAddressOf()));

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = mIndexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int32_t i = 0; i < indices.size(); i++) {
		indexMap[i] = indices[i];	//�C���f�b�N�X���R�s�[
	}
	//�}�b�s���O����
	mIndexBuff->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	mIbView.BufferLocation = mIndexBuff->GetGPUVirtualAddress();
	mIbView.Format = DXGI_FORMAT_R16_UINT;
	mIbView.SizeInBytes = sizeIB;
}