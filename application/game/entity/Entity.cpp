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

void Entity::Register()
{
	CollideManager::Get()->allCols.push_back(this);
}