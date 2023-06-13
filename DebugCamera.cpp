#include "DebugCamera.h"
#include <Input.h>
#include <MathF.h>
#include <ImguiManager.h>

using namespace Input;

void DebugCamera::Initialize()
{
	Obj3d::Initialize();
	Camera::camera->Initialize();
}

void DebugCamera::Update()
{
	centerVec = matWorld.ExtractAxisZ();
	sideVec = matWorld.ExtractAxisX();

	int16_t inputCenter = (InputKey::Get()->PushKey(DIK_DOWN) - InputKey::Get()->PushKey(DIK_UP));
	position.x += inputCenter * centerVec.x * moveSpeed;
	position.y += inputCenter * centerVec.y * moveSpeed;
	position.z += inputCenter * centerVec.z * moveSpeed;

	int16_t inputSide = (InputKey::Get()->PushKey(DIK_LEFT) - InputKey::Get()->PushKey(DIK_RIGHT));
	position.x += inputSide * sideVec.x * moveSpeed;
	position.y += inputSide * sideVec.y * moveSpeed;
	position.z += inputSide * sideVec.z * moveSpeed;

	//À•W‚Ì’Ç]
	Camera::camera->eye = position + (centerVec * radius);
	Camera::camera->target = position;

	if (Mouse::Wheel() < 0)
	{
		radius += 2.0f;
	}
	if (Mouse::Wheel() > 0)
	{
		radius -= 2.0f;
	}

	radius = Util::Clamp(radius, 1.0f, 100.f);

	//‰ñ“]‚³‚¹‚éˆ—
	if (Mouse::Down(Click::MIDDLE))
	{
		verticalRad += MathF::AngleConvRad(Mouse::GetVelocity().y);
		horizontalRad += MathF::AngleConvRad(Mouse::GetVelocity().x);

		//ŒÀŠE’l‚ð’´‚¦‚È‚¢ˆ—
		if (verticalRad > MathF::PIf / 2 - MathF::AngleConvRad(1.0f)) verticalRad = MathF::PIf / 2 - MathF::AngleConvRad(1.0f);
		if (verticalRad < -MathF::PIf / 2 + MathF::AngleConvRad(1.0f)) verticalRad = -MathF::PIf / 2 + MathF::AngleConvRad(1.0f);

		rotation.x = verticalRad;
		rotation.y = horizontalRad;
	}

	Obj3d::Update(*Camera::camera);
	Camera::camera->UpdatematView();
}
