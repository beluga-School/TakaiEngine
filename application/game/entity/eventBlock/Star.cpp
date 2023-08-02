#include "Star.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "TimeManager.h"
#include "MathF.h"
#include "ObjParticle.h"

void Star::Initialize()
{
	SetModel(ModelManager::GetModel("star"));
	SetTexture(TextureManager::GetTexture("white"));
}

void Star::Update()
{
	particleTimer.Update();
	jumpUpTimer.Update();
	inholeTimer.Update();
	delayTimer.Update();

	//ここから別挙動 starUIで行う
	//�@(�@の取得と同じタイミングで)starの文字列UIを出す
	//�Astarが流れきったらGet!の文字列UIを出す
	//�A.5 派手さが足りなかったらここで画面全体を横切るびーとだうん！UIを出す
	//�Bスターの画像を白で塗りつぶしたやつを出す
	//�C �@�A�Bを拡大する 拡大しきった時に通常のスターの画像を拡大せずに出す
	//�D �@�A�Bを縮小する

	float randScale = 0.0f;

	switch (starState)
	{
	case Star::StarState::None:
		//1秒間に0.25回転してる
		rotaSpeed = 0.25f;
		if (particleTimer.GetRun() == false)
		{
			particleTimer.Start();
			for (int i = 0; i < 3; i++)
			{
				ParticleManager::GetInstance()->CreateCubeParticle(position, { 1.5f,1.5f,1.5f }, 2.0f, { 1.0f,1.0f,0.0f,1 });
			}
		}

		break;
	case Star::StarState::jumpUp:
		//1秒間に2回転する
		rotaSpeed = 2;

		jumpUpPos = saveStartPos;
		jumpUpPos.y += 10.0f;

		jumpUpScale = saveScale * 1.5f;

		scale = TEasing::OutQuad(saveScale, jumpUpScale, jumpUpTimer.GetTimeRate());
		position = TEasing::OutQuad(saveStartPos, jumpUpPos, jumpUpTimer.GetTimeRate());

		
		if (jumpUpTimer.GetEnd())
		{
			if (delayTimer.GetStarted() == false)delayTimer.Start();
		}
		if (delayTimer.GetEnd())
		{
			starState = StarState::Inhole;
			inholeTimer.Start();
		}

		break;
	case Star::StarState::Inhole:

		//1秒間に2回転する
		rotaSpeed = 2;

		scale = TEasing::OutQuad(jumpUpScale, { 0,0,0 }, inholeTimer.GetTimeRate());
		position = TEasing::OutQuad(jumpUpPos, Player::Get()->position, inholeTimer.GetTimeRate());

		if (inholeTimer.GetEnd())
		{
			starState = StarState::End;		
			for (int i = 0; i < 10; i++)
			{
				randScale = MathF::GetRand(1.5f,2.0f);
				ParticleManager::GetInstance()->CreateCubeParticle(position, { randScale,randScale,randScale }, 10.0f, { 1.0f,1.0f,0.0f,1 });
			}
		}

		break;
	case Star::StarState::End:
	
		break;
	}

	rotation.y += (MathF::PIf * 2) * rotaSpeed * TimeManager::deltaTime;

	Obj3d::Update(*Camera::sCamera);
}

void Star::Draw()
{
	Obj3d::DrawMaterial();
}

void Star::HitEffect()
{
	//2回目は入らないように
	if (hit)return;

	saveScale = scale;

	saveStartPos = position;
	savePlayerPos = Player::Get()->position;

	hit = true;

	jumpUpTimer.Start();
	delayTimer.Reset();
	inholeTimer.Reset();

	starState = StarState::jumpUp;
}