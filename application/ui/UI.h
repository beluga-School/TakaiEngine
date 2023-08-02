#pragma once
#include "Sprite.h"
#include "Status.h"

class UI
{
public:
	static void LoadResource();
	UI(const Vector2& pos, float scale)
		: mPos(pos) {
		mScale.x = scale;
		mScale.y = scale;
		guiScale = scale;
	}
	void Initialize(const std::string &texturehandle);
	void Update();
	void Draw();

	/// <summary>
	/// ImGui内で自身の位置と大きさを調整できるようにする(GUIは別で用意する)
	/// </summary>
	void ValueSliders();

	void UpdateNumber(int32_t status);
	
	Vector2 mPos = {0,0};
	Vector2 mScale = {1,1};

protected:
	Sprite mainTex;
private:
	Sprite num[2];
	Sprite point;

	//imgui用の変数
	float guiScale = 1.0f;
	char output[50] = "";
	std::string savename = "";
};