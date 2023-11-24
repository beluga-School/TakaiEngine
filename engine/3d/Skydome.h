#pragma once
#include "Obj.h"

/*! Skydome
	天球クラス
*/
class Skydome : public Obj3d
{
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
};

