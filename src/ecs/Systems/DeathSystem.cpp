#include "DeathSystem.h"

namespace DeathSystem
{
	void update(float deltaTime, std::vector<int> &deathRow, std::unordered_map<int, AgeComponent> &age)
	{
		int objId;
		for (std::pair<int, AgeComponent> currentAge : age)
		{
			objId = currentAge.first;

			if (currentAge.second.lifeLived > currentAge.second.lifeTime)
				deathRow.push_back(objId);
			else
				age[objId].lifeLived += deltaTime;
		}
	}
};