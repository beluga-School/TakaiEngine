#pragma once
#include "Obj.h"

struct ConstBufferLine
{
	Matrix4 viewproj;	//�r���[�v���s��
	Matrix4 world;		//���[���h�s��
	Float4 color;		//�F
};

class LineObject : public Obj3d
{
public:
	ConstBuffer<ConstBufferLine> constBufferLine;
	
	void Initialize();
	void Update(const Camera& camera);
	void Draw();
};

