#include "InterfaceUI.h"

void InterfaceUI::InitPos(const Vector2& start, const Vector2& end)
{
	saveStartPos = start;
	saveEndPos = end;

	pos = { -1000,-1000 };
}

void InterfaceUI::InitScale(const Vector2& min, const Vector2& max)
{
	minScale = min;
	maxScale = max;
}

void InterfaceUI::Initialize()
{
	moveTimer.Reset();
	scalingTimer.Reset();

	move = UIMove::NONE;
}

void InterfaceUI::Move(UIMove move_)
{
	if (move == UIMove::NONE && move_ == UIMove::END)
	{
		return;
	}

	move = move_;
	moveTimer.Start();

	startPos = saveStartPos;
	endPos = saveEndPos;

	//位置が初期でないなら
	if (!GetNowInitPos())
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

void InterfaceUI::Scaling(UIMove scale_)
{
	scaling = scale_;
	scalingTimer.Start();
}

Vector2 InterfaceUI::GetPos()
{
	return pos;
}

bool InterfaceUI::GetMoveEnd(UIMove move_)
{
	//もし現在挙動と異なる方を確認したいと来たら失敗
	if (move != move_)return false;
	return moveTimer.GetEnd();
}

bool InterfaceUI::GetScaleEnd(UIMove scale_)
{
	//もし現在挙動と異なる方を確認したいと来たら失敗
	if (scaling != scale_)return false;
	return scalingTimer.GetEnd();
}

bool InterfaceUI::GetNowInitPos()
{
	return pos.x == -1000 && pos.y == -1000;
}

void InterfaceUI::InterFaceUpdate()
{
	moveTimer.Update();
	scalingTimer.Update();

	if (move == UIMove::START)
	{
		pos.x = TEasing::OutQuad(startPos.x, endPos.x, moveTimer.GetTimeRate());
		pos.y = TEasing::OutQuad(startPos.y, endPos.y, moveTimer.GetTimeRate());
	}
	if (move == UIMove::END)
	{
		pos.x = TEasing::InQuad(endPos.x, startPos.x, moveTimer.GetTimeRate());
		pos.y = TEasing::InQuad(endPos.y, startPos.y, moveTimer.GetTimeRate());
	}
	if (scaling == UIMove::START)
	{
		scale.x = TEasing::OutQuad(minScale.x, maxScale.x, scalingTimer.GetTimeRate());
		scale.y = TEasing::OutQuad(minScale.y, maxScale.y, scalingTimer.GetTimeRate());
	}
	if (scaling == UIMove::END)
	{
		scale.x = TEasing::InQuad(maxScale.x, minScale.x, scalingTimer.GetTimeRate());
		scale.y = TEasing::InQuad(maxScale.y, minScale.y, scalingTimer.GetTimeRate());
	}
}
