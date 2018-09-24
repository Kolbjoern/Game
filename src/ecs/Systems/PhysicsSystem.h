#pragma once

#include <unordered_map>

#include "../Components.h"

namespace PhysicsSystem
{
	void update(float deltaTime,
				std::unordered_map<int, PositionComp> &pos,
				std::unordered_map<int, AccelerationComp> &acc,
				std::unordered_map<int, VelocityComp> &vel,
				std::unordered_map<int, CollisionComp> &col);
};
