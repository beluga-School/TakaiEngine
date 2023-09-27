#pragma once
#include "UI.h"

class StarUI
{
public:
	static void LoadResource();

	void Initialize();
	void Update();
	void Draw();

	void Move(UIMove uimove);

private:
	UI uibase;
};

