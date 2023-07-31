#include "Sound.h"
#include "Result.h"
#include <combaseapi.h>

std::map<std::string, SoundData> SoundManager::sSounds;

void SoundManager::Initialize()
{
	//result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	sResult = XAudio2Create(&mXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	sResult = mXAudio2->CreateMasteringVoice(&mMasterVoice);
}

void SoundManager::Load(const char* filename, const std::string& handle)
{
	//①ファイルオープン
	std::ifstream file;

	//wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);

	//ファイルオープンが失敗したら検出する
	assert(file.is_open());

	//②wavデータ読み込み
	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0){
		assert(0);
	}
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	//Formatチャンクの読み込み
	FormatChunk format = {};
	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0){
		assert(0);
	}

	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);

		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//③ファイルクローズ
	//ファイルを閉じる
	file.close();
	
	//④読み込んだデータをリターン
	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	sSounds[handle] = soundData;
}

void SoundManager::Play(const std::string &handle, const bool& loopFlag, const float& volume)
{
	SoundData* soundData = SoundManager::GetSound(handle);

	//波形フォーマットからSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	sResult = mXAudio2->CreateSourceVoice(&pSourceVoice, &soundData->wfex);
	assert(SUCCEEDED(sResult));

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData->pBuffer;
	buf.AudioBytes = soundData->bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	if (loopFlag)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	//音量を調整
	pSourceVoice->SetVolume(volume);

	//波形データの再生
	sResult = pSourceVoice->SubmitSourceBuffer(&buf);
	sResult = pSourceVoice->Start();
}

void SoundManager::SoundUnload(SoundData* soundData)
{
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void SoundManager::End()
{
	mXAudio2.Reset();
}

SoundData *SoundManager::GetSound(const std::string& handle)
{
	return &sSounds[handle];
}
