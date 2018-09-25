#include "VectorMath.h"

#include <math.h>

namespace VectorMath
{
	sf::Vector2f normalize(const sf::Vector2f &vector)
	{
		float length = getMagnitude(vector);
		
		sf::Vector2f normalizedVector;
		normalizedVector.x = vector.x / length;
		normalizedVector.y = vector.y / length;

		return normalizedVector;
	}

	float getMagnitude(const sf::Vector2f &vector)
	{
		return sqrtf((vector.x * vector.x + vector.y * vector.y));
	}
}
