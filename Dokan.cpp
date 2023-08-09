#include "Dokan.h"
#include "Input.h"
#include "Stage.h"
#include "SceneChange.h"

void Dokan::Initialize()
{
	SetModel(ModelManager::GetModel("dokan"));
	SetTexture(TextureManager::GetTexture("white"));
}

void Dokan::Update()
{
	mPreMoveTimer.Update();
	mMainMoveTimer.Update();

	//保持したターゲットがいるなら
	if (mTarget != nullptr)
	{
		switch (mDokanState)
		{
		case Dokan::DokanState::None:
			break;
		case Dokan::DokanState::PreMove:
			
			//ターゲットの初期位置を土管の真上に移動
			mStartPos = { box.position.x ,box.position.y + box.scale.y / 2,box.position.z };
			mTarget->position = TEasing::InQuad(mSavePos, mStartPos, mPreMoveTimer.GetTimeRate());

			if (mPreMoveTimer.GetEnd())
			{
				mDokanState = DokanState::MainMove;
				mMainMoveTimer.Start();
			}
			
			break;
		case Dokan::DokanState::MainMove:
			//土管の原点まで移動させる
			mTarget->position.y = TEasing::InQuad(mStartPos.y, box.position.y - scale.y / 2, mMainMoveTimer.GetTimeRate());
			
			if (mMainMoveTimer.GetEnd())
			{
				mDokanState = DokanState::End;
			}

			break;
		case Dokan::DokanState::End:
			
			//eventtrigerに記載されたステージへ移動する
			StageChanger::Get()->ChangeLevel(*LevelLoader::Get()->GetData(trigerName));

			//諸々を元に戻す
			mDokanState = DokanState::None;

			//位置を固定
			mTarget->position.y = box.position.y - scale.y / 2;

			break;
		}
	}

	Obj3d::Update(*Camera::sCamera);
}

void Dokan::Draw()
{
	DrawMaterial();
}

void Dokan::HitEffect()
{
	
}

void Dokan::HitEffect(Mob* target_)
{
	//すでに開始しているならスキップ
	if (mDokanState != DokanState::None)return;

	//ボタン入力があったら
	if (Input::Keyboard::TriggerKey(DIK_LSHIFT))
	{
		mPreMoveTimer.Start();
		mDokanState = DokanState::PreMove;
		mTarget = target_;
		//ターゲットの当たり判定をなくす
		mTarget->SetNoCollsion(true);
		mTarget->SetNoGravity(true);
		mTarget->SetNoMove(true);
		mSavePos = target_->box.position;

		StageChanger::Get()->oldDokanInfo.stageName = dokanInfo.stageName;
		StageChanger::Get()->oldDokanInfo.id = dokanInfo.id;
	}
}
