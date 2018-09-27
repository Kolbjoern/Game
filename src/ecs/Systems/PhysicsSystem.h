#pragma once

#include <unordered_map>

#include "../Components.h"

namespace PhysicsSystem
{
	void update(float deltaTime, 
				std::unordered_map<int, bool> &ent,
				std::unordered_map<int, PositionComponent> &pos,
				std::unordered_map<int, AccelerationComponent> &acc,
				std::unordered_map<int, VelocityComponent> &vel,
				std::unordered_map<int, CollisionComponent> &col);
};
