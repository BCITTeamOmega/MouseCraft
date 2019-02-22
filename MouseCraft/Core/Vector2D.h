#pragma once
#include <cmath>
#include <exception>

/*Aaron's code from the previous project*/

class Vector2D
{
public:
	float x;
	float y;

	Vector2D();
	Vector2D(float x, float y);
	Vector2D(const Vector2D& v);
	Vector2D operator=(const Vector2D &v);
	Vector2D operator+(const Vector2D& v) const;
	Vector2D operator-(const Vector2D& v) const;
	Vector2D operator-() const;
	Vector2D operator+=(const Vector2D& v);
	Vector2D operator-=(const Vector2D& v);
	Vector2D operator*(const float& f) const;
	Vector2D operator*=(const float& f);
	float dot(const Vector2D& v) const;
	float project(const Vector2D& v) const;
	Vector2D unit() const;
	float length() const;
	~Vector2D();
};

Vector2D operator*(const float& f, const Vector2D& v);