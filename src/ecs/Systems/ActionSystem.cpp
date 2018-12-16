#include "ActionSystem.h"

#include "../../data/Actions.h"
#include "../../factories/ObjectFactory.h"
#include "../../utils/VectorMath.h"

namespace ActionSystem
{
	void update(float deltaTime, int& counter,
				std::unordered_map<int, Action1Component>& act1, 
				std::unordered_map<int, PositionComponent>& pos,
				std::unordered_map<int, MotionComponent>& mot,
				std::unordered_map<int, AgeComponent>& age,
				std::unordered_map<int, GraphicsComponent>& gra,
				std::unordered_map<int, CollisionComponent>& col,
				std::unordered_map<int, HealthComponent>& hea)
	{
		int objId;
		for (std::pair<int, Action1Component> action : act1)
		{
			objId = action.first;

			// SHOOT BASIC
			if (action.second.id == 11)
			{
				if (pos.find(objId) != pos.end())
				{
					act1[objId].reloadTimer += deltaTime;
					if (action.second.triggered && (action.second.reloadTimer > shootBasic.fireRate))
					{
						act1[objId].reloadTimer = 0.0f;

						sf::Vector2f gunslingerPos = sf::Vector2f(pos[objId].x, pos[objId].y);
						sf::Vector2f fireDirection = action.second.vector - gunslingerPos;
						sf::Vector2f normalized = VectorMath::normalize(fireDirection);

						int projectileId = counter++;

						ObjectFactory::createProjectile(projectileId, pos, mot, age, gra, col, hea);
						pos[projectileId].x = gunslingerPos.x + normalized.x*col[objId].width;
						pos[projectileId].y = gunslingerPos.y + normalized.y*col[objId].width;
						mot[projectileId].direction = normalized;
						mot[projectileId].speed = shootBasic.bulletSpeed;
						age[projectileId].lifeTime = shootBasic.lifeTime;
						gra[projectileId].width = shootBasic.bulletWidth;
					}
					act1[objId].triggered = false;
				}
			}
		}
	}
};
