#pragma once
#include "DirectXInit.h"
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>

struct ChunkHeader
{
	char id[4];
	int32_t size;
};

struct RiffHeader
{
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk
{
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

struct SoundData
{
	WAVEFORMATEX wfex;
	BYTE* pBuffer;
	uint32_t bufferSize;
};

class SoundManager
{
public:
	void Initialize();
	//‰¹º“Ç‚İ‚İ
	SoundData SoundLoadWave(const char* filename);
	//‰¹ºÄ¶
	void SoundPlayWave(const SoundData& soundData,const bool& loopFlag = false,const float& volume = 0.1f);
	//‰¹º‰ğ•ú
	void SoundUnload(SoundData* soundData);
	//xAudio2‚Ì‰ğ•ú
	void End();

	static SoundManager* GetInstance() {
		static SoundManager instance;
		return &instance;
	};
private:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice = nullptr;
};