#include "DeathSystem.h"

namespace DeathSystem
{
	void update(float deltaTime, std::vector<int>& deathRow,
				std::unordered_map<int, AgeComponent>& age,
				std::unordered_map<int, HealthComponent>& hea)
	{
		int oId;
		for (std::pair<int, AgeComponent> currentAge : age)
		{
			oId = currentAge.first;

			if (currentAge.second.lifeLived > currentAge.second.lifeTime)
				deathRow.push_back(oId);
			else
				age[oId].lifeLived += deltaTime;
		}

		for (std::pair<int, HealthComponent> health : hea)
		{
			oId = health.first;

			if (health.second.currentHealth <= 0)
				deathRow.push_back(oId); // have to check if already exist
		}
	}
};
