#pragma once
#include "Texture.h"
#include "TEasing.h"
#include "Billboard.h"

/*! IParticle2D
	2Dパーティクルの基底クラス
*/
class IParticle2D
{
public:
	//初期設定
	IParticle2D()
	{
		mActive = true;
		mLifeTimer.Start();
	}

	//更新
	void Update();

	TEasing::easeTimer mLifeTimer;

	bool mActive = false;

	BillboardY billboard;
};

