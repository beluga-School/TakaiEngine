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
	//�����ǂݍ���
	SoundData SoundLoadWave(const char* filename);
	//�����Đ�
	void SoundPlayWave(const SoundData& soundData,const bool& loopFlag = false,const float& volume = 0.1f);
	//�������
	void SoundUnload(SoundData* soundData);
	//xAudio2�̉��
	void End();

	static SoundManager* GetInstance() {
		static SoundManager instance;
		return &instance;
	};
private:
	Microsoft::WRL::ComPtr<IXAudio2> mXAudio2;
	IXAudio2MasteringVoice* mMasterVoice = nullptr;
};