#include "StarUI.h"

void StarUI::Initialize()
{
	ui.SetTexture(*TextureManager::GetTexture("white"));
}

void StarUI::Update()
{
	ui.Update();
}

void StarUI::Draw()
{
	ui.Draw();
}
