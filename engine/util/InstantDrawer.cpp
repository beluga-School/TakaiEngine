#include "InstantDrawer.h"
#include "ClearDrawScreen.h"
#include "MathF.h"
#include <minmax.h>
//ビルボードがプレイヤーカメラ依存の設計になってるので、違和感消しのためにイベント中はビルボード表示をなくす用にする
//現在カメラ依存に修正できたら消していい
#include <EventManager.h>

void InstantDrawer::DrawInit()
{
	for (auto& instant : Get()->sSprites)
	{
		instant.isUse = false;
		instant.mColor = {1,1,1,1};
		instant.mRotation = 0;
	}
	for (auto& instant : Get()->sBillboards)
	{
		instant.isUse = false;
	}
	for (auto& instant : Get()->sObjs)
	{
		instant.isUse = false;
	}
}

void InstantDrawer::DrawBox(const float& x, const float& y, const float& width, const float& height, const Color& color, const Anchor& anchor)
{
	for (auto& instant : Get()->sSprites)
	{
		if (!instant.isUse)
		{
			instant.isUse = true;
			instant.SetTexture(*TextureManager::GetTexture("white"));
			instant.mPosition = { x,y };
			instant.SetSize({ width, height });
			instant.mColor = color;
			switch (anchor)
			{
			case InstantDrawer::Anchor::LEFT:
				instant.SetAnchor({ 0,0.5f });
				break;
			case InstantDrawer::Anchor::CENTER:
				instant.SetAnchor({ 0.5f,0.5f });
				break;
			case InstantDrawer::Anchor::RIGHT:
				instant.SetAnchor({ 1.0f,0.5f });
				break;
			case InstantDrawer::Anchor::UP:
				instant.SetAnchor({ 0.5f,0 });
				break;
			case InstantDrawer::Anchor::DOWN:
				instant.SetAnchor({ 0.5f,1.0f });
				break;
			}
			break;
		}
	}
}

void InstantDrawer::DrawGraph(const float& x, const float& y, float sizerateX, float sizerateY, const std::string& handle, const Anchor& anchor)
{
	//ハンドルが空なら描画をキャンセル
	if (handle == "")return;

	for (auto& instant : Get()->sSprites)
	{
		if (!instant.isUse)
		{
			instant.isUse = true;
			instant.SetTexture(*TextureManager::GetTexture(handle));
			instant.mPosition = { x,y };
			instant.SetSizeRate({ sizerateX, sizerateY });
			switch (anchor)
			{
			case InstantDrawer::Anchor::LEFT:
				instant.SetAnchor({ 0,0.5f });
				break;
			case InstantDrawer::Anchor::CENTER:
				instant.SetAnchor({ 0.5f,0.5f });
				break;
			case InstantDrawer::Anchor::RIGHT:
				instant.SetAnchor({ 1.0f,0.5f });
				break;
			case InstantDrawer::Anchor::UP:
				instant.SetAnchor({ 0.5f,0 });
				break;
			case InstantDrawer::Anchor::DOWN:
				instant.SetAnchor({ 0.5f,1.0f });
				break;
			}
			break;
		}
	}
}

void InstantDrawer::DrawRotaGraph(const float& x, const float& y, float sizerateX, float sizerateY,
	float rotation, const std::string& handle, Color color, const Anchor& anchor)
{
	//ハンドルが空なら描画をキャンセル
	if (handle == "")return;

	for (auto& instant : Get()->sSprites)
	{
		if (!instant.isUse)
		{
			instant.isUse = true;
			instant.SetTexture(*TextureManager::GetTexture(handle));
			instant.mPosition = { x,y };
			instant.mRotation = rotation;
			instant.mColor = color;
			instant.SetSizeRate({ sizerateX, sizerateY });
			switch (anchor)
			{
			case InstantDrawer::Anchor::LEFT:
				instant.SetAnchor({ 0,0.5f });
				break;
			case InstantDrawer::Anchor::CENTER:
				instant.SetAnchor({ 0.5f,0.5f });
				break;
			case InstantDrawer::Anchor::RIGHT:
				instant.SetAnchor({ 1.0f,0.5f });
				break;
			case InstantDrawer::Anchor::UP:
				instant.SetAnchor({ 0.5f,0 });
				break;
			case InstantDrawer::Anchor::DOWN:
				instant.SetAnchor({ 0.5f,1.0f });
				break;
			}
			break;
		}
	}
}

void InstantDrawer::DrawGraph3D(const Vector3& pos, float width, float height, const std::string& handle)
{
	//includeで説明した通りイベント実行中なら消す
	if (EventManager::Get()->GetLineEvent() != nullptr) return;

	for (auto& instant : Get()->sBillboards)
	{
		if (!instant.isUse)
		{
			instant.isUse = true;
			instant.SetTexture(TextureManager::GetTexture(handle));
			instant.position = pos;

			int32_t texWidth = (int32_t)TextureManager::GetTexture(handle)->GetMetaData()->width;
			int32_t texHeight = (int32_t)TextureManager::GetTexture(handle)->GetMetaData()->height;

			int32_t bigger = max(texWidth, texHeight);
			int32_t smaller = min(texWidth, texHeight);

			bigger /= smaller;
			smaller = 1;

			//横の方が大きいなら
			if (texWidth > texHeight)
			{
				instant.scale = { (float)bigger * width,(float)smaller * height,1 };
			}
			//縦の方が大きいor同じなら
			else
			{
				instant.scale = { (float)smaller * width,(float)bigger * height,1 };
			}
			break;
		}
	}
}

void InstantDrawer::DrawBox3D(const Cube& cube)
{
	//includeで説明した通りイベント実行中なら消す
	if (EventManager::Get()->GetLineEvent() != nullptr) return;

	for (auto& instant : Get()->sObjs)
	{
		if (!instant.isUse)
		{
			instant.isUse = true;
			instant.Initialize();
			instant.SetTexture(TextureManager::GetTexture("white"));
			instant.position = cube.position;
			instant.rotation = cube.rotation;
			instant.scale = cube.scale;
			break;
		}
	}
}

void InstantDrawer::AllUpdate()
{
	for (auto& sprite : Get()->sSprites)
	{
		if (sprite.isUse)
		{
			sprite.Update();
		}
	}
	for (auto& billboard : Get()->sBillboards)
	{
		if (billboard.isUse)
		{
			billboard.Update(*Camera::sCamera);
		}
	}
	for (auto& obj : Get()->sObjs)
	{
		if (obj.isUse)
		{
			obj.Update(*Camera::sCamera);
		}
	}
}

void InstantDrawer::AllDraw2D()
{
	SpriteCommonBeginDraw();

	std::list<InstantSprite>* hoge = &Get()->sSprites;

	for (auto &sprite : *hoge)
	{
		if (sprite.isUse)
		{
			sprite.Draw();
		}
	}
}

void InstantDrawer::AllDraw3D()
{
	BasicObjectPreDraw("Billboard",false);
	for (auto& billboard : Get()->sBillboards)
	{
		if (billboard.isUse)
		{
			billboard.Draw();
		}
	}
	
	BasicObjectPreDraw("GroundToon");
	for (auto& obj : Get()->sObjs)
	{
		if (obj.isUse)
		{
			obj.Draw();
		}
	}
}

void InstantDrawer::PreCreate()
{
	for (int32_t i = 0; i < SP_MAX; i++)
	{
		Get()->sSprites.emplace_back();
	}
	for (int32_t i = 0; i < BILL_MAX; i++)
	{
		Get()->sBillboards.emplace_back();
	}
	for (int32_t i = 0; i < OBJ_MAX; i++)
	{
		Get()->sObjs.emplace_back();
	}
}
