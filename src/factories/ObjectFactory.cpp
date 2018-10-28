#include "ObjectFactory.h"

namespace ObjectFactory
{
	void createTestObstacle(int objId,
		std::unordered_map<int, PositionComponent>& pos,
		std::unordered_map<int, GraphicsComponent>& gra,
		std::unordered_map<int, CollisionComponent>& col,
		std::unordered_map<int, HealthComponent>& hea)
	{
		PositionComponent p;
		p.x = 600.0f;
		p.y = 350.0f;
		pos.emplace(objId, p);

		GraphicsComponent g;
		g.width = 200.0f;
		g.shape = 0;//CIRCLE
		g.color = sf::Color(255.0f, 255.0f, 255.0f);
		gra.emplace(objId, g);

		CollisionComponent c;
		c.width = 200.0f;
		col.emplace(objId, c);

		HealthComponent h;
		h.maxHealth = 3;
		h.currentHealth = 3;
		hea.emplace(objId, h);
	}

	void createTile(int objId,
		std::unordered_map<int, PositionComponent>& pos,
		std::unordered_map<int, GraphicsComponent>& gra)
	{
		PositionComponent p;
		p.x = 0.0f;
		p.y = 0.0f;
		pos.emplace(objId, p);

		GraphicsComponent g;
		g.width = 8.0f;
		g.shape = 1;//QUAD
		g.color = sf::Color(255.0f, 255.0f, 255.0f);
		gra.emplace(objId, g);
	}

	void createPlayer(int objId,
		std::unordered_map<int, PositionComponent>& pos,
		std::unordered_map<int, MotionComponent>& mot,
		std::unordered_map<int, GraphicsComponent>& gra,
		std::unordered_map<int, CollisionComponent>& col,
		std::unordered_map<int, Action1Component>& act1)
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
		g.shape = 0;//CIRCLE
		g.color = sf::Color(255.0f, 50.0f, 60.0f);
		gra.emplace(objId, g);

		CollisionComponent c;
		c.width = 50.0f;
		col.emplace(objId, c);

		Action1Component a1;
		a1.id = 11;
		a1.triggered = false;
		a1.vector = sf::Vector2f(0.0f, 0.0f);
		act1.emplace(objId, a1);
	}

	void createProjectile(int objId,
		std::unordered_map<int, PositionComponent>& pos,
		std::unordered_map<int, MotionComponent>& mot,
		std::unordered_map<int, AgeComponent>& age,
		std::unordered_map<int, GraphicsComponent>& gra)
	{
		PositionComponent p;
		p.x = 0.0f;
		p.y = 0.0f;
		pos.emplace(objId, p);

		MotionComponent m;
		m.velocity = sf::Vector2f(0.0f, 0.0f);
		m.direction = sf::Vector2f(0.0f, 0.0f);
		m.speed = 500.0f;
		m.friction = 0.0f;
		mot.emplace(objId, m);

		AgeComponent a;
		a.lifeTime = 10.0f;
		a.lifeLived = 0.0f;
		age.emplace(objId, a);

		GraphicsComponent g;
		g.width = 15.0f;
		g.shape = 0;//CIRCLE
		g.color = sf::Color(10.0f, 255.0f, 50.0f);
		gra.emplace(objId, g);
	}
};
