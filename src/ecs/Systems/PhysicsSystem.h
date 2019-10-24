#pragma once

#include <vector>
#include <unordered_map>

#include "../Components.h"

namespace PhysicsSystem
{
	void update(float deltaTime,
				std::unordered_map<int, PositionComponent>& pos,
				std::unordered_map<int, MotionComponent>& mot,
				std::unordered_map<int, ShapeComponent>& sha,
				std::unordered_map<int, HealthComponent>& hea);
};
