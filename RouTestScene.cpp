#include "RouTestScene.h"
#include "InstantDrawer.h"
#include "Input.h"

void RouTestScene::LoadResource()
{
}

void RouTestScene::Initialize()
{

}

void RouTestScene::Update()
{
	center.x = Util::WIN_WIDTH / 2;
	center.y = Util::WIN_HEIGHT / 2;
	mousePoint = Input::Mouse::GetPos();

	if (Input::Mouse::Down(Click::LEFT))
	{
		waxs.push_back(center);
		waxs.back().velocity = (mousePoint - center).GetNormalized();
	}
	waxs.remove_if([](Wax& wax) {
		return wax.isDead;
		});
	for (auto& wax : waxs)
	{
		wax.Update();
	}
}

void RouTestScene::Draw()
{
	InstantDrawer::DrawBox(center.x, center.y, 64, 64, { 0,0,1,1 });
	InstantDrawer::DrawBox(mousePoint.x, mousePoint.y, 64, 64, { 1,0,0,1 });

	for (auto& wax : waxs)
	{
		wax.Draw();
	}
}

void RouTestScene::End()
{
}