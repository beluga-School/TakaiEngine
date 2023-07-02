#pragma once
#include "Sprite.h"

//ゴール後の処理を進めるクラス
//今はスプライト出すだけ
//今後ステージに持たせて、1ステージ=1ゴールをもつ、という形にしたい
class GoalSystem
{
public:
	void LoadResource();

	void Initialize();
	void Update();
	void Draw();

	//クリアフラグ
	bool mIsClear = false;

private:
	Sprite mClearSprite;

};

