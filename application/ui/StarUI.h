#pragma once
#include "UI.h"
#include "Star.h"

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

	//
	void StarCount();

	//全てのスターを入手しているか
	bool CheckAllCollect();

private:

public:
	//UIがチュートリアルを通るまでは出現しないようにするフラグ(一度でも通ったら戻さない)
	bool uiAppearance = false;

private:
	UI uibase;

	std::vector<Star*> stars;
};

