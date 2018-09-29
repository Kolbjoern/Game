#pragma once

#include <SFML/Graphics/Color.hpp>

struct PositionComponent
{
	float x;
	float y;
};

struct AccelerationComponent
{
	float value;
	float dirX;
	float dirY;
};

struct VelocityComponent
{
	float x;
	float y;
};

struct GraphicsComponent
{
	float width;
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
