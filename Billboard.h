#pragma once
#include "Obj.h"
#include "ViewProjection.h"

class Billboard : public Obj3d
{
public:
	Billboard(Camera* view,bool yBillboardMode);

	Camera* view = nullptr;

	void Update(XMMATRIX& matProjection);
	void Draw();
public:
	bool yBillboardMode = false;
	XMMATRIX matBillboardY;
};