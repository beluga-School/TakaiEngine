#pragma once
#include <InterfaceUI.h>
#include <Sprite.h>
#include <Texture.h>

//�e�N�X�`���x�^�\���UI������N���X
class UI : public InterfaceUI
{
public:
	void SetTexture(const std::string& handle);

	void Update()override;
	void Draw()override;

	void SetSize(const Vector2& size);

private:
	Sprite sprite;
};

