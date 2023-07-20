#include "HPGauge.h"
#include "MathF.h"
#include "Util.h"

void HPGauge::Initialize()
{
	pos = { Util::WIN_WIDTH / 4,150 };

	for (int32_t i = 0; i < 8; i++)
	{
		ui[i].SetTexture(*TextureManager::GetTexture("hpGauge"));
		ui[i].SetAnchor({1,1});
		ui[i].SetPos(pos);
		ui[i].mRotation -= (360.f / 8.f) * i - (360.f / 8.f);

		//—ÎF‚Å‰Šú‰»
		ColorChange({ 0x00,0xff / 255,0x0d / 255,1 },i);
	}
}

void HPGauge::Update()
{
	for (int32_t i = 0; i < 8; i++)
	{
		ui[i].Update();
	}
}

void HPGauge::Draw()
{
	if (mIsVisilve == false)return;

	for (int32_t i = 0; i < 8; i++)
	{
		ui[i].Draw();
	}
}

void HPGauge::ColorChange(Float4 color, int32_t index)
{
	ui[index].mColor.f4 = color;
}
