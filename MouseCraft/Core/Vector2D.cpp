#include "Vector2D.h"

Vector2D::Vector2D() : x(0), y(0) {}

Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

Vector2D::Vector2D(const Vector2D& v)
{
	this->x = v.x;
	this->y = v.y;
}

Vector2D Vector2D::operator=(const Vector2D& v2)
{
	this->x = v2.x;
	this->y = v2.y;
	return *this;
}

Vector2D Vector2D::operator+(const Vector2D& v2) const
{
	Vector2D v(this->x + v2.x, this->y + v2.y);
	return v;
}

Vector2D Vector2D::operator-(const Vector2D& v2) const
{
	Vector2D v(this->x - v2.x, this->y - v2.y);
	return v;
}

Vector2D Vector2D::operator-() const
{
	Vector2D v(-this->x, -this->y);
	return v;
}

Vector2D Vector2D::operator+=(const Vector2D& v2) {
	this->x += v2.x;
	this->y += v2.y;
	Vector2D v(this->x, this->y);
	return v;
}

Vector2D Vector2D::operator-=(const Vector2D& v2) {
	this->x -= v2.x;
	this->y -= v2.y;
	Vector2D v(this->x, this->y);
	return v;
}

Vector2D Vector2D::operator*(const float& f) const {
	Vector2D v(this->x * f, this->y * f);
	return v;
}

Vector2D Vector2D::operator*=(const float& f) {
	this->x *= f;
	this->y *= f;
	Vector2D v(this->x, this->y);
	return v;
}

float Vector2D::dot(const Vector2D& v) const
{
	return this->x * v.x + this->y * v.y;
}

float Vector2D::project(const Vector2D& v2) const
{
	try {
		return this->dot(v2.unit());
	}
	catch (std::exception& _) {
		throw std::exception("Cannot project onto vector (0, 0)");
	}
}

Vector2D Vector2D::unit() const
{
	if (this->x == 0 && this->y == 0) {
		throw std::exception("Unit vector of (0, 0) does not exist.");
	}
	float f = this->length();
	Vector2D v(this->x / f, this->y / f);
	return v;
}

float Vector2D::length() const
{
	return std::sqrtf(this->x * this->x + this->y * this->y);
}

Vector2D::~Vector2D()
{
}

Vector2D operator*(const float& f, const Vector2D& v)
{
	Vector2D v2 = v * f;
	return v2;
}