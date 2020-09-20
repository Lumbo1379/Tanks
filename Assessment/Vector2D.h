#pragma once

class Vector2D
{
	public:
		Vector2D();
		Vector2D(double x, double y);
		friend Vector2D operator+ (Vector2D const& v1, Vector2D const& v2);
		friend bool operator== (Vector2D const& v1, Vector2D const& v2);
		friend bool operator!= (Vector2D const& v1, Vector2D const& v2);
		friend Vector2D operator/ (Vector2D const& vector, double const& value);
		friend Vector2D operator- (Vector2D const& v1, Vector2D const& v2);
		friend Vector2D operator* (double value, Vector2D const& vector);
	
		double x, y;
};