#pragma once
#include "IEvent.h"
#include "TEasing.h"

/*! Clear1
	ステージ1クリア後のイベント
*/
class ClearEvent : public IEvent
{
public:
	ClearEvent() : IEvent() {
		isUseEventLine = false;
		isTrigerEvent = true;
	}
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void Start()override;
	void End()override;

	bool EndFlag()override;

private:
	TEasing::easeTimer spriteTimer = 0.5f;
	TEasing::easeTimer changeTimer = 2.0f;

	Vector2 spritePos = { Util::WIN_WIDTH / 2,-300 };
};

