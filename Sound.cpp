#include "Sound.h"
#include "Result.h"
#include <combaseapi.h>

void SoundManager::Initialize()
{
	//result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

SoundData SoundManager::SoundLoadWave(const char* filename)
{
	//�@�t�@�C���I�[�v��
	std::ifstream file;

	//wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(filename, std::ios_base::binary);

	//�t�@�C���I�[�v�������s�����猟�o����
	assert(file.is_open());

	//�Awav�f�[�^�ǂݍ���
	//RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	//�t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0){
		assert(0);
	}
	//�^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	//Format�`�����N�̓ǂݍ���
	FormatChunk format = {};
	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0){
		assert(0);
	}

	//�`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);

		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	//Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//�B�t�@�C���N���[�Y
	//�t�@�C�������
	file.close();
	
	//�C�ǂݍ��񂾃f�[�^�����^�[��
	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

void SoundManager::SoundPlayWave(const SoundData& soundData)
{
	//�g�`�t�H�[�}�b�g����SourceVoice�̐���
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//�g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();

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
	xAudio2.Reset();
}