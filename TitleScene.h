#pragma once
#include "IScene.h"
#include "Sprite.h"

class TitleScene : public IScene
{
public:
	void LoadResource()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void End()override;

private:
	Sprite titleUI;
};

