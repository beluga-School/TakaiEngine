#pragma once
#include <string>

//�C�x���g�̃C���^�[�t�F�[�X
class IEvent
{
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//���s����
	virtual bool RunFlag() = 0;
	//�I������
	virtual bool End() = 0;

	std::string eventName;
};