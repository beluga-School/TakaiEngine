#pragma once
#include "IEvent.h"
#include "TEasing.h"

//���Ɍ��ʂ��Ȃ��C�x���g�@�J�����ŉ��o�������Ƃ��Ɏg��
class NoEffectEvent :public IEvent
{
public:
	void Start()override;
	void Update()override;
	void Draw()override;

	bool End()override;
};

