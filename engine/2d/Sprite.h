#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4.h"
#include "Color.h"

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

#include "Pipeline.h"
#include "Texture.h"

//�e�N�X�`���̍ő喇��(���͎��ƂŃe�N�X�`���̕��ƍ��킹�Ă�)
const int32_t spriteSRVCount = 2056;

struct VertexPosUV
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

struct Sprite
{
	//�Ƃ肠�����e�N�X�`���[�����ăX�v���C�g�����
	//�ȑO�ݒ肵�Ă��t���b�v�Ƃ��͕ʂŐݒ肷��
	Sprite();
	Sprite(const Texture& tex,const Vector2& anchorpoint = {0.5f,0.5f});

	void SetTexture(const Texture& tex);
	void SetAnchor(const Vector2& anchorpoint);

	void SetPos(const Vector2& pos);
	void SetRotation(const float& rotation);
	void SetColor(const Color& color);

	void SetSize(const Vector2& size);

	void Update();
	void Draw();

	//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�萔�o�b�t�@
	ConstBuffer<ConstBufferDataSprite> constBuffer;

	//Z�����̉�]�p
	float rotation = 0.0f;
	//���W
	Vector3 position = { 0,0,0 };
	//���[���h�s��
	XMMATRIX matWorld;
	//�F
	Color color = { 1,1,1,1 };

	const Texture *tex;

	Vector2 size;

	Vector2 anchorpoint = { 0.5f,0.5f };

	bool isFlipX = false;
	bool isFlipY = false;

	Vector2 texLeftTop = {0,0};
	Vector2 cutSize;

	bool isInvisible = false;

private:
	void Init();
};

struct SpriteCommon
{
	PipelineSet pipelineSet;
	//�ˉe�s��
	XMMATRIX matProjection{};

	static void Initialize();

	static SpriteCommon spriteCommon;
};

//�X�v���C�g���ʃO���t�B�b�N�R�}���h�̃Z�b�g
void SpriteCommonBeginDraw(const SpriteCommon& spriteCommon);

void SpriteTransferVertexBuffer(const Sprite& sprite);

SpriteCommon SpriteCommonCreate();
