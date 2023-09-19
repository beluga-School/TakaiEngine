#include "InterfaceUI.h"

void InterfaceUI::InitPos(const Vector2& start, const Vector2& end)
{
	saveStartPos = start;
	saveEndPos = end;
}

void InterfaceUI::Move(UIMove move)
{
	flag = move;
	moveTimer.Start();

	startPos = saveStartPos;
	endPos = saveEndPos;

	//�ʒu�������łȂ��Ȃ�
	if (pos.x != -1000 && pos.y != -1000)
	{
		//�n�܂�ʒu�����g�̈ʒu�ɏ㏑��
		switch (move)
		{
		case UIMove::START:
			startPos = pos;
			break;
		case UIMove::END:
			endPos = pos;
			break;
		}
	}
}

Vector2 InterfaceUI::GetPos()
{
	return pos;
}

bool InterfaceUI::GetEnd(UIMove move)
{
	//�������݋����ƈقȂ�����m�F�������Ɨ����玸�s
	if (flag != move)return false;
	return moveTimer.GetEnd();
}

void InterfaceUI::InterFaceUpdate()
{
	moveTimer.Update();

	if (flag == UIMove::START)
	{
		pos.x = TEasing::OutQuad(startPos.x, endPos.x, moveTimer.GetTimeRate());
		pos.y = TEasing::OutQuad(startPos.y, endPos.y, moveTimer.GetTimeRate());
	}
	if (flag == UIMove::END)
	{
		pos.x = TEasing::InQuad(endPos.x, startPos.x, moveTimer.GetTimeRate());
		pos.y = TEasing::InQuad(endPos.y, startPos.y, moveTimer.GetTimeRate());
	}
}
