#pragma once
#include "Obj.h"

struct ConstBufferLine
{
	Matrix4 viewproj;	//ビュープロ行列
	Matrix4 world;		//ワールド行列
	Float4 color;		//色
};

class LineObject : public Obj3d
{
public:
	ConstBuffer<ConstBufferLine> constBufferLine;
	
	void Initialize();
	void Update(const Camera& camera);
	void Draw();
};

