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
		None,		//�����Ȃ����
		Appearance,	//�o��
		ScreenShine,//��ʂ�����
		End,		//���o�I����ĉ�ʂ�����
	}effectState = EffectState::None;

	struct TitleString
	{
		Vector2 position;
		Sprite sprite;

		//�����܂ł̃^�C�}�[
		TEasing::easeTimer timer = 0.2f;
	
		float startY;
		float endY;

		//�V�F�C�N���Ɍ��̈ʒu�ɖ߂����߂̂��
		Vector2 savePos{};

		void Update();
		void Draw();
	};

	TitleString string[7];

	//�z���g�̓J��������h�炵�������ǁA�X�v���C�g�ȊO��������Ȃ��Ƃ����Ȃ��Ȃ肻���Ȃ̂�
	//��������^�C�g�������h�炷�^���V�F�C�N�����
	void SetShake(float power, float time);
	void ShakeUpdate();

	//�V�F�C�N�n�ϐ�
	Vector2 shake{};
	TEasing::easeTimer shakeTimer;
	float shakePower = 0.f;

	//�X�N���[�������点��^�C�}�[
	TEasing::easeTimer shineTimer = 1.0f;
	TEasing::easeTimer shineCoolTimer = 0.5f;
	TEasing::easeTimer shineEndTimer = 0.5f;
};

