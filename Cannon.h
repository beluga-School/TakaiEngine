#pragma once
#include "EventBlock.h"
#include "Mob.h"

class Cannon : public EventBlock
{
public:
	Cannon() {
		tag = TagTable::Cannon;
	};

	void Initialize()override;

	void Update()override;

	void Draw()override;

	void OnCollide(Mob& mob);

	//���̂Ƃ���͒��ԓ_��1�����ݒ�ł��Ȃ�
	
	//�n�_�A���ԓ_�A�I�_
	Vector3 startPos = {};
	Vector3 interPos = {};
	Vector3 endPos = {};
	
	//�ԍ���ێ�
	std::string id = "-1";

	void SetState(float maxtime = 3.0f);

private:
	//�_�̐�(����inter�𑝂₷�Ƃ��Ɏg��)
	int32_t pointNum = 3;

	//���v�̈ړ�����
	TEasing::easeTimer timer = 3.0f;

	//��΂����u��ێ�����|�C���^
	Mob* target = nullptr;

	enum class CannonState
	{
		None,
		One,
		Two,
	}state = CannonState::None;

	void HitEffect(){};
};