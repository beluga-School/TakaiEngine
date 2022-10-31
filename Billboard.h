#pragma once
#include "Obj.h"
#include "ViewProjection.h"

class Billboard : public Obj3d
{
public:
	Billboard(View* view,bool yBillboardMode);

	View* view = nullptr;

	void Update(XMMATRIX& matProjection);
	void Draw(Texture* texture);
public:
	bool yBillboardMode = false;
};