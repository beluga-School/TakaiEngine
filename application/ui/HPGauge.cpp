#include "HPGauge.h"
#include "MathF.h"
#include "Util.h"

Gauge::Gauge(const Vector2& pos, const int32_t& separetnum)
{
	mPos = pos;
	mGaugeNum = separetnum;
	mInitGaugeMax = separetnum;
	Initialize();
}

void Gauge::Initialize()
{
	mFlontGauge.SetTexture(*TextureManager::GetTexture("white"));
	mFlontGauge.SetAnchor({ 0,0.5f });
	mFlontGauge.SetPos(mPos);
	mFlontGauge.SetColor(mFlontColor);

	mBackGauge.SetTexture(*TextureManager::GetTexture("white"));
	mBackGauge.SetAnchor({ 0,0.5f });
	mBackGauge.SetPos(mPos);
	mBackGauge.SetColor(mBackColor);

	mInner.SetTexture(*TextureManager::GetTexture("white"));
	mInner.SetAnchor({ 0,0.5f });
	mInner.SetPos(mPos);
	mInner.SetColor(mInnerColor);

	mFrameGauge.SetTexture(*TextureManager::GetTexture("white"));
	mFrameGauge.SetAnchor({ 0.5f,0.5f });
	Vector2 framePos = { mPos.x + GAUGE_MAX_SIZEX / 2,mPos.y };
	mFrameGauge.SetPos(framePos);
	mFrameGauge.SetColor(mFrameColor);
	
	SetGaugeSize(mInitGaugeMax);
}

void Gauge::Update()
{
	mFlontTimer.Update();
	mBackTimer.Update();
	mDelayTimer.Update();

	//�Q�[�W���ω�������
	if (mGaugeNum.DecreaseTrigger())
	{
		//�^�C�}�[���J�n
		mBackTimer.Reset();
		mDelayTimer.Reset();
		mFlontTimer.Start();
	}
	if (mGaugeNum.IncreaseTrigger())
	{
		//�^�C�}�[���J�n
		mBackTimer.Reset();
		mDelayTimer.Reset();
		mFlontTimer.Start();
	}
	//�s�����������Ƀ^�C�}�[�𑀍�
	if (mFlontTimer.GetEnd())
	{
		if(mDelayTimer.GetStarted()==false)mDelayTimer.Start();
	}
	if (mDelayTimer.GetEnd())
	{
		if(mBackTimer.GetStarted()==false)mBackTimer.Start();
	}

	//�Q�[�W�𓮂���
	GaugeMove();

	//�X�V
	mBackGauge.Update();
	mFlontGauge.Update();
	mInner.Update();
	mFrameGauge.Update();
}

void Gauge::Draw()
{
	if (mIsVisilve == false)return;

	mFrameGauge.Draw();
	mInner.Draw();
	mBackGauge.Draw();
	mFlontGauge.Draw();
}

void Gauge::SetGaugeSize(int32_t separetNum, bool maxChange)
{
	mGaugeNum = separetNum;
	if (maxChange)
	{
		mInitGaugeMax = separetNum;
	}

	mGaugeNum.mCurrent = Util::Clamp(mGaugeNum.mCurrent, 0, mInitGaugeMax);

	if (separetNum <= 0)
	{
		mGaugeSizeX = 0;
		return;
	}
	mGaugeSizeX = GAUGE_MAX_SIZEX / separetNum;

	mFlontGauge.SetSize({ mGaugeSizeX * separetNum,mGaugeSizeY});
	mBackGauge.SetSize({ mGaugeSizeX * separetNum,mGaugeSizeY});
	mInner.SetSize({ mGaugeSizeX * separetNum,mGaugeSizeY});
	mFrameGauge.SetSize({
		(mGaugeSizeX * separetNum) * mFrameSize.x,
		mGaugeSizeY * mFrameSize.y });

	mFlontStart = mFlontGauge.mSize.x;
	mFlontEnd = mFlontGauge.mSize.x;

	mBackStart = mBackGauge.mSize.x;
	mBackEnd = mBackGauge.mSize.x;
}

void Gauge::Addition(int32_t value)
{
	mGaugeNum.mCurrent += value;
	//�����Ə���𒴂��Ȃ��悤��
	mGaugeNum.mCurrent = Util::Clamp(mGaugeNum.mCurrent, 0, mInitGaugeMax);

	mFlontStart = mFlontGauge.mSize.x;
	mFlontEnd = mGaugeSizeX * (mGaugeNum.mCurrent);

	mFlontStart = Util::Clamp(mFlontStart, 0.f, GAUGE_MAX_SIZEX);
	mFlontEnd = Util::Clamp(mFlontEnd, 0.f, GAUGE_MAX_SIZEX);

	mBackStart = mBackGauge.mSize.x;
	mBackEnd = mGaugeSizeX * (mGaugeNum.mCurrent);

	mBackStart = Util::Clamp(mBackStart, 0.f, GAUGE_MAX_SIZEX);
	mBackEnd = Util::Clamp(mBackEnd, 0.f, GAUGE_MAX_SIZEX);
}

void Gauge::GaugeMove()
{
	//��O�̃Q�[�W�̃C�[�W���O
	float gaugeX = 0;

	gaugeX = TEasing::lerp(mFlontStart, mFlontEnd, mFlontTimer.GetTimeRate());

	mFlontGauge.SetSize({ gaugeX,mGaugeSizeY });

	//���̃Q�[�W�̃C�[�W���O
	gaugeX = TEasing::lerp(mBackStart, mBackEnd, mBackTimer.GetTimeRate());

	mBackGauge.SetSize({ gaugeX,mGaugeSizeY });

	//�\�̃Q�[�W�����̃Q�[�W���傫���Ȃ�����
	if (mBackGauge.mSize.x < mFlontGauge.mSize.x)
	{
		//���̃Q�[�W�̑傫����\�Q�[�W�ɍ��킹��
		mBackGauge.mSize.x = mFlontGauge.mSize.x;
	}
}
