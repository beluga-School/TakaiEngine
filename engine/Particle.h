#pragma once
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include "DirectXInit.h"
#include <vector>
#include "Texture.h"
#include "ConstBuffer.h"
#include <list>
#include "Obj.h"

#include <Vector3.h>
#include <Vector4.h>

struct VertexPos
{
	Vector3 pos;
	float scale;
	Vector4 color;
};

struct ParticleConstBufferData
{
	XMMATRIX vpMat;
	XMMATRIX billboardMat;
};

const uint32_t VERTEX_COUNT = 1000;

class Particle
{
public:
	//座標
	Vector3 mPosition;
	//方向
	Vector3 mVelocity;
	//速度
	float mSpeed;
	//大きさ
	float mScale;
	//色
	Vector4 mColor;

	Vector3 mColorRand = { 0,0,0 };

	VertexPos mVertPos{};

	bool mIsDead = false;

	//生存時間(秒単位)
	float mLifeTime = 0.0f;
	float mMaxLifeTime = 0.0f;

	bool mRedChange = false;

	Particle(const Vector3& spawnPos,const Vector3& velocity,
		const float& scale, const float& speed, const float& maxLifeTime, 
		const Vector4& color,const bool& redChange);

	void Update();
};

class GParticleManager
{
public:
	ConstBuffer<ParticleConstBufferData> mConstBufferParticle;

	D3D12_RESOURCE_DESC mResDesc{};

	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW mVbView{};

	ComPtr<ID3D12Resource> mVertBuff;

	//VertexPos verticesPoint[vertexCount];
	std::list<Particle> mParticles;
	void CreateParticle(const Vector3& spawnPos,const Vector3& velocity,
		const float& scale, const float& speed, const bool& redChange,
		const float& maxLifeTime = 1.0f,const Vector4& color = {1,1,1,1});

	void Initialize();
	
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);

	void Draw(Texture* texture);

	static GParticleManager*Getinstance();

private:
	GParticleManager() {};
	~GParticleManager() {};

	GParticleManager(const GParticleManager& a) = delete;
	GParticleManager& operator=(const GParticleManager&) = delete;
};

class ParticleEmitter
{
public:
	Obj3d mEmitter;

	void Initialize();

	/// <summary>
	/// エミッターの情報を書き換える
	/// </summary>
	/// <param name="pos">エミッターの座標</param>
	/// <param name="range">posから全方向に広がるパーティクルの発生する範囲</param>
	/// <param name="scale">パーティクルの大きさ</param>
	/// <param name="color">パーティクルの色</param>
	/// <param name="spawnNum">1秒間に発生するパーティクルの数 最低値は1</param>
	void SetInfo(const Vector3& pos,const float& range, const float& scale,
		const Vector4& color,const int32_t& spawnNum = 1,const bool& redChange = false);

	void Update();

private:
	Model mModel;
	float mRange = 10;
	float mInitScale = 1;
	Vector4 mColor = { 1,1,1,1 };
	//一秒間に発生する数
	int32_t mSpawnNum = 10;

	bool mRedChange = false;

};