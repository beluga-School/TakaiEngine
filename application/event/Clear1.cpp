#include "Clear1.h"
#include "StageChanger.h"
#include "InstantDrawer.h"

void ClearEvent::Start()
{
	spriteTimer.Start();
	changeTimer.Reset();
}

void ClearEvent::End()
{
	StageChanger::Get()->ChangeLevel(*LevelLoader::Get()->GetData("stage_stageselect"));
	StageChanger::Get()->playerData.dokanPriority = true;
}

void ClearEvent::Initialize()
{

}

void ClearEvent::Update()
{
	spriteTimer.Update();
	changeTimer.Update();
	if (spriteTimer.GetEnd()) {
		if (!changeTimer.GetStarted()) {
			changeTimer.Start();
		}
	}

	spritePos.y = TEasing::OutQuad(-100, Util::WIN_HEIGHT / 2, spriteTimer.GetTimeRate());
}

void ClearEvent::Draw()
{
	InstantDrawer::DrawGraph(spritePos.x, spritePos.y,2.0f,2.0f,"clearString");
}

bool ClearEvent::EndFlag()
{
	return changeTimer.GetEnd();
}
