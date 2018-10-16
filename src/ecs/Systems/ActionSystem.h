#pragma once

#include <vector>
#include <unordered_map>

#include "../Components.h"

namespace ActionSystem
{
	void update(float deltaTime, int& counter,
				std::unordered_map<int, Action1Component>& act1, 
				std::unordered_map<int, PositionComponent>& pos,
				std::unordered_map<int, MotionComponent>& mot,
				std::unordered_map<int, AgeComponent>& age,
				std::unordered_map<int, GraphicsComponent>& gra);
};
