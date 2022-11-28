#pragma once
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>

#include "DirectXInit.h"
#include <vector>
#include "Texture.h"
#include "ConstBuffer.h"
#include <list>
#include "Obj.h"

struct VertexPos
{
	XMFLOAT3 pos;
	float scale;
	XMFLOAT4 color;
};

struct ParticleConstBufferData
{
	XMMATRIX vpMat;
	XMMATRIX billboardMat;
};

const int vertexCount = 1000;

class Particle
{
public:
	//���W
	XMFLOAT3 position;
	//����
	XMFLOAT3 velocity;
	//���x
	float speed;
	//�傫��
	float scale;
	//�F
	XMFLOAT4 color;

	XMFLOAT3 colorRand = { 0,0,0 };

	VertexPos vertPos;

	bool isDead = false;

	//��������(�b�P��)
	float lifeTime = 0.0f;
	float maxLifeTime = 0.0f;

	bool redChange = false;

	Particle(XMFLOAT3 spawnPos, XMFLOAT3 velocity,float scale, float speed, float maxLifeTime,XMFLOAT4 color, bool redChange);

	void Update();
};

class ParticleManager
{
public:
	ConstBuffer<ParticleConstBufferData> constBufferParticle;

	D3D12_RESOURCE_DESC resDesc{};

	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	ComPtr<ID3D12Resource> vertBuff;

	//VertexPos verticesPoint[vertexCount];
	std::list<Particle> particles;
	void CreateParticle(XMFLOAT3 spawnPos, XMFLOAT3 velocity, float scale, float speed, bool redChange, float maxLifeTime = 1.0f, XMFLOAT4 color = {1,1,1,1});

	void Initialize();
	
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);

	void Draw(Texture* texture);

	static ParticleManager *Getinstance();

private:
	ParticleManager() {};
	~ParticleManager() {};

	ParticleManager(const ParticleManager& a) = delete;
	ParticleManager& operator=(const ParticleManager&) = delete;
};

class ParticleEmitter
{
public:
	Obj3d emitter;

	void Initialize();

	/// <summary>
	/// �G�~�b�^�[�̏�������������
	/// </summary>
	/// <param name="pos">�G�~�b�^�[�̍��W</param>
	/// <param name="range">pos����S�����ɍL����p�[�e�B�N���̔�������͈�</param>
	/// <param name="scale">�p�[�e�B�N���̑傫��</param>
	/// <param name="color">�p�[�e�B�N���̐F</param>
	/// <param name="spawnNum">1�b�Ԃɔ�������p�[�e�B�N���̐� �Œ�l��1</param>
	void SetInfo(Vector3 pos,float range,float scale, XMFLOAT4 color,int spawnNum = 1,bool redChange = false);

	void Update();

private:
	Model model;
	float range = 10;
	float initScale = 1;
	XMFLOAT4 color = { 1,1,1,1 };
	//��b�Ԃɔ������鐔
	int spawnNum = 10;

	bool redChange = false;

};