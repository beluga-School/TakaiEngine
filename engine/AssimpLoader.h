#pragma once
#define NOMINMAX
#include <DirectXInit.h>
#include <string>
#include <vector>
#include "Model.h"

struct aiMesh;
struct aiMaterial;

struct ImportSettings
{
	const wchar_t* mFilename = nullptr;	//�t�@�C���p�X
	std::vector<Mesh>& mMeshes;			//�o�͐�̃��b�V���z��
	bool mInverseU = false;				//U���W�𔽓]�����邩
	bool mInverseV = false;				//V���W�𔽓]�����邩
};

class AssimpLoader
{
public:
	bool Load(const ImportSettings& setting);	//���f���̃��[�h

private:
	void LoadMesh(Mesh& dst, const aiMesh* src, const bool& inverseU, const bool& inverseV);
	void LoadTexture(const wchar_t* filename,Mesh& dst,const aiMaterial* src);

};

