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
	//座標
	XMFLOAT3 position;
	//方向
	XMFLOAT3 velocity;
	//速度
	float speed;
	//大きさ
	float scale;
	//色
	XMFLOAT4 color;

	XMFLOAT3 colorRand = { 0,0,0 };

	VertexPos vertPos;

	bool isDead = false;

	//生存時間(秒単位)
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

	//頂点バッファビューの作成
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
	/// エミッターの情報を書き換える
	/// </summary>
	/// <param name="pos">エミッターの座標</param>
	/// <param name="range">posから全方向に広がるパーティクルの発生する範囲</param>
	/// <param name="scale">パーティクルの大きさ</param>
	/// <param name="color">パーティクルの色</param>
	/// <param name="spawnNum">1秒間に発生するパーティクルの数 最低値は1</param>
	void SetInfo(Vector3 pos,float range,float scale, XMFLOAT4 color,int spawnNum = 1,bool redChange = false);

	void Update();

private:
	Model model;
	float range = 10;
	float initScale = 1;
	XMFLOAT4 color = { 1,1,1,1 };
	//一秒間に発生する数
	int spawnNum = 10;

	bool redChange = false;

};