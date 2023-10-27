#pragma once
#include "UI.h"

/*! StarUI
	スター取得の状況を表示するクラス(未使用)
*/
class StarUI
{
public:
	static void LoadResource();

	void Initialize();
	void Update();
	void Draw();

	void Move(UIMove uimove);

private:
	UI uibase;
};

