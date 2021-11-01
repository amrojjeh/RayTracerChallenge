#pragma once

#define M_PI 3.14159265358979323846f
#include <cmath>
#include <iostream>
#include <array>
#include <cstddef>
#include<string>
#include <cassert>

#define EPSILON 0.00001

struct OutOfBounds {
	std::size_t x, y;
};

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


template<std::size_t N>
struct Matrix {
	typedef Matrix<4> Transform;
	float elements[N][N];
	static const Transform identity;
	static Transform translation(float x, float y, float z);
	static Transform scaling(float x, float y, float z);
	static Transform rotation_x(float radians);
	static Transform rotation_y(float radians);
	static Transform rotation_z(float radians);
	static Transform shearing(float xy, float xz, float yx, float yz, float zx, float zy);

	Transform translate(float x, float y, float z) const;
	Transform scale(float x, float y, float z) const;
	Transform rotate_x(float radians) const;
	Transform rotate_y(float radians) const;
	Transform rotate_z(float radians) const;
	Transform shear(float xy, float xz, float yx, float yz, float zx, float zy) const;


	Matrix();
	Matrix(const float(&list)[N][N]);
	Matrix(const Matrix<N> &m);

	float* operator[](std::size_t row);
	const float* operator[](std::size_t row) const;
	bool operator==(const Matrix<N> other) const;
	bool operator!=(Matrix<N> other) const;
	Matrix<N> operator*(Matrix<N> other) const;
	Tuple operator*(const Tuple& other) const;
	Matrix<N> transpose() const;
	float determinant() const;
	Matrix<N - 1> submatrix(std::size_t row, std::size_t col) const;
	float minor(std::size_t row, std::size_t col) const;
	float cofactor(std::size_t row, std::size_t col) const;
	bool invertible() const;
	Matrix<N> inverse() const;
};

typedef Matrix<4> Transform;


struct Canvas {
	const std::size_t width;
	const std::size_t height;
	Tuple* canvas;

	Canvas(std::size_t width, std::size_t height);
	
	std::size_t index(std::size_t x, std::size_t y) const;
	void write_pixel(std::size_t x, std::size_t y, Color c);
	Color read_pixel(std::size_t x, std::size_t y) const;

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

template<std::size_t N>
const Matrix<4> Matrix<N>::identity = { {
	{1.f, 0.f, 0.f, 0.f},
	{0.f, 1.f, 0.f, 0.f},
	{0.f, 0.f, 1.f, 0.f},
	{0.f, 0.f, 0.f, 1.f}
} };

template<std::size_t N>
Transform Matrix<N>::translation(float x, float y, float z) {
	Matrix result = { {
		{1.f, 0.f, 0.f, x},
		{0.f, 1.f, 0.f, y},
		{0.f, 0.f, 1.f, z},
		{0.f, 0.f, 0.f, 1.f}
	}};
	return result;
}

template<std::size_t N>
Transform Matrix<N>::scaling(float x, float y, float z) {
	Matrix result = { {
		{x, 0.f, 0.f, 0.f},
		{0.f, y, 0.f, 0.f},
		{0.f, 0.f, z, 0.f},
		{0.f, 0.f, 0.f, 1.f}
	} };
	return result;
}

template<std::size_t N>
Transform Matrix<N>::rotation_x(float r) {
	float a = std::sin(r);
	float b = std::cos(r);
	Matrix result = { {
		{1.f, 0.f, 0.f, 0.f},
		{0.f, b, -a, 0.f},
		{0.f, a, b, 0.f},
		{0.f, 0.f, 0.f, 1.f}
	} };
	return result;
}

template<std::size_t N>
Transform Matrix<N>::rotation_y(float r) {
	float a = std::sin(r);
	float b = std::cos(r);
	Matrix result = { {
		{b, 0.f, a, 0.f},
		{0.f, 1.f, 0.f, 0.f},
		{-a, 0.f, b, 0.f},
		{0.f, 0.f, 0.f, 1.f}
	} };
	return result;
}

template<std::size_t N>
Transform Matrix<N>::rotation_z(float r) {
	float a = std::sin(r);
	float b = std::cos(r);
	Matrix result = { {
		{b, -a, 0.f, 0.f},
		{a, b, 0.f, 0.f},
		{0.f, 0.f, 1.f, 0.f},
		{0.f, 0.f, 0.f, 1.f}
	} };
	return result;
}

template<std::size_t N>
Transform Matrix<N>::shearing(float xy, float xz, float yx, float yz, float zx, float zy) {
	Matrix result = { {
		{1.f, xy, xz, 0.f},
		{yx, 1.f, yz, 0.f},
		{zx, zy, 1.f, 0.f},
		{0.f, 0.f, 0.f, 1.f}
	} };
	return result;
}

template<std::size_t N>
Transform Matrix<N>::translate(float x, float y, float z) const {
	return Transform::translation(x, y, z) * (*this);
}

template<std::size_t N>
Transform Matrix<N>::scale(float x, float y, float z) const {
	return Transform::scaling(x, y, z) * (*this);
}

template<std::size_t N>
Transform Matrix<N>::rotate_x(float r) const {
	return Transform::rotation_x(r) * (*this);
}

template<std::size_t N>
Transform Matrix<N>::rotate_y(float r) const {
	return Transform::rotation_y(r) * (*this);
}

template<std::size_t N>
Transform Matrix<N>::rotate_z(float r) const {
	return Transform::rotation_z(r) * (*this);
}

template<std::size_t N>
Transform Matrix<N>::shear(float xy, float xz, float yx, float yz, float zx, float zy) const {
	return Transform::shearing(xy, xz, yx, yz, zx, zy) * (*this);
}

template<std::size_t N>
Matrix<N>::Matrix(const float(&list)[N][N])
{
	static_assert(N >= 2, "Matrix must be at least 2x2.");
	for (int x = 0; x < N; x++) {
		for (int y = 0; y < N; y++) {
			elements[x][y] = list[x][y];
		}
	}
}

template<std::size_t N>
Matrix<N>::Matrix()
{
	static_assert(N >= 2, "Matrix must be at least 2x2.");
}

template<std::size_t N>
inline Matrix<N>::Matrix(const Matrix<N> &m)
{
	for (std::size_t x = 0; x < N; ++x) {
		for (std::size_t y = 0; y < N; ++y) {
			elements[x][y] = m[x][y];
		}
	}
}

template<std::size_t N>
float* Matrix<N>::operator[](std::size_t row){
	return elements[row];
}

template<std::size_t N>
const float* Matrix<N>::operator[](std::size_t row) const {
	return elements[row];
}

template<std::size_t N>
bool Matrix<N>::operator==(Matrix<N> other) const {
	for (std::size_t x = 0; x < N; ++x) {
		for (std::size_t y = 0; y < N; ++y) {
			if (std::abs(elements[x][y] - other[x][y]) > EPSILON)
				return false;
		}
	}
	return true;
}

template<std::size_t N>
bool Matrix<N>::operator!=(Matrix<N> other) const {
	return !(*this == other);
}

template<std::size_t N>
Matrix<N> Matrix<N>::operator*(Matrix<N> other) const
{
	Matrix<N> m;
	for (std::size_t x = 0; x < N; ++x) {
		for (std::size_t y = 0; y < N; ++y) {
			float result = 0.f;
			for (std::size_t z = 0; z < N; ++z) {
				result += elements[x][z] * other[z][y];
			}
			m[x][y] = result;
		}
	}
	return m;
}

// Just a bunch of dot products
template<std::size_t N>
Tuple Matrix<N>::operator*(const Tuple& other) const {
	static_assert(N == 4, "Multiplying tuples with different sizes is not supported.");
	return Tuple{
		elements[0][0] * other.x + elements[0][1] * other.y + elements[0][2] * other.z + elements[0][3] * other.w,
		elements[1][0] * other.x + elements[1][1] * other.y + elements[1][2] * other.z + elements[1][3] * other.w,
		elements[2][0] * other.x + elements[2][1] * other.y + elements[2][2] * other.z + elements[2][3] * other.w,
		elements[3][0] * other.x + elements[3][1] * other.y + elements[3][2] * other.z + elements[3][3] * other.w,
	};
}

template<std::size_t N>
Matrix<N> Matrix<N>::transpose() const {
	Matrix<N> result;
	for (std::size_t x = 0; x < N; ++x) {
		for (std::size_t y = 0; y < N; ++y) {
			result[x][y] = elements[y][x];
		}
	}
	return result;
}

template<std::size_t N>
float Matrix<N>::determinant() const {
	float result = 0.f;
	for (std::size_t x = 0; x < N; ++x) {
		result += elements[0][x] * cofactor(0, x);
	}
	return result;
}

float Matrix<2>::determinant() const {
	return elements[0][0] * elements[1][1] - elements[0][1] * elements[1][0];
}

template<std::size_t N>
Matrix<N - 1> Matrix<N>::submatrix(std::size_t row, std::size_t col) const {
	static_assert(N > 2, "Submatrix for 2x2 or less does not exist.");
	Matrix<N - 1> result;
	for (std::size_t x = 0; x < N; ++x) {
		for (std::size_t y = 0; y < N; ++y) {
			if (x == row) continue;
			if (y == col) continue;
			std::size_t ix = x - ((x > row) ? 1 : 0);
			std::size_t iy = y - ((y > col) ? 1 : 0);
			result[ix][iy] = elements[x][y];
		}
	}
	return result;
}

template<std::size_t N>
float Matrix<N>::minor(std::size_t row, std::size_t col) const {
	static_assert(N > 2, "Submatrix for 2x2 or less does not exist.");
	return submatrix(row, col).determinant();
}

template<std::size_t N>
float Matrix<N>::cofactor(std::size_t row, std::size_t col) const {
	static_assert(N > 2, "Submatrix for 2x2 or less does not exist.");
	return minor(row, col) * ((row + col) % 2 == 0 ? 1 : -1);
}

template<std::size_t N>
bool Matrix<N>::invertible() const {
	return determinant() != 0;
}

template<std::size_t N>
Matrix<N> Matrix<N>::inverse() const {
	float d = determinant();
	assert(d != 0);
	Matrix<N> result;
	for (std::size_t x = 0; x < N; ++x) {
		for (std::size_t y = 0; y < N; ++y) {
			result[y][x] = cofactor(x, y) / d;
		}
	}
	return result;
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
	return std::abs(x - other.x) <= EPSILON &&
		std::abs(y - other.y) <= EPSILON &&
		std::abs(z - other.z) <= EPSILON &&
		std::abs(w - other.w) <= EPSILON;
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

template<std::size_t N>
std::ostream& operator<<(std::ostream& output, Matrix<N> mat) {
	for (std::size_t x = 0; x < N; ++x) {
		output << "[";
		for (std::size_t y = 0; y < N; ++y) {
			output << mat[x][y] << ", ";
		}
		output << "]\n";
	}
	return output;
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
std::size_t Canvas::index(std::size_t x, std::size_t y) const {
	return x + y * width;
}

void Canvas::write_pixel(std::size_t x, std::size_t y, Color c) {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		throw new OutOfBounds{x, y};
	}
	canvas[index(x, y)] = c;
}

Color Canvas::read_pixel(std::size_t x, std::size_t y) const {
	return canvas[index(x, y)];
}

CanvasToPPM::CanvasToPPM(Canvas c, int maxval = 255) : c{ c }, maxval{ maxval } {

}

// Newlines are appended if the line exceeds 70 characters or the row of pixels ends
std::string CanvasToPPM::toPlainPPM() {
	result = std::string{ "P3\n" } + std::to_string(c.width) + " " + std::to_string(c.height) + "\n" + std::to_string(maxval) + "\n";
	// generate pixel data
	// TODO: Write an iterator for canvas
	for (std::size_t x = 0; x < c.height * c.width; ++x) {
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
