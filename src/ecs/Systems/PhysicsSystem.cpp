#include "PhysicsSystem.h"

void PhysicsSystem::update(	std::unordered_map<int, PositionComp> &pos, 
							std::unordered_map<int, VelocityComp> &vel)
{
	for (int i = 0; i < pos.size(); i++)
	{
		if (pos.find(i) != pos.end() && vel.find(i) != vel.end())
		{
			float newX = pos[i].x + vel[i].x;
			float newY = pos[i].y + vel[i].y;
			
			// VALIDATE AND HANDLE COLLISION
			pos[i].x = newX;
			pos[i].y = newY;

			// slow down
			vel[i].x *= 0.94f;
			vel[i].y *= 0.94f;
		}
	}
}
