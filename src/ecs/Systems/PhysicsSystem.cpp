#include "PhysicsSystem.h"

namespace PhysicsSystem
{
	void update(float deltaTime,
				std::unordered_map<int, PositionComp> &pos,
				std::unordered_map<int, AccelerationComp> &acc,
				std::unordered_map<int, VelocityComp> &vel)
	{
		int objId;
		for (std::pair<int, VelocityComp> velocity : vel)
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

				// slow down
				vel[objId].x *= 0.94f;
				vel[objId].y *= 0.94f;
			}
		}
	}
}
