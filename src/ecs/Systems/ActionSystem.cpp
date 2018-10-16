#include "ActionSystem.h"

#include "../../factories/ObjectFactory.h"
#include "../../utils/VectorMath.h"

namespace ActionSystem
{
	void update(float deltaTime, int& counter,
				std::unordered_map<int, Action1Component>& act1, 
				std::unordered_map<int, PositionComponent>& pos,
				std::unordered_map<int, MotionComponent>& mot,
				std::unordered_map<int, AgeComponent>& age,
				std::unordered_map<int, GraphicsComponent>& gra)
	{
		int objId;
		for (std::pair<int, Action1Component> action : act1)
		{
			objId = action.first;

			// SHOOT BASIC
			if (action.second.id == 11)
			{
				if (pos.find(objId) != pos.end() && action.second.triggered)
				{
					sf::Vector2f gunslingerPos = sf::Vector2f(pos[objId].x, pos[objId].y);
					sf::Vector2f fireDirection = action.second.vector - gunslingerPos;
					sf::Vector2f normalized = VectorMath::normalize(fireDirection);

					int projectileId = counter++;

					ObjectFactory::createProjectile(projectileId, pos, mot, age, gra);
					pos[projectileId].x = gunslingerPos.x;
					pos[projectileId].y = gunslingerPos.y;
					mot[projectileId].direction = normalized;

					act1[objId].triggered = false;
				}
			}
		}
	}
};
