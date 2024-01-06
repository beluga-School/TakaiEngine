#pragma once
#include "Block.h"

class TikuwaBlock : public Block
{
public:
	TikuwaBlock() : Block() {
		SetTag(TagTable::TikuwaBlock);
	};

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void HitEffect();

	Vector3 saveScale = { 0,0,0 };
	Vector3 saveBoxScale = { 0,0,0 };
	Vector3 savePosition = { 0,0,0 };

private:
	TEasing::easeTimer deleteTimer = 2.0f;
	TEasing::easeTimer scalingTimer = 1.0f;
	TEasing::easeTimer respawnTimer = 1.0f;
	TEasing::easeTimer reScalingTimer = 0.5f;

	Vector3 saveEndPosition = { 0,0,0 };

	bool isDraw = true;
};

