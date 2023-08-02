#include "Entity.h"
#include "CollideManager.h"

bool Entity::CheckTag(TagTable check)
{
	for (auto &tag : taglist)
	{
		if (tag == check)
		{
			return true;
		}
	}
	return false;
}

bool Entity::SetTag(TagTable check)
{
	for (auto itr = taglist.begin(); itr != taglist.end(); itr++)
	{
		if (*itr == check)
		{
			return false;
		}
	}
	taglist.push_back(check);
	return true;
}

bool Entity::DeleteTag(TagTable check)
{
	for (auto itr = taglist.begin();itr != taglist.end();itr++)
	{
		if (*itr == check)
		{
			taglist.erase(itr);
			return true;
		}
	}

	return false;
}

void Entity::Register()
{
	CollideManager::Get()->allCols.push_back(this);
}