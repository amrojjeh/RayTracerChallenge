#pragma once

#include <cmath>
#include <iostream>

using slf = std::numeric_limits<float>;

typedef struct Tuple {
	float x;
	float y;
	float z;
	float w;

	Tuple(const Tuple& other) = default;
	Tuple(Tuple&& other) = default;

	bool isPoint() const;
	bool isVector() const;

	bool operator==(const Tuple& other) const;
	Tuple& operator=(const Tuple& other);
	Tuple operator+(const Tuple& other) const;
	Tuple operator-(const Tuple& other) const;
	Tuple operator*(float other) const;
	Tuple operator/(float other) const;
	Tuple operator-() const;
	float magnitude() const;
	Tuple normalize() const;
	Tuple cross(const Tuple& other) const;
	float dot(const Tuple& other) const;
} Point, Vector;

Tuple point(float x, float y, float z) {
	return Tuple{ x, y, z, 1.0 };
}

Tuple vector(float x, float y, float z) {
	return Tuple{ x, y, z, 0.0 };
}

bool Tuple::operator==(const Tuple& other) const {
	return std::abs(x - other.x) < slf::epsilon() &&
		std::abs(y - other.y) < slf::epsilon() &&
		std::abs(z - other.z) < slf::epsilon() &&
		std::abs(w - other.w) < slf::epsilon();
}

Tuple& Tuple::operator=(const Tuple& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}

Tuple Tuple::operator+(const Tuple& other) const {
	return Tuple{x + other.x, y + other.y, z + other.z, w + other.w};
}

Tuple Tuple::operator-(const Tuple& other) const {
	return {x - other.x, y - other.y, z - other.z, w - other.w};
}

Tuple Tuple::operator-() const {
	return { -x, -y, -z, -w };
}

Tuple Tuple::operator*(float other) const {
	return { x * other, y * other, z * other, w * other };
}

// Multipication is communitive!

Tuple operator*(float f, Tuple& t) {
	return { t.x * f, t.y * f, t.z * f, t.w * f };
}

Tuple Tuple::operator/(float other) const {
	return { x / other, y / other, z / other, w / other };
}

Tuple Tuple::normalize() const {
	float m = magnitude();
	return Tuple{ x / m, y / m, z / m, w / m };
}

float Tuple::magnitude() const {
	return std::sqrtf(x * x + y * y + z * z + w * w);
}

float Tuple::dot(const Tuple& other) const {
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

Tuple Tuple::cross(const Tuple& other) const {
	return vector(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x);
}

bool Tuple::isPoint() const {
	return w == 1.0;
}

bool Tuple::isVector() const {
	return w == 0.0;
}

std::ostream& operator<<(std::ostream& o, Tuple t) {
	if (t.isPoint()) {
		o << "point(";
	}
	else if (t.isVector()) {
		o << "vector(";
	}
	else {
		o << "tuple(";
	}

	o << t.x << ", " << t.y << ", " << t.z;

	if (!t.isPoint() && !t.isVector()) {
		o << ", " << t.w;
	}

	o << ")";
	return o;
}
