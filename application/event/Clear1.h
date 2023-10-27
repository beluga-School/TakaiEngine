#pragma once
#include "IEvent.h"
#include "TEasing.h"

/*! Clear1
	ステージ1クリア後のイベント
*/
class Clear1 : public IEvent
{
public:
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void Start()override;
	void End()override;

	bool EndFlag()override;

	TEasing::easeTimer timer = 2.0f;
	bool setFlag = false;
};

