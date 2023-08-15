#pragma once
#include "Collision.h"
#include <list>
#include "Obj.h"
#include "Entity.h"
#include "Mob.h"
#include "Block.h"
#include "Enemy.h"
#include "Player.h"

//���X�g�̒��ɓ����I�u�W�F�N�g������΁A���̃I�u�W�F�N�g�����Ȃ��v�b�V���o�b�N
void UniqueObjectPushBack(std::list<IDdCube>& list, const IDdCube& col);

//���X�g�̒��ɓ����I�u�W�F�N�g������΁A���̃I�u�W�F�N�g���폜����
void UniqueObjectErase(std::list<IDdCube>& list, const IDdCube& col);

class CollideManager
{
public:
	std::list<Entity*> allCols;
	
	//HP�̕ύX���Ԃ̕ω��ȂǁA�X�e�[�^�X��ύX���铖���蔻��̍X�V
	void StatusUpdate();
	
	//�����߂������ȂǁA���W���ω����铖���蔻��̍X�V
	void CollideUpdate();

	static CollideManager* Get()
	{
		static CollideManager instance;
		return &instance;
	}

	//�蓮�œ����蔻���ǂ�ł��������Ƃ��p�ɁA�ʂŕ�����
	
	//�n�`�X�V�p
	void CheckCollide(Entity* check, Entity* collide);

	//�X�e�[�^�X�X�V�p
	void CheckStatus(Entity* check, Entity* collide);

private:
	CollideManager(){};
	~CollideManager(){};

	//�����߂�����
	void Osimodosi(Mob& check, const Block& collide);

	//check��collide�̈ʒu�֌W�����āA�E�ɂ��邩���ɂ��邩�Ȃǂ�true/false��Ԃ�
	bool CheckDirections(const Cube& check, const Cube& collide, const CheckDirection& CD);

	//�v���C���[��Enemy�̓����蔻��
	void CheckPlayerToEnemy(Player& player, Enemy& collide);
};