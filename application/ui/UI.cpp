#include "UI.h"
#include <ClearDrawScreen.h>

void UI::SetTexture(const std::string& handle)
{
	sprite.SetTexture(*TextureManager::GetTexture(handle));	
}

void UI::Update()
{
	InterFaceUpdate();

	SetSize(scale);
	sprite.mPosition = { pos.x,pos.y,0};
	sprite.Update();
}

void UI::Draw()
{
	SpriteCommonBeginDraw();
	sprite.Draw();
}

void UI::SetSize(const Vector2& size)
{
	sprite.SetSize({
		sprite.GetInitSize().x * size.x,
		sprite.GetInitSize().y * size.y,
		});
}

void UI::SetColor(const Color& color)
{
	sprite.SetColor(color);
}
