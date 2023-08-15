#pragma once
#include "EventBlock.h"
#include "TEasing.h"
#include "Mob.h"

class MoveBlock : public EventBlock
{
public:
	MoveBlock() : EventBlock()
	{
		SetTag(TagTable::MoveBlock);
	};

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void HitEffect()override;

	/// <summary>
	/// “–‚½‚Á‚Ä‚¢‚é‚É“Ç‚Ü‚ê‚éŠÖ”
	/// </summary>
	/// <param name="mob"></param>
	void OnCollide(Mob* mob);

	/// <summary>
	/// “–‚½‚Á‚Ä‚¢‚È‚¢‚É“Ç‚Ü‚ê‚éŠÖ”
	/// </summary>
	/// <param name="mob"></param>
	void NotCollide(Mob* mob);

	Vector3 startpos{};
	Vector3 endpos{};

	std::string id = "";

private:
	TEasing::easeTimer moveTimer = 2.0f;
};

