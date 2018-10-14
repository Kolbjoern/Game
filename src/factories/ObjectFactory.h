#pragma once

#include <unordered_map>

#include "../ecs/Components.h"

namespace ObjectFactory
{
	void createTestObstacle(int objId,
		std::unordered_map<int, PositionComponent>& pos,
		std::unordered_map<int, GraphicsComponent>& gra,
		std::unordered_map<int, CollisionComponent>& col);

	void createPlayer(int objId,
		std::unordered_map<int, PositionComponent>& pos,
		std::unordered_map<int, MotionComponent>& mot,
		std::unordered_map<int, GraphicsComponent>& gra,
		std::unordered_map<int, CollisionComponent>& col);
};
