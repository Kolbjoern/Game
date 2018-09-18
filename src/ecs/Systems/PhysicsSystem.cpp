#include "PhysicsSystem.h"

namespace PhysicsSystem
{
	void update(std::unordered_map<int, PositionComp> &pos, 
				std::unordered_map<int, VelocityComp> &vel)
	{
		int objId;
		for (std::pair<int, PositionComp> position : pos)
		{
			objId = position.first;
			if (vel.find(objId) != vel.end())
			{
				float newX = position.second.x + vel[objId].x;
				float newY = position.second.y + vel[objId].y;
			
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
