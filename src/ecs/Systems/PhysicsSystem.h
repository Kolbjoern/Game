#pragma once

#include <unordered_map>

#include "../Components.h"

namespace PhysicsSystem
{
	void update(std::unordered_map<int, PositionComp> &pos, 
				std::unordered_map<int, VelocityComp> &vel);
};
