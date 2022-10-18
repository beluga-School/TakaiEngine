#pragma once
#include "Obj.h"
#include "ViewProjection.h"

class Billboard : public Obj3d
{
public:
	Billboard(View* view);

	View* view = nullptr;

	void Update(XMMATRIX& matProjection);
	void Draw(Texture* texture);
};