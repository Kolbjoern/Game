#pragma once

#include <unordered_map>

#include "../Components.h"

class PhysicsSystem
{
	public:
		void update(std::unordered_map<int, PositionComp> &pos, 
					std::unordered_map<int, VelocityComp> &vel);
};
