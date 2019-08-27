#include "Collision.h"

#include <cmath>

namespace Collision
{
	bool circleCircle(CircleShape& circle1, CircleShape& circle2)
	{
		float minimumDistance = circle1.radius + circle2.radius;
		sf::Vector2f distVec = circle1.position - circle2.position;
		float magnitude = sqrtf(distVec.x * distVec.x + distVec.y * distVec.y);
		float collisionDepth = minimumDistance - magnitude;

		if (collisionDepth > 0.0f)
			return true;

		return false;
	}

	bool circleRect(CircleShape& circle, RectangleShape& rect)
	{
		sf::Vector2f rectCenter = sf::Vector2f(
			rect.position.x + rect.width/2,
			rect.position.y + rect.height/2);

		sf::Vector2f circleRectDist = sf::Vector2f(
			std::abs(circle.position.x - rectCenter.x),
			std::abs(circle.position.y - rectCenter.y));

		if (circleRectDist.x > (rect.width/2.0f + circle.radius))
			return false;

		if (circleRectDist.y > (rect.height/2.0f + circle.radius))
			return false;

		if (circleRectDist.x <= (rect.width/2.0f))
			return true;

		if (circleRectDist.y <= (rect.height/2.0f))
			return true;

		float cornerDist = 
			(circleRectDist.x - rect.width/2.0f)*
			(circleRectDist.x - rect.width/2.0f) + 
			(circleRectDist.y - rect.height/2.0f)*
			(circleRectDist.y - rect.height/2.0f);

		return (cornerDist <= (circle.radius * circle.radius));
	}

	bool rectRect(RectangleShape& rect1, RectangleShape& rect2)
	{
		if (rect1.position.x + rect1.width >= rect2.position.x &&
			rect1.position.x <= rect2.position.x + rect2.width &&
			rect1.position.y + rect1.height >= rect2.position.y &&
			rect1.position.y <= rect2.position.y + rect2.height)
			return true;

		return false;
	}
};
