#pragma once
#include "IEvent.h"
#include "TEasing.h"

//特に効果がないイベント　カメラで演出したいときに使う
class NoEffectEvent :public IEvent
{
public:
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void Start()override;
	void End()override;

	bool EndFlag()override;
};

