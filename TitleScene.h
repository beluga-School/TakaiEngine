#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "TEasing.h"
#include "ImguiManager.h"

class TitleScene : public IScene
{
public:
	void LoadResource()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void End()override;

private:
	//Sprite titleUI;

	Sprite whiteOut;
	Sprite backHexagon;

	TEasing::easeTimer hexagonScaleTimer = 0.5f;

	enum class EffectState
	{
		None,		//何もない状態
		Appearance,	//出現
		ScreenShine,//画面が光る
		End,		//演出終わって画面が動く
	}effectState = EffectState::None;

	struct TitleString
	{
		Vector2 position;
		Sprite sprite;

		//落下までのタイマー
		TEasing::easeTimer timer = 0.2f;
	
		float startY;
		float endY;

		//シェイク時に元の位置に戻すためのやつ
		Vector2 savePos{};

		void Update();
		void Draw();
	};

	TitleString string[7];

	//ホントはカメラ側を揺らしたいけど、スプライト以外もいじらないといけなくなりそうなので
	//いったんタイトルだけ揺らす疑似シェイクを作る
	void SetShake(float power, float time);
	void ShakeUpdate();

	//シェイク系変数
	Vector2 shake{};
	TEasing::easeTimer shakeTimer;
	float shakePower = 0.f;

	//スクリーンを光らせるタイマー
	TEasing::easeTimer shineTimer = 1.0f;
	TEasing::easeTimer shineCoolTimer = 0.5f;
	TEasing::easeTimer shineEndTimer = 0.5f;
};

