#include "ObjectFactory.h"

namespace ObjectFactory
{
	void createTestObstacle(int objId,
		std::unordered_map<int, PositionComponent>& pos,
		std::unordered_map<int, GraphicsComponent>& gra,
		std::unordered_map<int, CollisionComponent>& col)
	{
		PositionComponent p;
		p.x = 600.0f;
		p.y = 350.0f;
		pos.emplace(objId, p);

		GraphicsComponent g;
		g.width = 200.0f;
		g.color = sf::Color(255.0f, 255.0f, 255.0f);
		gra.emplace(objId, g);

		CollisionComponent c;
		c.width = 200.0f;
		col.emplace(objId, c);
	}

	void createPlayer(int objId,
		std::unordered_map<int, PositionComponent>& pos,
		std::unordered_map<int, MotionComponent>& mot,
		std::unordered_map<int, GraphicsComponent>& gra,
		std::unordered_map<int, CollisionComponent>& col)
	{
		PositionComponent p;
		p.x = 0.0f;
		p.y = 0.0f;
		pos.emplace(objId, p);

		MotionComponent m;
		m.velocity = sf::Vector2f(0.0f, 0.0f);
		m.direction = sf::Vector2f(0.0f, 0.0f);
		m.speed = 25.0f;
		m.friction = 0.06f;
		mot.emplace(objId, m);

		GraphicsComponent g;
		g.width = 50.0f;
		g.color = sf::Color(255.0f, 50.0f, 60.0f);
		gra.emplace(objId, g);

		CollisionComponent c;
		c.width = 50.0f;
		col.emplace(objId, c);
	}
};
