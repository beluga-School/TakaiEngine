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
	/// �O����󂯎�����ݒ肷��֐�
	/// </summary>
	void SetInfo(const Vector3& position,const Vector3& scale,const Vector2& tiling);

	float GetPosY();

private:
	//�g������鑬�x
	TEasing::easeTimer flowTimer = 100.0f;

	//�㉺�ɗh��銴�o
	TEasing::easeTimer updownTimer = 5.0f;

	Vector3 initPos{};
	bool updown = false;

	//�㉺�ɗh���傫���̔��a
	float updownRadius = 0.2f;
};

