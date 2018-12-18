#pragma once

#include <SFML/System/Vector2.hpp>

namespace VectorMath
{
	sf::Vector2f normalize(const sf::Vector2f &vector);
	float getMagnitude(const sf::Vector2f &vector);
};
