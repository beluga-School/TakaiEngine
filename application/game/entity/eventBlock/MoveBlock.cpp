#include "MoveBlock.h"

void MoveBlock::Initialize()
{
	Obj3d::Initialize();
	SetTexture(TextureManager::GetTexture("white"));

	moveTimer.Start();
}

void MoveBlock::Update()
{
	moveTimer.Update();

	if (moveTimer.GetEnd())
	{
		moveTimer.ReverseStart();
	}
	if (moveTimer.GetReverseEnd())
	{
		moveTimer.Start();
	}

	moveValue = { 0,0,0 };

	oldPos = position;

	//���W���X�V
	position = TEasing::lerp(startpos, endpos, moveTimer.GetTimeRate());

	moveValue = position - oldPos;

	box.CreateCol(position, box.scale);
	box.ColDrawerUpdate(position, box.scale);

	Obj3d::Update(*Camera::sCamera);

	//�ړ��ʂ𑫂��A�͂Ȃ񂩂������H�v���Ȃ��ƃ_������
	//���W��mob�̓����蔻�����ɌŒ肵�āA�ォ�烂�u�̈ړ������Z��������Ȃ�H
}

void MoveBlock::Draw()
{
	Obj3d::DrawMaterial();
}

void MoveBlock::HitEffect()
{
}

void MoveBlock::OnCollide(Mob* mob)
{
	//���[�u�u���b�N�ŉ��Z�����l��mob�ɍ��A�����ɒl����
	//�v�Z����Ƃ��ɒl������Ȃ�v�Z���銴����
	mob->moveBlockHit = true;
	
	//�ړ��u���b�N�p�ɍ��W���L�^����
	mob->moveBlockPosition = moveValue;
}
