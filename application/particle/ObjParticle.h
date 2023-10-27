#pragma once
#include <memory>
#include <list>
#include "Obj.h"

/*! CubeParticle
	3Dオブジェクトパーティクルのインターフェース
*/
class IParticle
{
public:
	Obj3d cube;

	virtual ~IParticle() {};

	virtual void Update() = 0;

	void Draw() {
		cube.Draw();
	};

	bool isdead = true;
};

/*! CubeParticle
	四角3Dオブジェクトを使用したパーティクル
*/
class CubeParticle : public IParticle
{
public:
	CubeParticle();
	CubeParticle(const Vector3& pos, const Vector3& scale, const float& moveDistance,const DirectX::XMFLOAT4& color);

	void Set(const Vector3& pos, const Vector3& scale,const float& moveDistance,const DirectX::XMFLOAT4& color);

	void Update();
private:
	//回転速度の倍率
	float rotateMag = 10.0f;

	//動く速さ
	Vector3 moveSpeed = {0,0,0};

	//縮小する速さ
	Vector3 shrinkSpeed = { 1.0f,1.0f,1.0f };

	Vector3 rotateSpeed = { 0,0,0 };
};

/*! ParticleManager
	3Dオブジェクトパーティクルを生成、管理するクラス
*/
class ParticleManager
{
public:
	static ParticleManager* GetInstance();

	void CreatePool();
	void AllDelete();

	void CreateCubeParticle(const Vector3& pos, const Vector3& scale, const float& moveDistance, const DirectX::XMFLOAT4& color);

	void Update();
	void Draw();

private:
	std::list<std::unique_ptr<CubeParticle>> cubePool;

	ParticleManager() {};
	~ParticleManager() {};

	ParticleManager(const ParticleManager& a) = delete;
	ParticleManager& operator=(const ParticleManager&) = delete;
};
