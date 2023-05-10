#include "AssimpLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <StringUtil.h>

using namespace std;

string ToUTF8(const wstring& value)
{
	auto length = WideCharToMultiByte(
		CP_UTF8, 0U, value.data(), -1, nullptr, 0, nullptr,nullptr);

	auto buffer = new char[length];

	WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, buffer, length, nullptr, nullptr);

	string result(buffer);
	delete[] buffer;
	buffer = nullptr;

	return result;
}

std::wstring ToWideString(const std::string& str)
{
	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

	wstring wstr;
	wstr.resize(num1);

	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);
	return wstr;
}

bool AssimpLoader::Load(ImportSettings settings)
{
	//書いてないならスキップ
	if (settings.filename == nullptr)
	{
		return false;
	}

	auto& meshs = settings.meshes;

	auto path = ToUTF8(settings.filename);

	Assimp::Importer importer;

	const aiScene *scene = 
		importer.ReadFile(path,
			aiProcess_Triangulate |
			aiProcess_PreTransformVertices | 
			aiProcess_CalcTangentSpace |
			aiProcess_GenSmoothNormals |
			aiProcess_GenUVCoords |
			aiProcess_RemoveRedundantMaterials |
			aiProcess_OptimizeMeshes 
			//aiProcess_ConvertToLeftHanded
			);
	if (scene == nullptr)
	{
		//エラーがでたら表示
		OutputDebugStringA(importer.GetErrorString());
		assert(0);
	}

	//読み込んだデータをMeshに保存
	meshs.clear();
	meshs.resize(scene->mNumMeshes);
	for (size_t i = 0; i < meshs.size(); i++)
	{
		const auto pMesh = scene->mMeshes[i];
		LoadMesh(meshs[i], pMesh, settings.inverseU, settings.inverseV);
		const auto pMaterial = scene->mMaterials[i];
		LoadTexture(settings.filename, meshs[i],pMaterial);
	}

	scene = nullptr;

	return true;
}

void AssimpLoader::LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV)
{
	aiVector3D zero(0.f, 0.f, 0.f);
	aiColor4D zeroColor(0.f, 0.f, 0.f,0.f);

	dst.vertices.resize(src->mNumVertices);

	for (unsigned int i = 0u; i < src->mNumVertices; ++i)
	{
		aiVector3D* position = &(src->mVertices[i]);
		aiVector3D* normal = &(src->mNormals[i]);
		aiVector3D* uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero;
		//aiVector3D* tangent = (src->HasTangentsAndBitangents()) ? &(src->mTangents[i]) : &zero;
		//aiColor4D* color = (src->HasVertexColors(0)) ? &(src->mColors[0][i]) : &zeroColor;
	
		if (inverseU)
		{
			uv->x = 1 - uv->x;
		}
		if (inverseV)
		{
			uv->y = 1 - uv->y;
		}

		Vertex vertex = {};
		vertex.pos = DirectX::XMFLOAT3(position->x, position->y, position->z);
		vertex.normal = DirectX::XMFLOAT3(normal->x, normal->y, normal->z);
	
		dst.vertices[i] = vertex;
	}

	dst.indices.resize(src->mNumFaces * 3);

	for (unsigned int i = 0u; i < src->mNumFaces; ++i)
	{
		const aiFace& face = src->mFaces[i];

		dst.indices[i * 3 + 0] = face.mIndices[0];
		dst.indices[i * 3 + 1] = face.mIndices[1];
		dst.indices[i * 3 + 2] = face.mIndices[2];
	}
}

void AssimpLoader::LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src)
{
	aiString path;
	if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
	{
		//相対パスのテクスチャを、ファイルの場所とくっつける
		auto directoryPath = "Resources/" + ConvertWCharToString(filename) + "/";
		auto wdir = ConvertStringToWChar(directoryPath);
		
		auto file = std::string(path.C_Str());
		dst.diffuseMap = wdir + ToWideString(file);
	}
	else
	{
		dst.diffuseMap.clear();
	}
}
