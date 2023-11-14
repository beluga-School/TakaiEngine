#pragma once
#include "Entity.h"
#include "Mob.h"

/*! Cannon
	モブが当たると飛んでいく大砲クラス
*/
class Cannon : public Entity
{
public:
	Cannon() : Entity() {
		SetTag(TagTable::Cannon);
	};

	void Initialize()override;

	void Update()override;

	void Draw()override;

	void OnCollide(Mob& mob);

	//今のところは中間点を1つしか設定できない
	
	//始点、中間点、終点
	Vector3 startPos = {};
	Vector3 interPos = {};
	Vector3 endPos = {};
	
	//番号を保持
	std::string id = "-1";

	void SetState(float maxtime);

private:
	//点の数(今後interを増やすときに使う)
	int32_t pointNum = 3;

	//合計の移動時間
	TEasing::easeTimer timer = 2.0f;

	//飛ばすモブを保持するポインタ
	Mob* target = nullptr;

	std::vector<Vector3> inters;

	Obj3d endTargetCircle;

	enum class CannonState
	{
		None,
		One,
	}state = CannonState::None;

	void HitEffect(){};
};