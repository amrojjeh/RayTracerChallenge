#include <gtest/gtest.h>
#include "lib.h" // includes cmath

TEST(Clamp, clamping) {
	ASSERT_EQ(25, clamp(25, 0, 30));
	ASSERT_EQ(25, clamp(30, 0, 25));
	ASSERT_EQ(25, clamp(0, 25, 30));
}

TEST(Tuples, points) {
	Tuple t1 = point(4.3f, -4.2f, 3.1f);
	ASSERT_FLOAT_EQ(t1.x, 4.3f);
	ASSERT_FLOAT_EQ(t1.y, -4.2f);
	ASSERT_FLOAT_EQ(t1.z, 3.1f);
	ASSERT_FLOAT_EQ(t1.w, 1.0f);
	ASSERT_TRUE(t1.isPoint());
	ASSERT_FALSE(t1.isVector());
}

TEST(Tuples, vectors) {
	Tuple t1 = vector(4.3f, -4.2f, 3.1f);
	ASSERT_FLOAT_EQ(t1.x, 4.3f);
	ASSERT_FLOAT_EQ(t1.y, -4.2f);
	ASSERT_FLOAT_EQ(t1.z, 3.1f);
	ASSERT_FLOAT_EQ(t1.w, 0.0f);
	ASSERT_FALSE(t1.isPoint());
	ASSERT_TRUE(t1.isVector());
}

TEST(Tuples, equality) {
	Tuple t1{ 1.f, 2.f, 3.f, 0.f };
	Tuple t2{ 1.f, 2.f, 3.f, 0.f };
	ASSERT_TRUE(t1 == t2);
	Tuple t3{ 1.f, 2.f, -3.f, 0.f};
	ASSERT_FALSE(t3 == t2);
	ASSERT_FALSE(t3 == t1);
}

TEST(Tuples, adding) {
	Tuple t1{ 3, -2, 5, 1 };
	Tuple t2{ -2, 3, 1, 0 };
	Tuple result = t1 + t2;
	Tuple expected{ 1, 1, 6, 1 };
	ASSERT_TRUE(result == expected);
}

TEST(Tuples, pSubtract) {
	Tuple p1 = point(3, 2, 1);
	Tuple p2 = point(5, 6, 7);
	Tuple result = p1 - p2;
	Tuple expected = vector(-2, -4, -6);
	ASSERT_TRUE(expected == result);
}

TEST(Tuples, pvSubtract) {
	Tuple p = point(3, 2, 1);
	Tuple v = vector(5, 6, 7);
	Tuple result = p - v;
	Tuple expected = point(-2, -4, -6);
	ASSERT_TRUE(expected == result);
}

TEST(Tuples, vSubtract) {
	Tuple v1 = vector(3, 2, 1);
	Tuple v2 = vector(5, 6, 7);
	Tuple result = v1 - v2;
	Tuple expected = vector(-2, -4, -6);
	ASSERT_TRUE(result == expected);
}

TEST(Tuples, uncivilziedNegate) {
	Tuple zero = vector(0, 0, 0);
	Tuple v = vector(1, -2, 3);
	Tuple result = zero - v;
	Tuple expected{-1, 2, -3};
	ASSERT_TRUE(expected == result);
}

TEST(Tuples, properNegate) {
	Tuple t1{1, -2, 3, -4};
	Tuple result = -t1;
	Tuple expected{-1, 2, -3, 4};
	ASSERT_TRUE(expected == result);
}

TEST(Tuples, scalarMultiplication) {
	Tuple t1{1, -2, 3, -4};
	Tuple result = 3.5f * t1;
	Tuple expected{3.5f, -7.f, 10.5f, -14.f};
	ASSERT_TRUE(result == expected);
}

TEST(Tuples, scalarDivision) {
	Tuple t{1, -2, 3, -4};
	Tuple result = t / 2.f;
	Tuple expected{0.5f, -1.f, 1.5f, -2.f};
	ASSERT_TRUE(result == expected);
}

TEST(Tuples, magnitude) {
	ASSERT_FLOAT_EQ(vector(1.f, 0.f, 0.f).magnitude(), 1.f);
	ASSERT_FLOAT_EQ(vector(0.f, 1.f, 0.f).magnitude(), 1.f);
	ASSERT_FLOAT_EQ(vector(0.f, 0.f, 1.f).magnitude(), 1.f);
	ASSERT_FLOAT_EQ(vector(1.f, 2.f, 3.f).magnitude(), std::sqrtf(14.f));
	ASSERT_FLOAT_EQ(vector(-1.f, -2.f, -3.f).magnitude(), std::sqrtf(14.f));
}

TEST(Tuples, normalizing) {
	ASSERT_TRUE(vector(4.f, 0.f, 0.f).normalize() == vector(1, 0.f, 0.f));
	ASSERT_TRUE(vector(1.f, 2.f, 3.f).normalize() == vector(1.f / sqrtf(14.f), 2.f / sqrtf(14.f), 3.f / sqrtf(14.f)));
	ASSERT_FLOAT_EQ(vector(1.f, 2.f, 3.f).normalize().magnitude(), 1.f);
}

TEST(Tuples, dotProduct) {
	ASSERT_FLOAT_EQ(vector(1.0f, 2.0f, 3.f).dot(vector(2.f, 3.f, 4.f)), 20.f);
}

TEST(Tuples, crossProduct) {
	Tuple v1{ vector(1.0f, 2.0f, 3.0f) };
	Tuple v2{ vector(2.0f, 3.0f, 4.0f) };

	Tuple result1{ v1.cross(v2) };
	Tuple result2{ v2.cross(v1) };

	Tuple expected1{ vector(-1.0f, 2.0f, -1.0f) };
	Tuple expected2{ vector(1.0f, -2.0f, 1.0f) };

	ASSERT_TRUE(result1 == expected1);
	ASSERT_TRUE(result2 == expected2);
}

TEST(Colors, creation) {
	Color c = color(-0.5f, 0.4f, 1.7f);
	ASSERT_FLOAT_EQ(-0.5f, c.getRGB().red);
	ASSERT_FLOAT_EQ(0.4f, c.getRGB().green);
	ASSERT_FLOAT_EQ(1.7f, c.getRGB().blue);
	c.getRGB().blue = 0.f;
	ASSERT_FLOAT_EQ(0.f, c.getRGB().blue);
	ASSERT_FLOAT_EQ(0.f, c.z);
}

TEST(Colors, addingColors) {
	Color c1{ color(0.9f, 0.6f, 0.75f) };
	Color c2{ color(0.7f, 0.1f, 0.25f) };
	Color result{ c1 + c2 };
	Color expected{ color(1.6f, 0.7f, 1.0f) };
	ASSERT_TRUE(result == expected);
}

TEST(Colors, subtractingColors) {
	Color c1{ color(0.9f, 0.6f, 0.75f) };
	Color c2{ color(0.7f, 0.1f, 0.25f) };
	Color result{ c1 - c2 };
	Color expected{ color(0.2f, 0.5f, 0.5f) };
	ASSERT_TRUE(result == expected);
}

TEST(Colors, multiplyingColors) {
	Color c1{ color(0.2f, 0.3f, 0.4f) };
	ASSERT_TRUE(c1 * 2 == color(0.4f, 0.6f, 0.8f));
}

TEST(Colors, HadamardProduct) {
	Color c1{ color(1.0f, 0.2f, 0.4f) };
	Color c2{ color(0.9f, 1.0f, 0.1f) };
	ASSERT_TRUE(c1.schur(c2) == color(0.9f, 0.2f, 0.04f));
}

TEST(Canvas, init) {
	Canvas c(10, 20);
	ASSERT_EQ(c.width, 10);
	ASSERT_EQ(c.height, 20);
	for (std::size_t x = 0; x < c.height * c.width; ++x)
		ASSERT_TRUE(c.canvas[x] == color(0, 0, 0));
}

TEST(Canvas, writePixel) {
	Canvas c(10, 20);
	Color red = color(1.f, 0.f, 0.f);
	c.write_pixel(2, 3, red);
	red.getRGB().red = 0.f; // Check if canvas[2][3] is copying
	ASSERT_TRUE(c.read_pixel(2, 3) == color(1.f, 0.f, 0.f));
}

std::string head(std::string str, int lines) {
	std::size_t last = 0;
	while (lines-- > 0) {
		last = str.find('\n', last + 1);
		if (last == std::string::npos) {
			return str;
		}
	}
	return str.substr(0, last);
}

TEST(util, head) {
	std::string s1 = "We\nThink\nIn\nGeneralities\n";
	std::string result1 = head(s1, 3);
	std::string result2 = head(s1, 5);
	std::string expected1 = "We\nThink\nIn";
	std::string expected2 = "We\nThink\nIn\nGeneralities\n";
	ASSERT_STREQ(result1.c_str(), expected1.c_str());
	ASSERT_STREQ(result2.c_str(), expected2.c_str());
}

TEST(PlainPPM, ppmHeader) {
	Canvas c(10, 20);
	CanvasToPPM ppm{ c, 255 };
	std::string output = ppm.toPlainPPM();
	ASSERT_STREQ("P3\n10 20\n255", head(output, 3).c_str());
}

TEST(PlainPPM, ppmPixelData) {
	Canvas c(5, 3);
	Color c1 = color(1.5f, 0.f, 0.f);
	Color c2 = color(0.f, 0.5f, 0.f);
	Color c3 = color(-0.5f, 0.f, 1.f);
	c.write_pixel(0, 0, c1);
	c.write_pixel(2, 1, c2);
	c.write_pixel(4, 2, c3);
	CanvasToPPM ppm{ c, 255 };
	std::string ppmStr = ppm.toPlainPPM();
	std::string expected =
		"P3\n"
		"5 3\n"
		"255\n"
		"255 0 0" " 0 0 0" " 0 0 0" " 0 0 0" " 0 0 0\n"
		"0 0 0" " 0 0 0" " 0 128 0" " 0 0 0" " 0 0 0\n"
		"0 0 0" " 0 0 0" " 0 0 0" " 0 0 0" " 0 0 255\n";
	ASSERT_STREQ(expected.c_str(), ppmStr.c_str());
}

TEST(PlainPPM, splittingLongLines) {
	Canvas c(10, 2);
	Color c1 = color(1.f, 0.8f, 0.6f);
	for (std::size_t x = 0; x < c.height * c.width; ++x) {
		c.canvas[x] = c1;
	}
	CanvasToPPM ppm{ c, 255 };
	std::string result = ppm.toPlainPPM();
	std::string expected =
		"P3\n"
		"10 2\n"
		"255\n"
		"255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
		"153 255 204 153 255 204 153 255 204 153 255 204 153\n"
		"255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
		"153 255 204 153 255 204 153 255 204 153 255 204 153\n";
	ASSERT_STREQ(expected.c_str(), result.c_str());
}

TEST(PlainPPM, endsWithNewLine) {
	Canvas c{ 5, 3 };
	CanvasToPPM ppm{ c, 255 };
	std::string result = ppm.toPlainPPM();
	ASSERT_EQ(result[result.length() - 1U], '\n');
}

TEST(Matrix, 4x4) {
	Matrix<4> m{ {
		{1.f, 2.f, 3.f, 4.f},
		{5.5f, 6.5f, 7.5f, 8.5f},
		{9.f, 10.f, 11.f, 12.f},
		{13.5f, 14.5f, 15.5f, 16.5f}
	} };

	ASSERT_FLOAT_EQ(m[0][0], 1.f);
	ASSERT_FLOAT_EQ(m[0][3], 4.f);
	ASSERT_FLOAT_EQ(m[1][0], 5.5f);
	ASSERT_FLOAT_EQ(m[1][2], 7.5f);
	ASSERT_FLOAT_EQ(m[2][2], 11.f);
	ASSERT_FLOAT_EQ(m[3][0], 13.5f);
	ASSERT_FLOAT_EQ(m[3][2], 15.5f);
}

TEST(Matrix, 2x2) {
	Matrix<2> m{ {
		{-3.f, 5.f},
		{1.f, -2.f},
	} };

	ASSERT_FLOAT_EQ(m[0][0], -3.f);
	ASSERT_FLOAT_EQ(m[0][1], 5.f);
	ASSERT_FLOAT_EQ(m[1][0], 1.f);
	ASSERT_FLOAT_EQ(m[1][1], -2.f);
}

TEST(Matrix, 3x3) {
	Matrix<3> m{ {
		{-3.f, 5.f, 0.f},
		{1.f, -2.f, -7.f},
		{0.f, 1.f, 1.f},
	} };

	ASSERT_FLOAT_EQ(m[0][0], -3.f);
	ASSERT_FLOAT_EQ(m[1][1], -2.f);
	ASSERT_FLOAT_EQ(m[2][2], 1.f);
}

TEST(Matrix, Equality) {
	Matrix<4> a{ {
		{1.f, 2.f, 3.f, 4.f},
		{5.f, 6.f, 7.f, 8.f},
		{9.f, 10.f, 11.f, 12.f},
		{13.f, 14.f, 15.f, 16.f},
	} };

	Matrix<4> b{ {
		{1.f, 2.f, 3.f, 4.f},
		{5.f, 6.f, 7.f, 8.f},
		{9.f, 10.f, 11.f, 12.f},
		{13.f, 14.f, 15.f, 16.f},
	} };

	ASSERT_TRUE(a == b);
	ASSERT_FALSE(a != b);
}

TEST(Matrix, Inequality) {
	Matrix<4> a{ {
		{1.f, 2.f, 3.f, 4.f},
		{5.f, 6.f, 7.f, 8.f},
		{9.f, 10.f, 11.f, 12.f},
		{13.f, 14.f, 15.f, 16.f},
	} };

	Matrix<4> b{ {
		{1.f, 2.f, 3.f, 4.f},
		{5.f, 5.f, 7.f, 8.f},
		{9.f, 10.f, 11.f, 12.f},
		{13.f, 14.f, 15.f, 16.f},
	} };

	ASSERT_FALSE(a == b);
	ASSERT_TRUE(a != b);
}
