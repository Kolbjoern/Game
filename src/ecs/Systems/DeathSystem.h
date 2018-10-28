#pragma once

#include <vector>
#include <unordered_map>

#include "../Components.h"

namespace DeathSystem
{
	void update(float deltaTime, std::vector<int>& deathRow,
				std::unordered_map<int, AgeComponent>& age,
				std::unordered_map<int, HealthComponent>& hea);
};
