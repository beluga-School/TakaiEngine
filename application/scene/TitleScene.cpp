#include "TitleScene.h"
#include "Input.h"
#include "SceneChange.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "ClearDrawScreen.h"
#include "Util.h"
#include "MathF.h"
#include "TimeManager.h"
#include "Sound.h"

void TitleScene::LoadResource()
{
	TextureManager::Load("Resources\\title_logo.png", "title_logo");
	TextureManager::Load("Resources\\title_logo\\U.png", "title_u");
	TextureManager::Load("Resources\\title_logo\\L.png", "title_l");
	TextureManager::Load("Resources\\title_logo\\T.png", "title_t");
	TextureManager::Load("Resources\\title_logo\\R.png", "title_r");
	TextureManager::Load("Resources\\title_logo\\A.png", "title_a");
	TextureManager::Load("Resources\\title_logo\\MARIMO.png", "title_marimo");
	TextureManager::Load("Resources\\title_logo\\6400.png", "title_6400");
	TextureManager::Load("Resources\\space.png", "space");
	TextureManager::Load("Resources\\Start_A.png", "Start_A");
	
	TextureManager::Load("Resources\\hexagon_big.png", "hexagon_big");

	SoundManager::Load("Resources\\sound\\shock.wav", "shock");
	SoundManager::Load("Resources\\sound\\pop.wav", "pop");

	//titleUI.SetTexture(*TextureManager::GetTexture("title_logo"));
	//titleUI.SetPos({ Util::WIN_WIDTH / 2, Util::WIN_HEIGHT / 2 });
}

void TitleScene::Initialize()
{
	mStart.SetTexture(*TextureManager::GetTexture("space"));
	mStart.mPosition = { Util::WIN_WIDTH / 2,Util::WIN_HEIGHT - 100 };

	sceneID = "Title";

	string[1].sprite.SetTexture(*TextureManager::GetTexture("title_l"));
	string[0].sprite.SetTexture(*TextureManager::GetTexture("title_u"));
	string[2].sprite.SetTexture(*TextureManager::GetTexture("title_t"));
	string[3].sprite.SetTexture(*TextureManager::GetTexture("title_r"));
	string[4].sprite.SetTexture(*TextureManager::GetTexture("title_a"));
	string[5].sprite.SetTexture(*TextureManager::GetTexture("title_marimo"));
	string[6].sprite.SetTexture(*TextureManager::GetTexture("title_6400"));

	//初期設定
	//位置ずらし
	const float sideX = 300;
	//1文字ごとの間隔
	const float invideX = 600 / 5;
	for (int32_t i = 0; i < 7; i++)
	{
		string[i].sprite.mPosition = { -114514, -114514 };
		string[i].position.x = (invideX * i) + sideX;
		string[i].startY = -100.f;
		string[i].endY = Util::WIN_HEIGHT / 2 - 50.f;
	}

	string[5].position.x = Util::WIN_WIDTH / 2;
	string[5].startY = Util::WIN_HEIGHT + 100.f;
	string[5].endY = Util::WIN_HEIGHT / 2 + 100.f;
	string[5].timer.mMaxTime = 0.3f;

	string[6].position.x = sideX + invideX * 5.5f;
	string[6].startY = Util::WIN_HEIGHT + 100.f;
	string[6].endY = Util::WIN_HEIGHT / 2;
	string[6].timer.mMaxTime = 0.3f;

	for (int32_t i = 0; i < 7; i++)
	{
		string[i].savePos = {
			string[i].position.x,
			string[i].endY
		};
	}
	whiteOut.mPosition = { Util::WIN_WIDTH / 2,Util::WIN_HEIGHT / 2 };
	whiteOut.SetTexture(*TextureManager::GetTexture("white"));
	whiteOut.SetSize({ Util::WIN_WIDTH,Util::WIN_HEIGHT});
	whiteOut.mColor.f4.w = 0.0f;

	backHexagon.mPosition = { Util::WIN_WIDTH / 2,Util::WIN_HEIGHT / 2 };
	backHexagon.SetSize({ Util::WIN_WIDTH + 100,Util::WIN_HEIGHT + 100 });
	backHexagon.SetTexture(*TextureManager::GetTexture("hexagon_big"));
	backHexagon.mColor = {0, 1, 0, 1};
}

void TitleScene::Update()
{
	if (Input::Keyboard::TriggerKey(DIK_SPACE))
	{
		//デバッグ用 演出がどのタイミングでもスキップできるように
		if (effectState != EffectState::None)
		{
			SceneChange::Get()->Start();
		}
		//演出開始
		if (effectState == EffectState::None)
		{
			for (int32_t i = 0; i < 7; i++)
			{
				string[i].timer.Reset();
			}
			string[0].timer.Start();

			effectState = EffectState::Appearance;
		}
		//演出が終わってspaceを押したらゲーム開始
		if (effectState == EffectState::End)
		{
			SceneChange::Get()->Start();
		}
	}
	if (SceneChange::Get()->IsBlackOut())
	{
		SceneManager::Get()->ChangeScene<GameScene>();
	}

	shineTimer.Update();
	shineCoolTimer.Update();
	shineEndTimer.Update();
	hexagonScaleTimer.Update();

	switch (effectState)
	{
	case TitleScene::EffectState::None:
		break;
	case TitleScene::EffectState::Appearance:
		for (int32_t i = 0; i < 7; i++)
		{
			//着地時に揺らす
			if (string[i].timer.GetNowEnd())
			{
				if (i < 6)
				{
					SoundManager::Get()->Play("shock");
				}
				else
				{
					SoundManager::Get()->Play("pop");
				}
				if (i < 5)
				{
					SetShake(10, 0.1f);
				}
				else
				{
					SetShake(50, 0.1f);
				}
			}
		}

		//最後のタイマーが終わったら
		if (string[6].timer.GetEnd())
		{
			//次に進む
			effectState = EffectState::ScreenShine;
			shineTimer.Start();
		}

		break;

	case TitleScene::EffectState::ScreenShine:
		
		whiteOut.mColor.f4.w = TEasing::InQuad(
			0,1,
			shineTimer.GetTimeRate()
		);

		//タイマーを都合よく動かす
		if (shineTimer.GetEnd())
		{
			if (shineCoolTimer.GetStarted() == false)
			{
				shineCoolTimer.Start();
			}
		}
		if (shineCoolTimer.GetEnd())
		{
			if (shineEndTimer.GetStarted() == false)
			{
				shineEndTimer.Start();
				hexagonScaleTimer.Start();
				effectState = EffectState::End;
			}
		}

		break;
	case TitleScene::EffectState::End:
		whiteOut.mColor.f4.w = TEasing::InQuad(
			1, 0,
			shineEndTimer.GetTimeRate()
		);

		if (hexagonScaleTimer.GetEnd())
		{
			hexagonScaleTimer.Start();
		}
		
		backHexagon.mRotation += 50.0f * TimeManager::deltaTime;
		backHexagon.SetSize({
			TEasing::InQuad(Util::WIN_WIDTH + 400,Util::WIN_WIDTH + 200,hexagonScaleTimer.GetTimeRate()),
			TEasing::InQuad(Util::WIN_WIDTH + 400,Util::WIN_WIDTH + 200,hexagonScaleTimer.GetTimeRate())
			});


		for (int32_t i = 0; i < 7; i++)
		{
			string[i].sprite.SetSize(
				{
				TEasing::InQuad(string[i].sprite.GetInitSize().x * 1.1f,string[i].sprite.GetInitSize().x,hexagonScaleTimer.GetTimeRate()),
				TEasing::InQuad(string[i].sprite.GetInitSize().y * 1.1f,string[i].sprite.GetInitSize().y,hexagonScaleTimer.GetTimeRate())
				});
		}

		break;
	}

	for (int32_t i = 0; i < 7; i++)
	{
		//次のタイマーに進める処理
		//0なら飛ばす
		if (i == 0)continue;
		if (string[i - 1].timer.GetEnd())
		{
			if (string[i].timer.GetStarted() == false)
			{
				string[i].timer.Start();
			}
		}
	}

	for (int32_t i = 0; i < 7; i++)
	{
		string[i].Update();
	}

	whiteOut.Update();
	backHexagon.Update();
	mStart.Update();

	ShakeUpdate();

	Camera::sCamera->UpdatematView();
	
	//titleUI.Update();
}

void TitleScene::Draw()
{
	SpriteCommonBeginDraw();
	//titleUI.Draw();
	if (effectState == EffectState::End)
	{
		backHexagon.Draw();
	}

	for (int32_t i = 0; i < 7; i++)
	{
		string[i].Draw();
	}

	if (effectState == EffectState::None || effectState == EffectState::End)
	{
		mStart.Draw();
	}

	whiteOut.Draw();
}

void TitleScene::End()
{

}

void TitleScene::ShakeUpdate()
{
	//位置を初期に戻す
	for (int32_t i = 0; i < 7; i++)
	{
		string[i].position = string[i].savePos;
	}

	shakeTimer.Update();

	//タイマーが動いてるなら実行
	if (shakeTimer.GetRun())
	{
		//位置をランダム化
		Vector2 randomVec = {
			MathF::GetRand(-1,1),
			MathF::GetRand(-1,1)
		};

		//保存したパワーを元に揺らす
		shake = randomVec * shakePower;
		for (int32_t i = 0; i < 7; i++)
		{
			string[i].position += shake;
		}
	}
}

void TitleScene::SetShake(float power, float time)
{
	shakePower = power;

	shakeTimer.mMaxTime = time;
	shakeTimer.Start();
}

void TitleScene::TitleString::Update()
{
	timer.Update();

	position.y = TEasing::OutQuad(startY, endY, timer.GetTimeRate());

	sprite.mPosition = position;
	sprite.Update();
}

void TitleScene::TitleString::Draw()
{
	sprite.Draw();
}
