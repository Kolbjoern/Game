#include "PhysicsSystem.h"

#include <SFML/System/Vector2.hpp>

#include "../../utils/VectorMath.h"

namespace PhysicsSystem
{
	void update(float deltaTime,
				std::vector<int> &deathRow,
				std::unordered_map<int, PositionComponent> &pos,
				std::unordered_map<int, AccelerationComponent> &acc,
				std::unordered_map<int, VelocityComponent> &vel,
				std::unordered_map<int, CollisionComponent> &col)
	{
		int objId;
		for (std::pair<int, VelocityComponent> velocity : vel)
		{
			objId = velocity.first;

			if (acc.find(objId) != acc.end())
			{
				// acceleration affects the objects velocity
				vel[objId].x = velocity.second.x + acc[objId].dirX * acc[objId].value * deltaTime;
				vel[objId].y = velocity.second.y + acc[objId].dirY * acc[objId].value * deltaTime;

				// remove acceleration
				acc[objId].dirX = 0.0f;
				acc[objId].dirY = 0.0f;
			}

			if (pos.find(objId) != pos.end())
			{
				float newX = pos[objId].x + velocity.second.x;
				float newY = pos[objId].y + velocity.second.y;
			
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
						if (collisionDepth > 0)
						{
							deathRow.push_back(objId);
							//noramlize depth vector
							sf::Vector2f depthVec = VectorMath::normalize(distVec) * collisionDepth;
							
							pos[objId].x += depthVec.x;
							pos[objId].y += depthVec.y;

							vel[objId].x *= -0.25f;
							vel[objId].y *= -0.25f;
						}
					}
				}

				// slow down
				vel[objId].x *= 0.94f;
				vel[objId].y *= 0.94f;
			}
		}
	}
}
