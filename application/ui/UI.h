#pragma once
#include <InterfaceUI.h>
#include <Sprite.h>
#include <Texture.h>

/*! UI
	テクスチャベタ貼りのUIを作れるクラス
*/
class UI : public InterfaceUI
{
public:
	void SetTexture(const std::string& handle);

	void Update()override;
	void Draw()override;

	void SetPos(const Vector2 &pos);

	void SetSize(const Vector2& size);

	void SetColor(const Color& color);

private:
	Sprite sprite;
};

