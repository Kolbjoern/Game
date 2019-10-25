#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>

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

struct ShapeComponent
{
	std::string data;
	std::string currentData;
	int width;
	sf::Color color;
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
