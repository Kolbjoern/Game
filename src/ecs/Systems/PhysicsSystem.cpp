#include "PhysicsSystem.h"

#include <SFML/System/Vector2.hpp>

#include "../../math/VectorMath.h"
#include "../../math/Collision.h"

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

						bool collided = false;

						if (col[objId].shape == Shape::Circle && col[i].shape == Shape::Circle)
						{
							CircleShape c1;
							c1.position = sf::Vector2f(newX, newY);
							c1.radius = col[objId].width/2.0f;

							CircleShape c2;
							c2.position = sf::Vector2f(pos[i].x, pos[i].y);
							c2.radius = col[i].width/2.0f;

							collided = Collision::circleCircle(c1, c2);
						}
						else if (col[objId].shape == Shape::Circle && col[i].shape == Shape::Rectangle)
						{
							CircleShape c;
							c.position = sf::Vector2f(newX, newY);
							c.radius = col[objId].width/2.0f;

							RectangleShape r;
							r.position = sf::Vector2f(pos[i].x, pos[i].y);
							r.width = col[i].width;
							r.height = col[i].height;

							collided = Collision::circleRect(c, r);
						}
						else if (col[objId].shape == Shape::Rectangle && col[i].shape == Shape::Circle)
						{
							RectangleShape r;
							r.position = sf::Vector2f(newX, newY);
							r.width = col[objId].width;
							r.height = col[objId].height;

							CircleShape c;
							c.position = sf::Vector2f(pos[i].x, pos[i].y);
							c.radius = col[i].width/2.0f;

							collided = Collision::circleRect(c, r);
						}

						if (collided)
						{	
							if (hea.find(objId) != hea.end())
							{
								// inflict damage to object
								hea[objId].currentHealth -= 1; // some basic damage value for now
							}

							if (hea.find(i) != hea.end())
							{
								hea[i].currentHealth -= 1;
							}

							pos[objId].x += motion.second.velocity.x * -1.0f;
							pos[objId].y += motion.second.velocity.y * -1.0f;

							mot[objId].velocity.x *= -0.2f;
							mot[objId].velocity.y *= -0.2f;
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
