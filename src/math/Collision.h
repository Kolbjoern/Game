#pragma once

#include "Geometry.h"

namespace Collision
{
	bool circleCircle(CircleShape& circle1, CircleShape& circle2);
	bool circleRect(CircleShape& circle, RectangleShape& rect);
	bool rectRect(RectangleShape& rect1, RectangleShape& rect2);
};
