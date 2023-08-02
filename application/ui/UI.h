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
	/// ImGui���Ŏ��g�̈ʒu�Ƒ傫���𒲐��ł���悤�ɂ���(GUI�͕ʂŗp�ӂ���)
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

	//imgui�p�̕ϐ�
	float guiScale = 1.0f;
	char output[50] = "";
	std::string savename = "";
};