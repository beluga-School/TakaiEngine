#pragma once
#include "EventBlock.h"
#include "TEasing.h"
#include "Mob.h"
#include "Obj.h"

class Signboard : public EventBlock
{
public:
	Signboard() : EventBlock()
	{
	};

	static void LoadResource();

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void HitEffect()override;

	void SetPicture(const std::string& texHandle);

private:
	Obj3d pictureObj;
	//テクスチャサイズのパラメータ化
	//後でBlenderからいじれるようにする
	float width = 2.0f;
	float height = 2.0f;
};

