#pragma once
#include "TEasing.h"
#include "Status.h"
#include "HPGauge.h"
#include "Mob.h"
#include "ImguiManager.h"

class Player final : public Mob
{
public:
	void LoadResource();

	void Initialize();
	void Update();
	void Draw();

	void DrawUI();

	void Reset();

	static Player* Get()
	{
		static Player instance;
		return &instance;
	}

	//�v���C���[�̉�]���Ǘ�����ϐ�
	float mVerticalRad = 0;
	float mHorizontalRad = 0;

	//�O�ړ�(��ړ�)
	Vector3 preMove = { 0,0,0 };

	//�G�Ƃ̓����蔻��p�X�t�B�A
	Sphere mEncountCol;

	//�ĂԂ�1��W�����v����
	void Jump();

	//����HP���擾
	int32_t GetNowHP();

	/// <summary>
	/// �_���[�W���󂯂鏈��
	/// </summary>
	void DamageEffect(int32_t damage);

	//�ݒu���ꂽ�t���[���Ȃ痧��
	bool mDokanApparrance = false;

	//�y�ǂ���o�����铮��
	void ApparranceMove(const Vector3& dokanPos, const Vector3& dokanScale);

	bool GetApparanceEnd();

private:
	//������ӂ̏�ԊǗ��͓��ꂵ�������AApparrance�������ɂ���ƐF�X�s�s�������肻��
	//�o�������̂������
	enum class PlayerState
	{
		Normal,
		Apparrance,
		Debug,
	}playerState = PlayerState::Normal;

	//�v���C���[�̃��[�h��ς���(0,�ʏ�,1,�f�o�b�O���[�h)
	void ChangeMode(const PlayerState& pState);

	//hp�̍ő�l�@�X�e�[�^�X�̍ő�l�͊O����ς�����悤�ɂ�����
	int32_t MAX_HP = 8;

	Status hp = 8;

	Gauge hpGauge = {{
			Util::WIN_WIDTH / 16,
			Util::WIN_HEIGHT - Util::WIN_HEIGHT / 12
		},8};

	Player() : Mob()
	{
		SetTag(TagTable::Player);
	};
	~Player(){};

	void SideMoveUpdate();
	void ColUpdate();
	void RotaUpdate();
	void Fly();

	void DamageUpdate();

	bool IsMove();

	//HP�����̒l�ɏ��������� �ő�l���傫���ꍇ�A�ő�l������������
	void HPOverFlow(int32_t value);

	//���G����
	TEasing::easeTimer mutekiTimer = 2.0f;
	//�_���[�W�󂯂���̓_��
	//0.1�b���Ƃɕ`���������
	TEasing::easeTimer flashTimer = 0.05f;

	///---���ړ�
	//�ړ����x
	float mSpeed = 7.5f;

	//�ő�l�ɂȂ�܂ł̉�������
	TEasing::easeTimer accelerationTimer = 0.25f;

	//�ő�l���猸��܂ł̌�������
	TEasing::easeTimer decelerationTimer = 0.25f;

	//�ړ��I��������̑��x
	float mSaveSpeed = 0.0f;

	//�ő呬�x
	const float MAX_SPEED = 7.5f;

	//�_�b�V�����̍ő呬�x
	const float MAX_DASH_SPEED = 15.0f;

	Vector3 oldMoveVec = {0,0,0};

	//�����x�N�g���ۑ�
	Vector3 mCenterVec = {0,0,0};
	Vector3 mSideVec = {0,0,0};

	//�G�Ƃ̓����蔻��p�X�t�B�A�̕`��
	Obj3d colDrawer;

	//���W���֌W
	//�X�^�[�̎擾��
	Status starCorrectNum = 0;

	TEasing::easeTimer UIDelayTimer = 0.5f;

	TEasing::easeTimer apparranceTimer = 1.0f;

	Vector3 saveDokanPos{};
	Vector3 saveDokanScale{};
};