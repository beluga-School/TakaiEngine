#pragma once
#include "Obj.h"
#include "TEasing.h"

/*! Sea
	海
	プレイヤーが落ちるとリセットになる
*/
class Sea final : protected Obj3d
{
public:
	void LoadResource();
	void Initialize();
	void Update();
	void Draw();

	/// <summary>
	/// 外から受け取る情報を設定する関数
	/// </summary>
	void SetInfo(const Vector3& position,const Vector3& scale,const Vector2& tiling);

	float GetPosY();

private:
	//波が流れる速度
	TEasing::easeTimer flowTimer = 100.0f;

	//上下に揺れる感覚
	TEasing::easeTimer updownTimer = 5.0f;

	Vector3 initPos{};
	bool updown = false;

	//上下に揺れる大きさの半径
	float updownRadius = 0.2f;
};

