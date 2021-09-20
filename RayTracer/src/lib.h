#pragma once

#include <cmath>
#include <iostream>
#include <array>
#include <cstddef>
#include<string>

using slf = std::numeric_limits<float>;

struct RGB {
	float &red, &green, &blue;
};

typedef struct Tuple {
	float x, y, z, w;

	Tuple() = default;
	Tuple(const Tuple& other) = default;
	Tuple(Tuple&& other) = default;
	~Tuple() = default;

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
	Tuple schur(const Tuple& other) const;
	RGB getRGB();
} Point, Vector, Color;

struct Canvas {
	const std::size_t width;
	const std::size_t height;
	Tuple* canvas;

	Canvas(std::size_t width, std::size_t height);
	
	std::size_t index(int x, int y) const;
	void write_pixel(int x, int y, Color c);
	Color read_pixel(int x, int y) const;

	// For some reason, this signature does not work
	//Tuple[] operator[](std::size_t index);
};

class CanvasToPPM {
public:
	CanvasToPPM(Canvas c, int maxval);
	std::string toPlainPPM();
private:
	int line_length = 0;
	const int max_line_length = 70; // including newline
	const Canvas c;
	const int maxval;
	std::string result;
	void appendSampleStr(float sample);
};

template<class T>
T clamp(T v, T a, T b) {
	if (v < a)
		return a;
	if (b < v)
		return b;
	return v;
}

Color color(float r, float g, float b) {
	return Color{ r, g, b };
}

Tuple point(float x, float y, float z) {
	return Point{ x, y, z, 1.0 };
}

Tuple vector(float x, float y, float z) {
	return Vector{ x, y, z, 0.0 };
}

bool Tuple::operator==(const Tuple& other) const {
	return std::abs(x - other.x) <= slf::epsilon() &&
		std::abs(y - other.y) <= slf::epsilon() &&
		std::abs(z - other.z) <= slf::epsilon() &&
		std::abs(w - other.w) <= slf::epsilon();
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

RGB Tuple::getRGB() {
	return RGB{ x, y, z };
}

Tuple Tuple::schur(const Tuple& other) const {
	return Tuple{
		x * other.x,
		y * other.y,
		z * other.z,
		w * other.w
	};
}

Canvas::Canvas(std::size_t width, std::size_t height) : width{ width }, height{height} {
	canvas = new Tuple[width * height];
	for (std::size_t x = 0; x < width * height; ++x) {
		canvas[x] = color(0, 0, 0);
	}
}

// Assumes x and y are in range
std::size_t Canvas::index(int x, int y) const {
	return x + y * width;
}

struct OutOfBounds {
	int x, y;
};

void Canvas::write_pixel(int x, int y, Color c) {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		throw new OutOfBounds{x, y};
	}
	canvas[index(x, y)] = c;
}

Color Canvas::read_pixel(int x, int y) const {
	return canvas[index(x, y)];
}

CanvasToPPM::CanvasToPPM(Canvas c, int maxval = 255) : c{ c }, maxval{ maxval } {

}

// Newlines are appended if the line exceeds 70 characters or the row of pixels ends
std::string CanvasToPPM::toPlainPPM() {
	result = std::string{ "P3\n" } + std::to_string(c.width) + " " + std::to_string(c.height) + "\n" + std::to_string(maxval) + "\n";
	// generate pixel data
	// TODO: Write an iterator for canvas
	for (int x = 0; x < c.height * c.width; ++x) {
		if (x != 0 && x % c.width == 0) {
			result[result.length() - 1] = '\n';
			line_length = 0;
		}
		appendSampleStr(c.canvas[x].getRGB().red);
		appendSampleStr(c.canvas[x].getRGB().green);
		appendSampleStr(c.canvas[x].getRGB().blue);
	}
	if (result[result.length() - 1] != '\n')
		result[result.length() - 1] = '\n';
	return result;
}

void CanvasToPPM::appendSampleStr(float sample) {
	std::string sampleStr = std::to_string(clamp(static_cast<int>(std::round(maxval * sample)), 0, maxval)) + " ";
	int diff = 70 - line_length - sampleStr.length();
	if (diff > 0) {
		line_length += sampleStr.length();
		result += sampleStr;
	}
	else if (diff == 0) {
		sampleStr[sampleStr.length() - 1] = '\n';
		line_length = 0;
		result += sampleStr;
	}

	else {
		line_length = sampleStr.length();
		result[result.length() - 1] = '\n';
		result += sampleStr;
	}
}
