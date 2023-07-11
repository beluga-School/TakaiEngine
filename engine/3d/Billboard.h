#pragma once
#include "Obj.h"
#include "ViewProjection.h"
#include "Matrix4.h"

class Billboard : public Obj3d
{
public:
	Billboard();
	void Initialize();

	void Update(const Camera &camera);
};

class BillboardY : public Obj3d
{
public:
	BillboardY();
	void Initialize();

	void Update(const Camera& camera);
};