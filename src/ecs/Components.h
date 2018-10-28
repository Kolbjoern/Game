#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

struct PositionComponent
{
	float x;
	float y;
};

struct MotionComponent
{
	sf::Vector2f velocity;
	sf::Vector2f direction;
	float speed;
	float friction;
};

struct GraphicsComponent
{
	float width;
	int shape;
	sf::Color color;
};

struct CollisionComponent
{
	float width;
};

struct AgeComponent
{
	float lifeTime;
	float lifeLived;
};

struct Action1Component
{
	int id;
	bool triggered;
	sf::Vector2f vector;
};
