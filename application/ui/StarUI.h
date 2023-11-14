#pragma once
#include "UI.h"

/*! StarUI
	スター取得の状況を表示するクラス
	そのステージで取ったスターの合計数を表示
*/
class StarUI
{
public:
	static void LoadResource();

	void Initialize();
	void Update();
	void Draw();

	void Move(UIMove uimove);

	void CountUp();

	void CountDown();

	//残りのスターの数を代入する
	void Substitution(int32_t value);

private:

public:
	//UIがチュートリアルを通るまでは出現しないようにするフラグ(一度でも通ったら戻さない)
	bool uiAppearance = false;

private:
	UI uibase;

	int32_t count = 0;
};

