#pragma once
#include "Entity.h"
#include "TEasing.h"
#include "Mob.h"
#include "Obj.h"

/*! Signboard
	テクスチャを貼れる看板クラス
*/
class Signboard : public Entity
{
public:
	Signboard() : Entity()
	{
		SetTag(TagTable::DitherTransparent);
	};

	//リソース読み込み
	static void LoadResource();

	void Initialize()override;
	void Update()override;
	void Draw()override;

	//貼り付けるテクスチャの設定
	void SetPicture(const std::string& texHandle);

private:
	Obj3d pictureObj;
	//テクスチャサイズのパラメータ化
	//後でBlenderからいじれるようにする
	float width = 2.0f;
	float height = 2.0f;
};

