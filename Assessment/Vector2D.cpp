#include "Vector2D.h"

Vector2D::Vector2D() {}

Vector2D::Vector2D(double x, double y)
{
	this->x = x;
	this->y = y;
}

Vector2D operator+(Vector2D const& v1, Vector2D const& v2)
{
	return { v1.x + v2.x, v1.y + v2.y };
}

bool operator==(Vector2D const& v1, Vector2D const& v2)
{
	return v1.x == v2.x && v1.y == v2.y;
}

bool operator!=(Vector2D const& v1, Vector2D const& v2)
{
	return v1.x != v2.x || v1.y != v2.y;
}

Vector2D operator/(Vector2D const& vector, double const& value)
{
	if (value == 0)
		return vector;
	
	return { vector.x / value, vector.y / value };
}

Vector2D operator-(Vector2D const& v1, Vector2D const& v2)
{
	return { v1.x - v2.x, v1.y - v2.y };
}

Vector2D operator*(double value, Vector2D const& vector)
{
	return { vector.x * value, vector.y * value };
}
