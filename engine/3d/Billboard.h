#pragma once
#include "Obj.h"
#include "ViewProjection.h"

class Billboard : public Obj3d
{
public:
	Billboard(Camera* view,bool yBillboardMode);

	Camera* mView = nullptr;

	void Update(DirectX::XMMATRIX& matProjection);
	void Draw();
public:
	bool mYbillboardMode = false;
	DirectX::XMMATRIX mMatBillboardY;
};