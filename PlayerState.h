#pragma once
#include <string>
class Player;

class PlayerState
{
public:
	virtual void Update(Player* player) = 0;
	virtual std::string showState() = 0;
	virtual ~PlayerState() {};
};

class PlayerNormal : public PlayerState
{
public:
	void Update(Player *player);
	std::string showState();
};

class PlayerDash : public PlayerState
{
public:
	void Update(Player* player);
	std::string showState();
};