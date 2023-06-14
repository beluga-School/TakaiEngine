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
	Microsoft::WRL::ComPtr<ID3D12Resource> mVertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW mVbView{};
	//�萔�o�b�t�@
	ConstBuffer<ConstBufferDataSprite> mConstBuffer;

	//Z�����̉�]�p
	float mRotation = 0.0f;
	//���W
	Vector3 mPosition = { 0,0,0 };
	//���[���h�s��
	XMMATRIX mMatWorld;
	//�F
	Color mColor = { 1,1,1,1 };

	const Texture *mTexture;

	Vector2 mSize;

	Vector2 mAnchorpoint = { 0.5f,0.5f };

	bool mIsFlipX = false;
	bool mIsFlipY = false;

	Vector2 mTexLeftTop = {0,0};
	Vector2 mCutSize;

	bool mIsInvisible = false;

private:
	void Init();
};

struct SpriteCommon
{
	PipelineSet mPipelineSet;
	//�ˉe�s��
	XMMATRIX mMatProjection{};

	static void Initialize();

	static SpriteCommon mSpriteCommon;
};

//�X�v���C�g���ʃO���t�B�b�N�R�}���h�̃Z�b�g
void SpriteCommonBeginDraw(const SpriteCommon& spriteCommon);

void SpriteTransferVertexBuffer(const Sprite& sprite);

SpriteCommon SpriteCommonCreate();
