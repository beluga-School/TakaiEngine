#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4.h"
#include "Color.h"
#include "DirectXInit.h"
#include <vector>
#include "ConstBuffer.h"
#include <Windows.h>
#include <DirectXMath.h>
#include "Pipeline.h"
#include "Texture.h"

//�e�N�X�`���̍ő喇��(���͎��ƂŃe�N�X�`���̕��ƍ��킹�Ă�)
const int32_t SPRITE_SRV_COUNT = 2056;

struct VertexPosUV
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
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
	DirectX::XMMATRIX mMatWorld;
	//�F
	Color mColor = { 1,1,1,1 };

	const Texture *mTEXTURE;

	Vector2 mAnchorpoint = { 0.5f,0.5f };

	bool mIsFlipX = false;
	bool mIsFlipY = false;

	Vector2 mTexLeftTop = {0,0};
	Vector2 mCutSize;

	bool mIsInvisible = false;

	Vector2 mSize = { 0.f,0.f };
protected:
	void Init();

private:
};

struct SpriteCommon
{
	PipelineSet mPipelineSet;
	//�ˉe�s��
	DirectX::XMMATRIX mMatProjection{};

	static void Initialize();

	static SpriteCommon sSpriteCommon;
};

//�X�v���C�g���ʃO���t�B�b�N�R�}���h�̃Z�b�g
void SpriteCommonBeginDraw();

void SpriteTransferVertexBuffer(const Sprite& sprite);

SpriteCommon SpriteCommonCreate();
