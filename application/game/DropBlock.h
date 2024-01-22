#pragma once
#include "Block.h"
#include "Enemy.h"

class DropBlock : public Enemy
{
public:
	DropBlock() : Enemy()
	{
		SetTag(TagTable::Block);
		Register();
	};

	//リソース読み込み
	static void LoadResource();

	void Initialize()override;

	void Update()override;

	void Draw()override;

	//ヒット時に発生すること
	void HitEffect()override;

	//エンカウント時の処理
	void Encount()override;
};

