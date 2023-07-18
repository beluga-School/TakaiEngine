#pragma once
#include "EventBlock.h"
#include "Mob.h"

class Cannon : public EventBlock
{
public:
	Cannon() {
		tag = TagTable::Cannon;
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

	void SetState(float maxtime = 3.0f);

private:
	//点の数(今後interを増やすときに使う)
	int32_t pointNum = 3;

	//合計の移動時間
	TEasing::easeTimer timer = 3.0f;

	//飛ばすモブを保持するポインタ
	Mob* target = nullptr;

	enum class CannonState
	{
		None,
		One,
		Two,
	}state = CannonState::None;

	void HitEffect(){};
};