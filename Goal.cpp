#include "Goal.h"
#include "MathF.h"

void Goal::Initialize()
{
	SetModel(ModelManager::GetModel("GoalPaul"));
	SetTexture(TextureManager::GetTexture("white"));

	goalBlock.Initialize();
	mInitFragPos = true;

	goalBlock.parent = this;
}

void Goal::Update()
{
	Obj3d::Update(*Camera::sCamera);
	goalBlock.Update();

	goalGUI.Begin({ 500,100 }, { 100,100 });
	ImGui::Text("goalFrag.position %f %f %f", goalBlock.position.x, goalBlock.position.y, goalBlock.position.z);
	ImGui::Text("goalPaul.position %f %f %f", position.x, position.y,position.z);
	goalGUI.End();
}

void Goal::Draw()
{
	Obj3d::Draw();
	goalBlock.Draw();
}

void Goal::HitEffect()
{
	goalBlock.HitEffect();
}
