#include "PhysicsSystem.h"

#include <SFML/System/Vector2.hpp>

#include "../../utils/VectorMath.h"

namespace PhysicsSystem
{
	void update(float deltaTime,
				std::unordered_map<int, PositionComponent>& pos,
				std::unordered_map<int, MotionComponent>& mot,
				std::unordered_map<int, CollisionComponent>& col,
				std::unordered_map<int, HealthComponent>& hea)
	{
		int objId;
		for (std::pair<int, MotionComponent> motion : mot)
		{
			objId = motion.first;

			// add acceleration direction
			mot[objId].velocity.x = motion.second.velocity.x + motion.second.direction.x * motion.second.speed * deltaTime;
			mot[objId].velocity.y = motion.second.velocity.y + motion.second.direction.y * motion.second.speed * deltaTime;

			// reset direction
			mot[objId].direction = sf::Vector2f(0.0f, 0.0f);

			if (pos.find(objId) != pos.end())
			{
				float newX = pos[objId].x + motion.second.velocity.x;
				float newY = pos[objId].y + motion.second.velocity.y;
			
				// VALIDATE AND HANDLE COLLISION
				pos[objId].x = newX;
				pos[objId].y = newY;

				if (col.find(objId) != col.end())
				{
					for (std::pair<int, CollisionComponent> collision : col)
					{
						int i = collision.first;

						if (i == objId)
							continue;

						const float MIN_DISTANCE = col[objId].width/2.0f + col[i].width/2.0f;
						sf::Vector2f centerA = sf::Vector2f(newX, newY);
						sf::Vector2f centerB = sf::Vector2f(pos[i].x, pos[i].y);

						sf::Vector2f distVec = centerA - centerB;

						float dist = VectorMath::getMagnitude(distVec);
						float collisionDepth = MIN_DISTANCE - dist;
						if (collisionDepth > 0) // COLLISION
						{
							if (hea.find(objId) != hea.end())
							{
								// inflict damage to object 1
								hea[objId].currentHealth -= 1; // some basic damage value for now
							}

							if (hea.find(i) != hea.end())
							{
								hea[i].currentHealth -= 1;
							}

							//noramlize depth vector
							sf::Vector2f depthVec = VectorMath::normalize(distVec) * collisionDepth;
							
							pos[objId].x += depthVec.x;
							pos[objId].y += depthVec.y;

							mot[objId].velocity.x *= -0.25f;
							mot[objId].velocity.y *= -0.25f;
						}
					}
				}

				// slow down
				mot[objId].velocity.x *= (1 - motion.second.friction);
				mot[objId].velocity.y *= (1 - motion.second.friction);
			}
		}
	}
}
