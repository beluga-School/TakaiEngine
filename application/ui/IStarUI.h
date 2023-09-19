#pragma once
#include "Sprite.h"
#include "Status.h"
#include "TEasing.h"

class IStarUI
{
public:
	static void StaticLoadResource();
	IStarUI(const Vector2& pos, float scale)
	: mPos (pos) {
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

	//出現状態で固定する
	void AppLock();

	//出現
	void Appearance(float time);
	//消滅
	void DisAppearance(float time);

protected:
	Sprite mainTex;
	Sprite mainTexBack;

	Sprite num[2];
	Sprite numBack[2];
	Sprite point;

	float mAlpha = 1.0f;
	bool mAppMode = true;

	TEasing::easeTimer alphaTimer = 0.2f;
	TEasing::easeTimer sideMoveTimer = 0.05f;

	Vector2 initpos{};

	//imgui用の変数
	float guiScale = 1.0f;
	char output[50] = "";
	std::string savename = "";
};