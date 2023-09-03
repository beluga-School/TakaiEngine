#pragma once
#include <string>
#include "EventCamera.h"

//�C�x���g�̃C���^�[�t�F�[�X
class IEvent
{
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//�I������
	virtual bool End() = 0;

	//���s�ς݂Ȃ痧��
	bool isExecuted = false;

	std::string eventName;

	EventCamera eventCamera;
};