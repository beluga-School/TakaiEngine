#pragma once
#include "IEvent.h"
#include "TEasing.h"

//特に効果がないイベント　カメラで演出したいときに使う
class NoEffectEvent :public IEvent
{
public:
	void Start()override;
	void Update()override;
	void Draw()override;

	bool End()override;
};

