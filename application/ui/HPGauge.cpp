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
	mFlontGauge.mPosition = mPos;
	mFlontGauge.mColor = (mFlontColor);

	mBackGauge.SetTexture(*TextureManager::GetTexture("white"));
	mBackGauge.SetAnchor({ 0,0.5f });
	mBackGauge.mPosition = mPos;
	mBackGauge.mColor = (mBackColor);

	mInner.SetTexture(*TextureManager::GetTexture("white"));
	mInner.SetAnchor({ 0,0.5f });
	mInner.mPosition = mPos;
	mInner.mColor = (mInnerColor);

	mFrameGauge.SetTexture(*TextureManager::GetTexture("white"));
	mFrameGauge.SetAnchor({ 0.5f,0.5f });
	Vector2 framePos = { mPos.x + GAUGE_MAX_SIZEX / 2,mPos.y };
	mFrameGauge.mPosition = { framePos };
	mFrameGauge.mColor = (mFrameColor);
	
	SetGaugeSize(mInitGaugeMax);
}

void Gauge::Update()
{
	mFlontTimer.Update();
	mBackTimer.Update();
	mDelayTimer.Update();

	//ゲージが変化した時
	if (mGaugeNum.DecreaseTrigger())
	{
		//タイマーを開始
		mBackTimer.Reset();
		mDelayTimer.Reset();
		mFlontTimer.Start();
	}
	if (mGaugeNum.IncreaseTrigger())
	{
		//タイマーを開始
		mBackTimer.Reset();
		mDelayTimer.Reset();
		mFlontTimer.Start();
	}
	//都合いい感じにタイマーを操作
	if (mFlontTimer.GetEnd())
	{
		if(mDelayTimer.GetStarted()==false)mDelayTimer.Start();
	}
	if (mDelayTimer.GetEnd())
	{
		if(mBackTimer.GetStarted()==false)mBackTimer.Start();
	}

	//ゲージを動かす
	GaugeMove();

	//更新
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

	mFlontStart = mFlontGauge.GetSize().x;
	mFlontEnd = mFlontGauge.GetSize().x;

	mBackStart = mBackGauge.GetSize().x;
	mBackEnd = mBackGauge.GetSize().x;
}

void Gauge::Addition(int32_t value)
{
	mGaugeNum.mCurrent += value;
	//下限と上限を超えないように
	mGaugeNum.mCurrent = Util::Clamp(mGaugeNum.mCurrent, 0, mInitGaugeMax);

	mFlontStart = mFlontGauge.GetSize().x;
	mFlontEnd = mGaugeSizeX * (mGaugeNum.mCurrent);

	mFlontStart = Util::Clamp(mFlontStart, 0.f, GAUGE_MAX_SIZEX);
	mFlontEnd = Util::Clamp(mFlontEnd, 0.f, GAUGE_MAX_SIZEX);

	mBackStart = mBackGauge.GetSize().x;
	mBackEnd = mGaugeSizeX * (mGaugeNum.mCurrent);

	mBackStart = Util::Clamp(mBackStart, 0.f, GAUGE_MAX_SIZEX);
	mBackEnd = Util::Clamp(mBackEnd, 0.f, GAUGE_MAX_SIZEX);
}

void Gauge::GaugeMove()
{
	//手前のゲージのイージング
	float gaugeX = 0;

	gaugeX = TEasing::lerp(mFlontStart, mFlontEnd, mFlontTimer.GetTimeRate());

	mFlontGauge.SetSize({ gaugeX,mGaugeSizeY });

	//後ろのゲージのイージング
	gaugeX = TEasing::lerp(mBackStart, mBackEnd, mBackTimer.GetTimeRate());

	mBackGauge.SetSize({ gaugeX,mGaugeSizeY });

	//表のゲージが裏のゲージより大きくなったら
	if (mBackGauge.GetSize().x < mFlontGauge.GetSize().x)
	{
		//裏のゲージの大きさを表ゲージに合わせる
		mBackGauge.SetSize({ mFlontGauge.GetSize().x,mBackGauge.GetSize().y});
	}
}
