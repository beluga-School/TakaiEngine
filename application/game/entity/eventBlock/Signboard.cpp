#include "Signboard.h"
#include "MathF.h"

void Signboard::LoadResource()
{
	ModelManager::LoadModel("signboard", "signboard");
	ModelManager::LoadModel("plate", "plate", true);
}

void Signboard::Initialize()
{
	Obj3d::Initialize();
	SetTexture(TextureManager::GetTexture("white"));
	SetModel(ModelManager::GetModel("signboard"));

	pictureObj.Initialize();
	pictureObj.SetModel(ModelManager::GetModel("plate"));
}

void Signboard::Update()
{
	pictureObj.position = position + -matWorld.ExtractAxisZ() * 0.6f;
	pictureObj.position += {0, 6.5f, 0};
	pictureObj.rotation = rotation;

	Obj3d::Update(*Camera::sCamera);
	pictureObj.Update(*Camera::sCamera);
}

void Signboard::Draw()
{
	Obj3d::DrawMaterial();
	pictureObj.Draw();
}

void Signboard::HitEffect()
{

}

void Signboard::SetPicture(const std::string& texHandle_)
{
	pictureObj.SetTexture(TextureManager::GetTexture(texHandle_));

	int32_t texWidth = (int32_t)TextureManager::GetTexture(texHandle_)->GetMetaData()->width;
	int32_t texHeight = (int32_t)TextureManager::GetTexture(texHandle_)->GetMetaData()->height;

	int32_t bigger = max(texWidth, texHeight);
	int32_t smaller = min(texWidth, texHeight);

	bigger /= smaller;
	smaller = 1;

	//横の方が大きいなら
	if (texWidth > texHeight)
	{
		pictureObj.scale = { (float)bigger * width,(float)smaller * height,1 };
	}
	//縦の方が大きいor同じなら
	else
	{
		pictureObj.scale = { (float)smaller * width,(float)bigger * height,1 };
	}
}
