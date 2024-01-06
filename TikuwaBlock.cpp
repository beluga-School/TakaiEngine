#include "TikuwaBlock.h"
#include "MathF.h"

void TikuwaBlock::Initialize()
{
	Obj3d::Initialize();
	SetTexture(TextureManager::GetTexture("white"));
}

void TikuwaBlock::Update()
{
	deleteTimer.Update();
	scalingTimer.Update();
	reScalingTimer.Update();
	respawnTimer.Update();

	if (reScalingTimer.GetEnd())
	{
		scalingTimer.Reset();
		deleteTimer.Reset();
		respawnTimer.Reset();
		reScalingTimer.Reset();
	}

	//消える実装
	if (deleteTimer.GetRun())
	{
		//位置リセット
		position = savePosition;

		saveEndPosition = position;
		saveEndPosition.y -= 10;

		//震える挙動(みため)
		//適当に揺らす
		float rand = MathF::GetRand(-0.25f, 0.25f);
		position += {rand, 0, rand};
	}

	if (scalingTimer.GetRun()) {
		scale = TEasing::OutQuad(saveScale, { 0,0,0 }, scalingTimer.GetTimeRate());
		box.scale = TEasing::OutQuad(saveBoxScale, { 0,0,0 }, scalingTimer.GetTimeRate());
		position = TEasing::OutQuad(savePosition, saveEndPosition, scalingTimer.GetTimeRate());
		box.position = TEasing::OutQuad(savePosition, saveEndPosition, scalingTimer.GetTimeRate());
	}
	if (reScalingTimer.GetRun()) {
		position = savePosition;
		box.position = savePosition;
		scale = TEasing::OutQuad({ 0,0,0 }, saveScale, reScalingTimer.GetTimeRate());
		box.scale = TEasing::OutQuad({ 0,0,0 }, saveBoxScale, reScalingTimer.GetTimeRate());
	}

	if (deleteTimer.GetEnd())
	{
		if(!scalingTimer.GetStarted())scalingTimer.Start();
	}
	if (scalingTimer.GetEnd())
	{
		if (!respawnTimer.GetStarted())
		{
			isDraw = false;
			respawnTimer.Start();
		}
	}
	if (respawnTimer.GetEnd())
	{
		if (!reScalingTimer.GetStarted()) {
			isDraw = true;
			reScalingTimer.Start();
		}
	}

	Obj3d::Update(*Camera::sCamera);
}

void TikuwaBlock::Draw()
{
	if (!isDraw) {

		return;
	}

	if (isTexDraw)
	{
		Obj3d::Draw();
	}
	else
	{
		Obj3d::DrawMaterial();
	}
}

void TikuwaBlock::HitEffect()
{
	if (!deleteTimer.GetStarted()) {
		deleteTimer.Start();
	}
}
