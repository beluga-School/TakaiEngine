#include "BossHikiCamera.h"
#include "PlayerCamera.h"

void BossHikiCamera::Initialize()
{
}

void BossHikiCamera::Update()
{
	PlayerCamera::Get()->InitRadius();
}

void BossHikiCamera::Draw()
{
}

void BossHikiCamera::Start()
{
	PlayerCamera::Get()->ChangeDefaultRadius(20.0f);
}

void BossHikiCamera::End()
{
	PlayerCamera::Get()->ChangeDefaultRadius(8.0f);
	PlayerCamera::Get()->InitRadius();
}

bool BossHikiCamera::EndFlag()
{
	return false;
}
