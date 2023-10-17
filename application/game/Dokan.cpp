#include "Dokan.h"
#include "Input.h"
#include "Stage.h"
#include "SceneChange.h"
#include <GameUIManager.h>
#include <InstantDrawer.h>

void Dokan::LoadResource()
{
	TextureManager::Load("Resources\\ui\\LSHIFTdownUI.png", "LSHIFTdownUI");
}

void Dokan::Initialize()
{
	SetModel(ModelManager::GetModel("dokan"));
	SetTexture(TextureManager::GetTexture("white"));
}

void Dokan::Update()
{
	mPreMoveTimer.Update();
	mMainMoveTimer.Update();

	TargetEnter();

	UIUpdate();

	Obj3d::Update(*Camera::sCamera);
}

void Dokan::Draw()
{
	DrawMaterial();

	InstantDrawer::DrawGraph3D({
		position.x,
		position.y + 5,
		position.z,
		}, mUISize.x, mUISize.y, "LSHIFTdownUI");
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

		//移動先の情報をステージへ移す
		StageChanger::Get()->saveNextDokanInfo.stageName = nextDokanInfo.stageName;
		StageChanger::Get()->saveNextDokanInfo.id = nextDokanInfo.id;

		//UIを動かす
		//GameUIManager::Get()->Move(UIMove::END,"StageTitle");
	}
}

void Dokan::TargetEnter()
{
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
			std::vector<std::string> split = Util::SplitString(trigerName, "_");

			std::string stagename = "stage_";

			for (auto str : split)
			{
				//冠詞の"stage"でないなら保存
				if (!Util::IsNumber(str) && str != "stage")
				{
					stagename += str;
				}
			}

			StageChanger::Get()->ChangeLevel(*LevelLoader::Get()->GetData(stagename));

			//諸々を元に戻す
			mDokanState = DokanState::None;

			//位置を固定
			mTarget->position.y = box.position.y - scale.y / 2;

			break;
		}
	}
}

void Dokan::UIUpdate()
{
	popUIRangeSphere.center = position;
	
	mUIPopUpTimer.Update();
	mUIPopOutTimer.Update();

	switch (mUIState)
	{
	case Dokan::UIState::None:
		break;
	case Dokan::UIState::PopUp:
		mUISize.x = TEasing::OutBack(0.0f, 1.0f, mUIPopUpTimer.GetTimeRate());
		mUISize.y = TEasing::OutBack(0.0f, 1.0f, mUIPopUpTimer.GetTimeRate());
		
		if (mUIPopUpTimer.GetEnd())
		{
			mUIState = UIState::Exist;
		}

		break;
	case Dokan::UIState::Exist:
		break;
	case Dokan::UIState::PopOut:
		mUISize.x = TEasing::InBack(1.0f, 0.0f, mUIPopOutTimer.GetTimeRate());
		mUISize.y = TEasing::InBack(1.0f, 0.0f, mUIPopOutTimer.GetTimeRate());

		if (mUIPopOutTimer.GetEnd())
		{
			mUIState = UIState::None;
		}

		break;
	}
}

void Dokan::PopUpUI()
{
	//未出現状態以外で入ったらスキップ
	if (mUIState != UIState::None)return;

	mUIPopUpTimer.Start();
	mUIState = UIState::PopUp;
}

void Dokan::PopOutUI()
{
	//出現状態以外で入ったらスキップ
	if (mUIState != UIState::Exist)return;

	mUIPopOutTimer.Start();
	mUIState = UIState::PopOut;
}
