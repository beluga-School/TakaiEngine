#pragma once
#include "Vector3.h"
#include "Vector2.h"

#include "DirectXInit.h"
#include <vector>
#include "ConstBuffer.h"

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

#include "Pipeline.h"
#include "Texture.h"

//�e�N�X�`���̍ő喇��(���͎��ƂŃe�N�X�`���̕��ƍ��킹�Ă�)
const int spriteSRVCount = 2056;

struct VertexPosUV
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

struct Sprite
{
	//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�萔�o�b�t�@
	ConstBuffer<ConstBufferDataSprite> constBuffer;

	//Z�����̉�]�p
	float rotation = 0.0f;
	//���W
	XMFLOAT3 position = { 0,0,0 };
	//���[���h�s��
	XMMATRIX matWorld;
	//�F
	XMFLOAT4 color = { 1,1,1,1 };

	Texture *tex;

	XMFLOAT2 size;

	XMFLOAT2 anchorpoint = { 0.5f,0.5f };

	bool isFlipX = false;
	bool isFlipY = false;

	XMFLOAT2 texLeftTop = {0,0};
	XMFLOAT2 cutSize;

	bool isInvisible = false;

};

struct SpriteCommon
{
	PipelineSet pipelineSet;
	//�ˉe�s��
	XMMATRIX matProjection{};

	static void Initialize();

	static SpriteCommon spriteCommon;
};

//�X�v���C�g�̐���
void SpriteCreate(Sprite *sprite,Texture* tex, XMFLOAT2 anchorpoint,bool isFlipX = false, bool isFlipY = false);

//�X�v���C�g�̕ϐ��̏�����
void SpriteInit(Sprite& sprite, SpriteCommon& spriteCommon,XMFLOAT2 pos = {0,0}, float rotation = 0.0f, XMFLOAT4 color = {1,1,1,1});

//�X�v���C�g���ʃO���t�B�b�N�R�}���h�̃Z�b�g
void SpriteCommonBeginDraw(const SpriteCommon& spriteCommon);

void SpriteUpdate(Sprite& sprite,const SpriteCommon& spriteCommon);

void SpriteTransferVertexBuffer(const Sprite& sprite);

void SpriteSetSize(Sprite& sprite, XMFLOAT2 size);

void SpriteDraw(const Sprite& sprite);

SpriteCommon SpriteCommonCreate();
