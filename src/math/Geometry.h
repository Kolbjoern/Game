#pragma once

#include <SFML/System/Vector2.hpp>

enum Shape
{
	Circle = 0,
	Rectangle = 1
};

struct CircleShape
{
	sf::Vector2f position;
	float radius;
};

struct RectangleShape
{
	sf::Vector2f position;
	float width;
	float height;
};
/*
namespace Collision
{
	bool circleCircle(sf::Vector2f& c1Pos, float c1Radius, sf::Vector2f& c2Pos, float c2Radius);
	bool circleRect(sf::Vector2f& cPos, float cRadius, sf::Vector2f& rPos, float rWidth, float rHeight);
};*/
