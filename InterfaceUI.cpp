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

	//位置が初期でないなら
	if (pos.x != -1000 && pos.y != -1000)
	{
		//始まる位置を自身の位置に上書き
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
	//もし現在挙動と異なる方を確認したいと来たら失敗
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
