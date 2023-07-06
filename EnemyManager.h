#pragma once
#include "LevelLoader.h"
#include "Enemy.h"
#include "ImguiManager.h"

//�S�Ă̓G���Ǘ�����EnemyManager������
//�������G�̃��[�_�[�Ƃ��Ă̋@�\������

class EnemyManager
{
public:
	//��������eventtriger��"enemy"���ď����Ă�������GEnemy��z�u����݌v��
	void Load(const LevelData::ObjectData &data);

	static EnemyManager* Get()
	{
		static EnemyManager instance;
		return &instance;
	}

	void Initialize();
	void Update();
	void Draw();

	std::list<std::unique_ptr<Enemy>> enemyList;
private:
	GUI gui3 = {"check"};
};

