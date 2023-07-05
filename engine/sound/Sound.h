#pragma once
#include "DirectXInit.h"
#include <xaudio2.h>
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
	//音声読み込み
	SoundData SoundLoadWave(const char* filename);
	//音声再生
	void SoundPlayWave(const SoundData& soundData,const bool& loopFlag = false,const float& volume = 0.1f);
	//音声解放
	void SoundUnload(SoundData* soundData);
	//xAudio2の解放
	void End();

	static SoundManager* GetInstance() {
		static SoundManager instance;
		return &instance;
	};
private:
	Microsoft::WRL::ComPtr<IXAudio2> mXAudio2;
	IXAudio2MasteringVoice* mMasterVoice = nullptr;
};