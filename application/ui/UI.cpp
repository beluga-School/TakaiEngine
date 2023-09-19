#include "UI.h"

void UI::SetTexture(const std::string& handle)
{
	sprite.SetTexture(*TextureManager::GetTexture(handle));
	
}

void UI::Update()
{
	InterFaceUpdate();

	sprite.mPosition = { pos.x,pos.y,0};
	sprite.Update();
}

void UI::Draw()
{
	sprite.Draw();
}

void UI::SetSize(const Vector2& size)
{
	sprite.SetSize({
		sprite.GetInitSize().x * size.x,
		sprite.GetInitSize().y * size.y,
		});
}
