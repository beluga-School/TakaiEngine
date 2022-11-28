#include "Sprite.h"
#include "Result.h"
#include "Util.h"

SpriteCommon SpriteCommon::spriteCommon;

void SpriteCommon::Initialize()
{
	spriteCommon = SpriteCommonCreate();
}

void SpriteCreate(Sprite* sprite, Texture* tex,XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	DirectX12* dx12 = DirectX12::GetInstance();

	result = S_FALSE;

	//�V�K�X�v���C�g�𐶐�
	//Sprite sprite{};

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
	result = dx12->device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(sprite->vertBuff.GetAddressOf()));

	//���_�o�b�t�@�ւ̃f�[�^�]��
	/*VertexPosUV* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);*/

	sprite->vbView.BufferLocation = sprite->vertBuff->GetGPUVirtualAddress();
	sprite->vbView.SizeInBytes = sizeof(vertices);
	sprite->vbView.StrideInBytes = sizeof(vertices[0]);

	//�萔�o�b�t�@�̐ݒ�
	//���s���e�s��
	sprite->constBuffer.constBufferData->mat =
		XMMatrixOrthographicOffCenterLH(
		0.0f, Util::window_width, Util::window_height, 0.0f, 0.0f, 1.0f);

	//�F�w��
	sprite->constBuffer.constBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);

	sprite->tex = tex;

	sprite->size = { (float)tex->getResDesc.Width,(float)tex->getResDesc.Height };

	sprite->cutSize = sprite->size;

	sprite->anchorpoint = anchorpoint;

	sprite->isFlipX = isFlipX;
	sprite->isFlipY = isFlipY;

	SpriteTransferVertexBuffer(*sprite);
}

void SpriteInit(Sprite& sprite, SpriteCommon& spriteCommon, XMFLOAT2 pos, float rotation, XMFLOAT4 color)
{
	sprite.position.x = pos.x;
	sprite.position.y = pos.y;
	sprite.position.z = 0;
	sprite.rotation = rotation;
	sprite.color = color;

	SpriteUpdate(sprite, spriteCommon);
}

void SpriteUpdate(Sprite& sprite, const SpriteCommon& spriteCommon)
{
	//���[���h�s��̍X�V
	sprite.matWorld = XMMatrixIdentity();
	//Z����]
	sprite.matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite.rotation));
	//���s�ړ�
	sprite.matWorld *= XMMatrixTranslation(sprite.position.x, sprite.position.y, sprite.position.z);

	//�萔�o�b�t�@�̓]��
	result = sprite.constBuffer.buffer->Map(0, nullptr, (void**)&sprite.constBuffer.constBufferData);
	sprite.constBuffer.constBufferData->mat = sprite.matWorld * spriteCommon.matProjection;
	sprite.constBuffer.constBufferData->color = sprite.color;
	sprite.constBuffer.buffer->Unmap(0, nullptr);
}

void SpriteTransferVertexBuffer(const Sprite& sprite)
{
	result = S_FALSE;

	VertexPosUV vertices[] = {
		{{},{0.0f,1.0f}},
		{{},{0.0f,0.0f}},
		{{},{1.0f,1.0f}},
		{{},{1.0f,0.0f}},
	};

	//�����A����A�E���A�E��
	enum {
		LB,LT,RB,RT
	};

	vertices[LB].pos = { 0.0f,	sprite.size.y,	0.0f };
	vertices[LT].pos = { 0.0f,	0.0f,	0.0f };
	vertices[RB].pos = { sprite.size.x,	sprite.size.y,	0.0f };
	vertices[RT].pos = { sprite.size.x,	0.0f,	0.0f };

	float left =	(0.0f - sprite.anchorpoint.x) * sprite.size.x;
	float right =	(1.0f - sprite.anchorpoint.x) * sprite.size.x;
	float top =		(0.0f - sprite.anchorpoint.y) * sprite.size.y;
	float bottom =	(1.0f - sprite.anchorpoint.y) * sprite.size.y;

	if (sprite.isFlipX)
	{
		left = -left;
		right = -right;
	}

	if (sprite.isFlipY)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,	bottom,	0.0f };
	vertices[LT].pos = { left,	top,	0.0f };
	vertices[RB].pos = { right,	bottom,	0.0f };
	vertices[RT].pos = { right,	top,	0.0f };

	if (sprite.tex->texBuff)
	{
		D3D12_RESOURCE_DESC resDesc = sprite.tex->getResDesc;

		float tex_left = sprite.texLeftTop.x / resDesc.Width;
		float tex_right = (sprite.texLeftTop.x + sprite.cutSize.x) / resDesc.Width;
		float tex_top = sprite.texLeftTop.y / resDesc.Height;
		float tex_bottom = (sprite.texLeftTop.y + sprite.cutSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,tex_bottom };
		vertices[LT].uv = { tex_left,tex_top };
		vertices[RB].uv = { tex_right,tex_bottom };
		vertices[RT].uv = { tex_right,tex_top };
	}

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUV* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);
}

void SpriteCommonBeginDraw(const SpriteCommon& spriteCommon)
{
	DirectX12* dx12 = DirectX12::GetInstance();
	TextureManager* texM = TextureManager::GetInstance();

	//�p�C�v���C���X�e�[�g�̐ݒ�
	dx12->commandList->SetPipelineState(spriteCommon.pipelineSet.pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	dx12->commandList->SetGraphicsRootSignature(spriteCommon.pipelineSet.rootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	dx12->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	dx12->commandList->SetDescriptorHeaps(1, texM->srvHeap.GetAddressOf());
}

void SpriteDraw(const Sprite& sprite)
{
	DirectX12* dx12 = DirectX12::GetInstance();
	TextureManager* texM = TextureManager::GetInstance();

	if (sprite.isInvisible)
	{
		return;
	}

	dx12->commandList->IASetVertexBuffers(0, 1, &sprite.vbView);

	dx12->commandList->SetGraphicsRootDescriptorTable(1, sprite.tex->gpuHandle);

	dx12->commandList->SetGraphicsRootConstantBufferView(0, sprite.constBuffer.buffer->GetGPUVirtualAddress());

	dx12->commandList->DrawInstanced(4, 1, 0, 0);
}

void SpriteSetSize(Sprite& sprite, XMFLOAT2 size)
{
	sprite.size = size;
	SpriteTransferVertexBuffer(sprite);
}

SpriteCommon SpriteCommonCreate()
{
	result = S_FALSE;

	//�X�v���C�g�̋��ʃf�[�^���쐬
	SpriteCommon spriteCommon{};

	//�X�v���C�g�p�̃p�C�v���C���𐶐�
	spriteCommon.pipelineSet = CreateSpritePipeline();

	//���s���e�̎ˉe�s�񐶐�
	spriteCommon.matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)Util::window_width, (float)Util::window_height, 0.0f, 0.0f, 1.0f);

	return spriteCommon;
}
