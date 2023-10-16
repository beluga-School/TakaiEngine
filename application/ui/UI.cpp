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
	sprite.mPosition = pos;
	sprite.Update();
}

void UI::Draw()
{
	SpriteCommonBeginDraw();
	sprite.Draw();
}

void UI::SetPos(const Vector2& pos_)
{
	pos = pos_;
	sprite.mPosition = pos_;
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
	sprite.mColor = color;
}
