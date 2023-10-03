#pragma once
#include <string>
#include "EventCamera.h"
#include <list>

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

	//�J�������g�p���邩
	bool isUseCamera = false;

	//�v���C���[�𓮂��Ȃ����邩

};