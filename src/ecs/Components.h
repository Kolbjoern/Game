#pragma once

#include <SFML/Graphics/Color.hpp>

struct PositionComp
{
	float x;
	float y;
};

struct AccelerationComp
{
	float value;
	float dirX;
	float dirY;
};

struct VelocityComp
{
	float x;
	float y;
};

struct GraphicsComp
{
	float width;
	sf::Color color;
};

struct CollisionComp
{
	float width;
};
