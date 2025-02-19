#pragma once
#include "LevelLoader.h"
#include "Enemy.h"
#include "ImguiManager.h"
#include "MathF.h"

/*! EnemyManager
	エネミーを管理するクラス
*/

class EnemyManager
{
public:
	//敵のオブジェクトデータを読み込む
	void Load(const LevelData::ObjectData &data);
	
	static EnemyManager* Get()
	{
		static EnemyManager instance;
		return &instance;
	}

	void Initialize();
	void Update();
	void Draw();

	//敵を出現させる処理
	template <class TEnemy> Enemy* PopEnemy(const Vector3& posision, const Vector3& rotation, const Vector3& scale);

	//イベント名を元に、事前に配置している敵をその座標に出現させる処理
	std::list<Enemy*> PopEventEnemy(std::string eventname_);

public:
	std::list<std::unique_ptr<Enemy>> enemyList;

	bool mIsDrawEncountSphere = false;

private:

};

template<class TEnemy>
inline Enemy* EnemyManager::PopEnemy(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	enemyList.emplace_back();
	enemyList.back() = std::make_unique<TEnemy>();
	enemyList.back()->Initialize();
	enemyList.back()->SetShadow();

	//座標
	enemyList.back()->position = position;
	//回転角
	enemyList.back()->rotation = {
			MathF::AngleConvRad(rotation.x),
			MathF::AngleConvRad(rotation.y),
			MathF::AngleConvRad(rotation.z)
	};
	//大きさ
	enemyList.back()->scale = scale;

	enemyList.back()->SetInitScale(scale);

	enemyList.back()->box.SetTexture(TextureManager::GetTexture("white"));
	enemyList.back()->box.SetModel(ModelManager::GetModel("Cube"));

	return enemyList.back().get();
}
