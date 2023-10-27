#pragma once
#include "IEvent.h"
#include "TEasing.h"
#include "UI.h"

/*! TutorialUIEyeMove
	視点移動の操作チュートリアルを表示するイベント
*/
class TutorialUIEyeMove : public IEvent
{
public:
	TutorialUIEyeMove() : IEvent()
	{
		isUseEventLine = false;
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void Start()override;
	void End()override;

	bool EndFlag()override;

private:
	
};

