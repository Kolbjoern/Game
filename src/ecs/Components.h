#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "../math/Geometry.h"

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
	float height;
	Shape shape;
	sf::Color color;
};

struct CollisionComponent
{
	float width;
	float height;
	Shape shape;
};

struct AgeComponent
{
	float lifeTime;
	float lifeLived;
};

struct HealthComponent
{
	int maxHealth;
	int currentHealth;
};

struct Action1Component
{
	int id;
	bool triggered;
	float reloadTimer;
	sf::Vector2f vector;
};
