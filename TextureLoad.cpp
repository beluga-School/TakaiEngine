#include "TextureLoad.h"
#include "Result.h"


void TextureLoad::WIC(const wchar_t* file_pass)
{
	//WICテクスチャのロード
	result = LoadFromWICFile(
		file_pass,	//ここに読み込みたいファイルのパスを入れる
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	ScratchImage mipChain{};
	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);
}