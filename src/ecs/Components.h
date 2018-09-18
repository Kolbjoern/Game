#pragma once

#include <SFML/Graphics/Color.hpp>

struct PositionComp
{
	float x;
	float y;
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
