#pragma once
#include "DirectXInit.h"
#include <xaudio2.h>
#include <fstream>
#include <map>

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
	static void Load(const char* filename,const std::string& handle);
	//音声再生
	void Play(const std::string& handle,const bool& loopFlag = false,const float& volume = 0.1f);
	//音声解放
	void SoundUnload(SoundData* soundData);
	//xAudio2の解放
	void End();

	static SoundData* GetSound(const std::string& handle);

	static SoundManager* Get() {
		static SoundManager instance;
		return &instance;
	};
private:

	static std::map<std::string, SoundData> sSounds;

	Microsoft::WRL::ComPtr<IXAudio2> mXAudio2;
	IXAudio2MasteringVoice* mMasterVoice = nullptr;
};