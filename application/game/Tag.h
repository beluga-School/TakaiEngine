#pragma once

//�����蔻������Ȃ�A���̓����蔻��I�u�W�F�N�g�S�ĂƔ�����Ƃ�
//->������惊�X�g�ɓ����
//�����͐������Ȃ̂ŁA�S���̔���Ǝ�����Ă����^�O(string)�Ŕ��ʂł���悤�ɂ���
//�����͒n�ʂƂ��Ȃ̂ŁA�������Ƃ�����������Ȃ�����Ă����^�O�Ŕ���
enum class TagTable
{
	None = -1,
	Mob = 0,
	Block = 1,
	Cannon = 2,
	Box = 3,
	Collsion = 4,
	Player = 5,
	Enemy = 6,
	Star = 7,
	WarpBlock = 8,
	Coin = 9,
	Dokan = 10,
	NoDraw = 11,
	MoveBlock = 12,
};