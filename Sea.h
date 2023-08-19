#pragma once
#include "Obj.h"
#include "TEasing.h"

class Sea final : protected Obj3d
{
public:
	void LoadResource();
	void Initialize();
	void Update();
	void Draw();

	/// <summary>
	/// ŠO‚©‚çó‚¯æ‚éî•ñ‚ğİ’è‚·‚éŠÖ”
	/// </summary>
	void SetInfo(const Vector3& position,const Vector3& scale,const Vector2& tiling);

	float GetPosY();

private:
	//”g‚ª—¬‚ê‚é‘¬“x
	TEasing::easeTimer flowTimer = 100.0f;

	//ã‰º‚É—h‚ê‚éŠ´Šo
	TEasing::easeTimer updownTimer = 5.0f;

	Vector3 initPos{};
	bool updown = false;

	//ã‰º‚É—h‚ê‚é‘å‚«‚³‚Ì”¼Œa
	float updownRadius = 0.2f;
};

