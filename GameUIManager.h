#pragma once
#include <StarUI.h>
#include <StageTitleUI.h>

//�Q�[����ʏ�ɏo�Ă���2DUI���Ǘ�����}�l�[�W���[
class GameUIManager
{
public:
	static GameUIManager* Get() {
		static GameUIManager instance;
		return &instance;
	};

	static void LoadResource();

	void Move(UIMove uimove);

	void Initialize();
	void Update();
	void Draw();

	StageTitleUI* GetStageTitleUI()
	{
		return &stageTitleUI;
	}

private:
	GameUIManager() {};
	~GameUIManager() {};

	StarUI starUI;
	StageTitleUI stageTitleUI;
};

