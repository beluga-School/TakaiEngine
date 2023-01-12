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
	unsigned int bufferSize;
};

class SoundManager
{
public:
	void Initialize();
	//音声読み込み
	SoundData SoundLoadWave(const char* filename);
	//音声再生
	void SoundPlayWave(const SoundData& soundData);
	//音声解放
	void SoundUnload(SoundData* soundData);
	//xAudio2の解放
	void End();

	static SoundManager* GetInstance() {
		static SoundManager instance;
		return &instance;
	};
private:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
};